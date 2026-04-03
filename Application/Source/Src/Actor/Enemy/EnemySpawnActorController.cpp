#include "PlayerDataChunk.h"

#include "Actor/IActor.h"
#include "EnemyDataChunk.h"
#include "Macro/Macro.h"
#include "Manager/ConfigManager.h"
#include "Manager/ContextManager.h"
#include "Manager/DataChunkManager.h"
#include "Manager/SceneManager.h"
#include "Scene/IScene.h"
#include "Utils/LogUtils.h"
#include "Utils/MathUtils.h"

#include "Actor/Enemy/EnemyActor.h"
#include "Actor/Enemy/EnemyModel.h"
#include "Actor/Enemy/EnemySpawnActorController.h"
#include "Actor/Enemy/EnemySpawnActorModel.h"
#include "App/AppDef.h"
#include "Config/GameConfig.h"
#include "Context/PlayerContext.h"

void EnemySpawnActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

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

	if (Result<void> result = InitializeFromConfig(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_FROM_CONFIG(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeSpawnTime(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_FROM_SPAWN_TIME(msg:{0})", result.GetError().GetMessage());
		return;
	}
}

void EnemySpawnActorController::OnRelease()
{
	Event<int32_t>& levelUpEvent = _context->GetLevelUpEvent();
	levelUpEvent.UnregisterCallback(NAME_OF(EnemySpawnActorController));

	_model = nullptr;
}

void EnemySpawnActorController::OnTick(float deltaSeconds)
{
	_timeSinceLastSpawn += deltaSeconds;
	if (_timeSinceLastSpawn < _spawnTime)
	{
		return;
	}

	SpawnEnemyActor();
}


Result<void> EnemySpawnActorController::InitializeContext()
{
	ContextManager& contextMgr = ContextManager::Get();
	Result<PlayerContext*> result = contextMgr.GetContext<PlayerContext>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_context = result.GetValue();

	Event<int32_t>& levelUpEvent = _context->GetLevelUpEvent();
	levelUpEvent.RegisterCallback(
		NAME_OF(EnemySpawnActorController),
		[this](int32_t level) { UpdateEnemySpawnTimeByPlayerLevel(level); }
	);

	return Result<void>::Success();
}

Result<void> EnemySpawnActorController::InitializeModel()
{
	Result<EnemySpawnActorModel*> result = _ownerActor->GetModel<EnemySpawnActorModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_model = result.GetValue();
	return Result<void>::Success();
}

Result<void> EnemySpawnActorController::InitializeFromConfig()
{
	ConfigManager& configMgr = ConfigManager::Get();
	Result<const GameConfig*> result = configMgr.GetConfig<GameConfig>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const GameConfig* config = result.GetValue();
	_spawnRangeMinX = static_cast<float>(config->GetSpawnRangeMinX());
	_spawnRangeMaxX = static_cast<float>(config->GetSpawnRangeMaxX());
	_spawnRangeY = static_cast<float>(config->GetSpawnRangeY());
	_enemySize = config->GetEnemySize();

	return Result<void>::Success();
}


Result<void> EnemySpawnActorController::InitializeSpawnTime()
{
	Result<const PlayerDataChunk*> result = DataChunkManager::Get().GetDataChunk<PlayerDataChunk>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const PlayerDataChunk* dataChunk = result.GetValue();
	int32_t idx = dataChunk->LevelToIdx.at(_context->GetLevel());
	_spawnTime = dataChunk->DataPacks[idx].EnemySpawnTime;

	return Result<void>::Success();
}

Result<const EnemyDataPack*> EnemySpawnActorController::GetRandomEnemyDataPack() const
{
	DataChunkManager& dataChunkMgr = DataChunkManager::Get();
	Result<const EnemyDataChunk*> result = dataChunkMgr.GetDataChunk<EnemyDataChunk>();
	if (!result.IsSuccess() || result.GetValue()->DataPacks.empty())
	{
		return Result<const EnemyDataPack*>::Fail(MAKE_ERROR(
			EErrorCode::INVALID_DATA_PACK,
			std::format("INVALID_DATA_PACK(name:{0})", NAME_OF(EnemyDataChunk))
		));
	}

	const auto& dataPacks = result.GetValue()->DataPacks;
	int32_t randomIdx = MathUtils::GenerateRandomInt(0, dataPacks.size() - 1);
	return Result<const EnemyDataPack*>::Success(&dataPacks[randomIdx]);
}

EnemyModel* EnemySpawnActorController::FindAvailableEnemyModel() const
{
	int32_t spawnEnemyActorModelIndex = DEF::INVALID_IDX;
	for (std::size_t index = 0; index < _enemyModelPool.size(); ++index)
	{
		EEnemyState state = _enemyModelPool[index]->GetState();
		if (state == EEnemyState::DEAD)
		{
			spawnEnemyActorModelIndex = index;
			break;
		}
	}

	return (spawnEnemyActorModelIndex != DEF::INVALID_IDX)
		? _enemyModelPool[spawnEnemyActorModelIndex]
		: nullptr;
}

EnemyModel* EnemySpawnActorController::CreateAndRegisterEnemy()
{
	SceneManager& sceneMgr = SceneManager::Get();
	IScene* currentScene = sceneMgr.GetCurrentScene();

	std::string key = std::format("EnemyActor_{0}", _spawnedCount);
	Result<EnemyActor*> createResult = currentScene->CreateAndAddActor<EnemyActor>(key, DEF::SCENE_ENEMY_ACTOR_ORDER);
	if (!createResult.IsSuccess())
	{
		LOG_E("FAILED_TO_CREATE_AND_ADD_ENEMY_ACTOR:(key:{0})", key);
		return nullptr;
	}

	EnemyActor* enemyActor = createResult.GetValue();
	Result<EnemyModel*> getResult = enemyActor->GetModel<EnemyModel>();
	if (!getResult.IsSuccess())
	{
		LOG_E("FAILED_TO_ENEMY_MODEL_FROM_ENEMY_ACTOR:(key:{0})", key);
		return nullptr;
	}

	EnemyModel* enemyModel = getResult.GetValue();

	_enemyActorKeyMap.emplace(key, _spawnedCount++);
	_enemyModelPool.push_back(enemyModel);

	return enemyModel;
}

void EnemySpawnActorController::SpawnEnemyActor()
{
 	SceneManager& sceneMgr = SceneManager::Get();
	IScene* currentScene = sceneMgr.GetCurrentScene();

	EnemyModel* enemyModel = FindAvailableEnemyModel();
	if (enemyModel == nullptr)
	{
		enemyModel = CreateAndRegisterEnemy();
		if (enemyModel == nullptr)
		{
			LOG_E("FAILED_TO_CREATE_ENEMY_ACTOR");
			return;
		}
	}

	SetEnemyModel(enemyModel);
	_timeSinceLastSpawn = 0.0f;
}

void EnemySpawnActorController::SetEnemyModel(EnemyModel* model)
{
	Result<const EnemyDataPack*> result = GetRandomEnemyDataPack();
	if (!result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_RANDOM_ENEMY_DATA_PACK");
		return;
	}

	const EnemyDataPack* dataPack = result.GetValue();

	glm::vec2 position = glm::vec2(MathUtils::GenerateRandomFloat(_spawnRangeMinX, _spawnRangeMaxX), _spawnRangeY);
	float size = static_cast<float>(_enemySize);
	glm::vec4 color = ConvertColorFromColorData(dataPack->Color);

	model->SetPosition(position);
	model->SetSize(size);
	model->SetColor(color);
	model->SetMoveSpeed(static_cast<float>(dataPack->MoveSpeed));
	model->SetRotationSpeed(static_cast<float>(dataPack->RotationSpeed));
	model->SetMoveDirection(glm::vec2(0.0f, 1.0f));
	model->SetCollidable(true);
	model->SetState(EEnemyState::MOVE);
}


void EnemySpawnActorController::UpdateEnemySpawnTimeByPlayerLevel(int32_t playerLevel)
{
	Result<const PlayerDataChunk*> result = DataChunkManager::Get().GetDataChunk<PlayerDataChunk>();
	if (!result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_PLAYER_DATA_CHUNK(EnemySpawnActorController)");
		return;
	}

	const PlayerDataChunk* dataChunk = result.GetValue();
	int32_t idx = dataChunk->LevelToIdx.at(playerLevel);
	_spawnTime = dataChunk->DataPacks[idx].EnemySpawnTime;
}

glm::vec4 EnemySpawnActorController::ConvertColorFromColorData(const std::vector<float>& colorData) const
{
	glm::vec4 color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	std::size_t size = glm::min<std::size_t>(colorData.size(), 3); // R, G, B
	for (std::size_t idx = 0; idx < size; ++idx)
	{
		color[idx] = colorData[idx];
	}

	return color;
}