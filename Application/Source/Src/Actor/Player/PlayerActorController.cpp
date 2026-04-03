#include <format>

#include "PlayerDataChunk.h"

#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Manager/ConfigManager.h"
#include "Manager/ContextManager.h"
#include "Manager/DataChunkManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Particle/ParticleActor.h"
#include "Scene/IScene.h"
#include "Utils/LogUtils.h"

#include "Actor/Physics/MoveBoundModel.h"
#include "Actor/Player/PlayerActorController.h"
#include "Actor/Player/PlayerModel.h"
#include "Actor/UI/EffectTextActor.h"
#include "Actor/UI/EffectTextModel.h"
#include "App/AppDef.h"
#include "Config/GameConfig.h"
#include "Context/PlayerContext.h"

void PlayerActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	_inputMgr = InputManager::GetPtr();

	if (Result<void> result = InitializeContext(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_CONTEXT(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeModelFromConfig(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITAILIZE_MODEL_FROM_CONFIG(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeMoveSpeed(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITAILIZE_MODEL_MOVE_SPEED(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeMoveBoundModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MOVE_BOUND_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}
}

void PlayerActorController::OnRelease()
{
	Event<int32_t>& levelUpEvent = _context->GetLevelUpEvent();
	levelUpEvent.UnregisterCallback(NAME_OF(PlayerActorController));

	_inputMgr = nullptr;
	_model = nullptr;
}

void PlayerActorController::OnTick(float deltaSeconds)
{
	if (_model->IsDead())
	{
		return;
	}

	UpdateContext(deltaSeconds);
	UpdateMoveDirection();
	Move(deltaSeconds);
	UpdateDirectionByBounds();
}

void PlayerActorController::OnCollision(IActor* actor)
{
	if (_model->IsDead())
	{
		return;
	}

	_model->SetVisible(false);
	_model->SetDead(true);
	_context->RecordBestTime();
	_context->SetGameOver(true);

	GenerateParticleEffect();
}

Result<void> PlayerActorController::InitializeContext()
{
	ContextManager& contextMgr = ContextManager::Get();
	Result<PlayerContext*> result = contextMgr.GetContext<PlayerContext>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_context = result.GetValue();
	_context->Reset();

	Event<int32_t>& levelUpEvent = _context->GetLevelUpEvent();
	levelUpEvent.RegisterCallback(
		NAME_OF(PlayerActorController),
		[this](int32_t level)
		{
			UpdateMoveSpeed(level);
			GenerateLevelUpTextEffect();
		}
	);

	return Result<void>::Success();
}

Result<void> PlayerActorController::InitializeModel()
{
	Result<PlayerModel*> result = _ownerActor->GetModel<PlayerModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_model = result.GetValue();
	return Result<void>::Success();
}

Result<void> PlayerActorController::InitializeModelFromConfig()
{
	ConfigManager& configMgr = ConfigManager::Get();
	Result<const GameConfig*> result = configMgr.GetConfig<GameConfig>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const GameConfig* config = result.GetValue();
	_moveRangeMinX = static_cast<float>(config->GetPlayerMoveRangeMinX());
	_moveRangeMaxX = static_cast<float>(config->GetPlayerMoveRangeMaxX());
	_tabTextMoveSpeed = config->GetTabTextMoveSpeed();
	_tabTextLifeTime = config->GetTabTextLifeTime();
	_tabTextFontSize = config->GetTabTextFontSize();
	_tabTextOffsetY = config->GetTabTextOffsetY();
	_tabTextColor = config->GetTabTextColor();

	_levelUpTextMoveSpeed = config->GetLevelUpTextMoveSpeed();
	_levelUpTextLifeTime = config->GetLevelUpTextLifeTime();
	_levelUpTextFontSize = config->GetLevelUpTextFontSize();
	_levelUpTextOffsetY = config->GetLevelUpTextOffsetY();
	_levelUpTextColor = config->GetLevelUpTextColor();
	float moveRangeX = (_moveRangeMinX + _moveRangeMaxX) * 0.5f;
	float moveRangeY = static_cast<float>(config->GetPlayerMoveRangeY());
	bool isStartMovePositive = config->IsPlayerStartMovePositive();

	glm::vec2 position(moveRangeX, moveRangeY);
	glm::vec2 moveDirection(isStartMovePositive ? +1.0f : -1.0f, 0.0f);

	_model->SetPosition(position);
	_model->SetColor(config->GetPlayerColor());
	_model->SetRadius(config->GetPlayerRadius());
	_model->SetMoveDirection(moveDirection);
	_model->SetCollidable(true);

	return Result<void>::Success();
}

Result<void> PlayerActorController::InitializeMoveBoundModel()
{
	Result<MoveBoundModel*> result = _ownerActor->GetModel<MoveBoundModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	MoveBoundModel* moveBoundModel = result.GetValue();
	moveBoundModel->SetPosition(_model->GetPosition());
	moveBoundModel->SetRadius(_model->GetRadius() * 1.5f);
	moveBoundModel->SetHeight(_moveRangeMaxX - _moveRangeMinX);
	moveBoundModel->SetRotate(DEF::ANGLE_90_DEG);
	moveBoundModel->SetColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));

	return Result<void>::Success();
}

Result<void> PlayerActorController::InitializeMoveSpeed()
{
	Result<const PlayerDataChunk*> result = DataChunkManager::Get().GetDataChunk<PlayerDataChunk>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const PlayerDataChunk* dataChunk = result.GetValue();
	int32_t idx = dataChunk->LevelToIdx.at(_context->GetLevel());
	int32_t moveSpeed = dataChunk->DataPacks[idx].Speed;

	_model->SetMoveSpeed(static_cast<float>(moveSpeed));

	return Result<void>::Success();
}

void PlayerActorController::UpdateContext(float deltaSeconds)
{
	_context->AddPlayTime(deltaSeconds);
}

void PlayerActorController::UpdateMoveDirection()
{
	glm::vec2 direction = _model->GetMoveDirection();
	if (_inputMgr->GetKeyPress(EKey::SPACE) == EPress::PRESSED)
	{
		direction.x *= -1.0f;
		GenerateTabTextEffect();
	}
	_model->SetMoveDirection(direction);
}

void PlayerActorController::Move(float deltaSeconds)
{
	glm::vec2 position = _model->GetPosition();
	glm::vec2 direction = _model->GetMoveDirection();
	float speed = _model->GetMoveSpeed();

	position += direction * deltaSeconds * speed;
	position.x = glm::clamp(position.x, _moveRangeMinX, _moveRangeMaxX);

	_model->SetPosition(position);
}

void PlayerActorController::UpdateDirectionByBounds()
{
	glm::vec2 position = _model->GetPosition();
	glm::vec2 direction = _model->GetMoveDirection();

	if ((position.x <= _moveRangeMinX && direction.x < 0.0f) ||
		(position.x >= _moveRangeMaxX && direction.x > 0.0f))
	{
		direction.x *= -1.0f;
		_model->SetMoveDirection(direction);
	}
}

void PlayerActorController::GenerateParticleEffect()
{
	ConfigManager& configMgr = ConfigManager::Get();
	Result<const GameConfig*> result = configMgr.GetConfig<GameConfig>();
	if (!result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG");
		return;
	}

	const GameConfig* config = result.GetValue();
	ParticleActorParam param{
		_model->GetPosition(),
		config->GetParticleCount(),
		config->GetParticleMinSize(),
		config->GetParticleMaxSize(),
		config->GetParticleMinSpeed(),
		config->GetParticleMaxSpeed(),
		config->GetParticleLifeTime(),
		_model->GetColor()
	};

	SceneManager& sceneMgr = SceneManager::Get();
	IScene* currentScene = sceneMgr.GetCurrentScene();
	if (Result<ParticleActor*> result = currentScene->CreateAndAddActor<ParticleActor>(DEF::PARTICLE_ACTOR_NAME, DEF::SCENE_PARTICLE_ACTOR_ORDER, param); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_CREATE_OR_ADD_PARTICLE_ACTOR(key:{0})", DEF::PARTICLE_ACTOR_NAME);
		return;
	}
}

void PlayerActorController::GenerateTabTextEffect()
{
	EffectTextModel* model = FindAvailableEffectTextModel();
	if (model == nullptr)
	{
		model = CreateAndRegisterEffectText();
		if (model == nullptr)
		{
			LOG_E("FAILED_TO_CREATE_EFFECT_TEXT_ACTOR");
			return;
		}
	}

	ActivateEffectTextModel(model, "TAB!", _tabTextFontSize, _tabTextColor, _tabTextMoveSpeed, _tabTextLifeTime, _tabTextOffsetY);
}

void PlayerActorController::GenerateLevelUpTextEffect()
{
	EffectTextModel* model = FindAvailableEffectTextModel();
	if (model == nullptr)
	{
		model = CreateAndRegisterEffectText();
		if (model == nullptr)
		{
			LOG_E("FAILED_TO_CREATE_EFFECT_TEXT_ACTOR");
			return;
		}
	}

	ActivateEffectTextModel(model, "LEVEL UP!", _levelUpTextFontSize, _levelUpTextColor, _levelUpTextMoveSpeed, _levelUpTextLifeTime, _levelUpTextOffsetY);
}

EffectTextModel* PlayerActorController::FindAvailableEffectTextModel() const
{
	for (EffectTextModel* model : _effectTextModelPool)
	{
		if (model->GetState() == EEffectTextState::DEAD)
		{
			return model;
		}
	}
	return nullptr;
}

EffectTextModel* PlayerActorController::CreateAndRegisterEffectText()
{
	SceneManager& sceneMgr = SceneManager::Get();
	IScene* currentScene = sceneMgr.GetCurrentScene();

	std::string key = std::format("{0}_{1}", DEF::EFFECT_TEXT_ACTOR_KEY_PREFIX, _effectTextModelPool.size());
	Result<EffectTextActor*> createResult = currentScene->CreateAndAddActor<EffectTextActor>(key, DEF::SCENE_EFFECT_TEXT_ACTOR_ORDER);
	if (!createResult.IsSuccess())
	{
		LOG_E("FAILED_TO_CREATE_AND_ADD_EFFECT_TEXT_ACTOR(key:{0})", key);
		return nullptr;
	}

	EffectTextActor* actor = createResult.GetValue();
	Result<EffectTextModel*> getResult = actor->GetModel<EffectTextModel>();
	if (!getResult.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_EFFECT_TEXT_MODEL(key:{0})", key);
		return nullptr;
	}

	EffectTextModel* model = getResult.GetValue();
	_effectTextModelPool.push_back(model);

	return model;
}

void PlayerActorController::ActivateEffectTextModel(EffectTextModel* model, const std::string& text, float fontSize, const glm::vec4& color, float moveSpeed, float lifetime, float offsetY)
{
	glm::vec2 position = _model->GetPosition();
	position.y -= offsetY;

	model->SetText(text);
	model->SetPosition(position);
	model->SetFontSize(fontSize);
	model->SetColor(color);
	model->SetMoveSpeed(moveSpeed);
	model->SetInitialLifeTime(lifetime);
	model->SetVisible(true);
	model->SetState(EEffectTextState::ACTIVE);
}

void PlayerActorController::UpdateMoveSpeed(int32_t level)
{
	Result<const PlayerDataChunk*> result = DataChunkManager::Get().GetDataChunk<PlayerDataChunk>();
	if (!result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_PLAYER_DATA_CHUNK(PlayerActorController)");
		return;
	}

	const PlayerDataChunk* dataChunk = result.GetValue();
	int32_t idx = dataChunk->LevelToIdx.at(level);
	int32_t moveSpeed = dataChunk->DataPacks[idx].Speed;

	_model->SetMoveSpeed(static_cast<float>(moveSpeed));
}