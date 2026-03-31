#include <format>

#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Manager/ConfigManager.h"
#include "Manager/ContextManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Particle/ParticleActor.h"
#include "Scene/IScene.h"
#include "Utils/LogUtils.h"

#include "AppDef.h"
#include "GameConfig.h"
#include "MoveBoundModel.h"
#include "PlayerActorController.h"
#include "PlayerContext.h"
#include "PlayerModel.h"
#include "TabTextActor.h"
#include "TabTextModel.h"

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

	if (Result<void> result = InitializeMoveBoundModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MOVE_BOUND_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}
}

void PlayerActorController::OnRelease()
{
	_inputMgr = nullptr;
	_model = nullptr;
}

void PlayerActorController::OnTick(float deltaSeconds)
{
	if (_model->IsDead())
	{
		return;
	}

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
	_moveRangeMinX    = static_cast<float>(config->GetPlayerMoveRangeMinX());
	_moveRangeMaxX    = static_cast<float>(config->GetPlayerMoveRangeMaxX());
	_tabTextMoveSpeed = config->GetTabTextMoveSpeed();
	_tabTextLifeTime  = config->GetTabTextLifeTime();
	_tabTextFontSize  = config->GetTabTextFontSize();
	_tabTextOffsetY   = config->GetTabTextOffsetY();
	float moveRangeX = (_moveRangeMinX + _moveRangeMaxX) * 0.5f;
	float moveRangeY = static_cast<float>(config->GetPlayerMoveRangeY());
	bool isStartMovePositive = config->IsPlayerStartMovePositive();

	glm::vec2 position(moveRangeX, moveRangeY);
	glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f); // TODO: Remove hard coding
	float radius = config->GetPlayerRadius();
	float moveSpeed = 500.0f;  // TODO: Remove hard coding
	glm::vec2 moveDirection(isStartMovePositive ? +1.0f : -1.0f, 0.0f);

	_model->SetPosition(position);
	_model->SetColor(color);
	_model->SetRadius(radius);
	_model->SetMoveSpeed(moveSpeed);
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
	moveBoundModel->SetColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f)); // TODO: Remove hard coding

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
	TabTextModel* model = FindAvailableTabTextModel();
	if (model == nullptr)
	{
		model = CreateAndRegisterTabText();
		if (model == nullptr)
		{
			LOG_E("FAILED_TO_CREATE_TAB_TEXT_ACTOR");
			return;
		}
	}

	ActivateTabTextModel(model);
}

TabTextModel* PlayerActorController::FindAvailableTabTextModel() const
{
	for (TabTextModel* model : _tabTextModelPool)
	{
		if (model->GetState() == ETabTextState::DEAD)
		{
			return model;
		}
	}
	return nullptr;
}

TabTextModel* PlayerActorController::CreateAndRegisterTabText()
{
	SceneManager& sceneMgr = SceneManager::Get();
	IScene* currentScene = sceneMgr.GetCurrentScene();

	std::string key = std::format("{0}_{1}", DEF::TAB_TEXT_ACTOR_KEY_PREFIX, _tabTextModelPool.size());
	Result<TabTextActor*> createResult = currentScene->CreateAndAddActor<TabTextActor>(key, DEF::SCENE_TAB_TEXT_ACTOR_ORDER);
	if (!createResult.IsSuccess())
	{
		LOG_E("FAILED_TO_CREATE_AND_ADD_TAB_TEXT_ACTOR(key:{0})", key);
		return nullptr;
	}

	TabTextActor* actor = createResult.GetValue();
	Result<TabTextModel*> getResult = actor->GetModel<TabTextModel>();
	if (!getResult.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_TAB_TEXT_MODEL(key:{0})", key);
		return nullptr;
	}

	TabTextModel* model = getResult.GetValue();
	_tabTextModelPool.push_back(model);

	return model;
}

void PlayerActorController::ActivateTabTextModel(TabTextModel* model)
{
	glm::vec2 position = _model->GetPosition();
	position.y -= _tabTextOffsetY;

	model->SetText("TAB!");
	model->SetPosition(position);
	model->SetFontSize(_tabTextFontSize);
	model->SetColor(_model->GetColor());
	model->SetMoveSpeed(_tabTextMoveSpeed);
	model->SetInitialLifeTime(_tabTextLifeTime);
	model->SetVisible(true);
	model->SetState(ETabTextState::ACTIVE);
}