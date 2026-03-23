#include "AppDef.h"
#include "EnemyDataChunk.h"

#include "Actor/IActor.h"
#include "Utils/LogUtils.h"
#include "Utils/MathUtils.h"
#include "Macro/Macro.h"
#include "Manager/ConfigManager.h"
#include "Manager/DataChunkManager.h"
#include "Manager/SceneManager.h"
#include "Scene/IScene.h"

#include "GameConfig.h"
#include "EnemyActor.h"
#include "EnemySpawnActorController.h"
#include "EnemySpawnActorModel.h"
#include "EnemyModel.h"

void EnemySpawnActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	if (Result<EnemySpawnActorModel*> result = _ownerActor->GetModel<EnemySpawnActorModel>(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_ENEMY_SPAWN_MODEL");
		return;
	}
	else
	{
		_model = result.GetValue();
	}

	ConfigManager& configMgr = ConfigManager::Get();
	if (Result<const GameConfig*> result = configMgr.GetConfig<GameConfig>(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG");
		return;
	}
	else
	{
		const GameConfig* config = result.GetValue();
		_spawnRangeMinX = static_cast<float>(config->GetSpawnRangeMinX());
		_spawnRangeMaxX = static_cast<float>(config->GetSpawnRangeMaxX());
		_spawnRangeY = static_cast<float>(config->GetSpawnRangeY());
		_enemySize = config->GetEnemySize();
	}
}

void EnemySpawnActorController::OnRelease()
{
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

void EnemySpawnActorController::SpawnEnemyActor()
{
 	SceneManager& sceneMgr = SceneManager::Get();
	IScene* currentScene = sceneMgr.GetCurrentScene();

	std::string key = std::format("EnemyActor_{0}", _spawnedCount);
	if (Result<EnemyActor*> result = currentScene->CreateAndAddActor<EnemyActor>(key, DEF::SCENE_ENEMY_ACTOR_ORDER); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_CREATE_AND_ADD_ENEMY_ACTOR:(key:{0})", key);
		return;
	}
	else
	{
		EnemyActor* enemyActor = result.GetValue();
		if (Result<EnemyModel*> result = enemyActor->GetModel<EnemyModel>(); !result.IsSuccess())
		{
			LOG_E("FAILED_TO_ENEMY_MODEL_FROM_ENEMY_ACTOR:(key:{0})", key);
			return;
		}
		else
		{
			EnemyModel* model = result.GetValue();
			SetEnemyModel(model);
		}

		_enemyActorKeyMap.emplace(key, _spawnedCount);
		_spawnedCount++;
		_timeSinceLastSpawn = 0.0f;
	}
}

void EnemySpawnActorController::SetEnemyModel(EnemyModel* model)
{
	if (Result<const EnemyDataPack*> result = GetRandomEnemyDataPack(); result.IsSuccess())
	{
		const EnemyDataPack* dataPack = result.GetValue();

		glm::vec2 position = glm::vec2(MathUtils::GenerateRandomFloat(_spawnRangeMinX, _spawnRangeMaxX), _spawnRangeY);
		glm::vec2 size = glm::vec2(static_cast<float>(_enemySize), static_cast<float>(_enemySize));
		glm::vec4 color = ConvertColorFromColorData(dataPack->Color);

		model->SetPosition(position);
		model->SetSize(size);
		model->SetColor(color);
		model->SetMoveSpeed(static_cast<float>(dataPack->MoveSpeed));
		model->SetRotationSpeed(static_cast<float>(dataPack->RotationSpeed));
		model->SetMoveDirection(glm::vec2(0.0f, 1.0f));
	}
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