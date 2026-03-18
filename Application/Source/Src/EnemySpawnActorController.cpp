#include "AppDef.h"
#include "BallDataChunk.h"

#include "Actor/IActor.h"
#include "Utils/LogUtils.h"
#include "Utils/MathUtils.h"
#include "Macro/Macro.h"
#include "Manager/ConfigManager.h"
#include "Manager/DataChunkManager.h"
#include "Manager/SceneManager.h"
#include "Scene/IScene.h"

#include "BallModel.h"
#include "GameConfig.h"
#include "EnemyActor.h"
#include "EnemySpawnActorController.h"
#include "EnemySpawnActorModel.h"

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
		if (Result<BallModel*> result = enemyActor->GetModel<BallModel>(); !result.IsSuccess())
		{
			LOG_E("FAILED_TO_GET_BALL_MODEL_FROM_ENEMY_ACTOR:(key:{0})", key);
			return;
		}
		else
		{
			BallModel* model = result.GetValue();
			SetBallModel(model);
		}

		_enemyActorKeyMap.emplace(key, _spawnedCount);
		_spawnedCount++;
		_timeSinceLastSpawn = 0.0f;
	}
}

void EnemySpawnActorController::SetBallModel(BallModel* model)
{
	if (Result<const BallDataPack*> result = GetRandomBallDataPack(); result.IsSuccess())
	{
		const BallDataPack* dataPack = result.GetValue();

		glm::vec2 position = glm::vec2(MathUtils::GenerateRandomFloat(_spawnRangeMinX, _spawnRangeMaxX), _spawnRangeY);
		glm::vec4 color = ConvertColorFromColorData(dataPack->Color);

		model->SetPosition(position);
		model->SetColor(color);
		model->SetRadius(static_cast<float>(dataPack->Size));
		model->SetMoveSpeed(static_cast<float>(dataPack->Speed));
		model->SetMoveDirection(glm::vec2(0.0f, 1.0f));
	}
}

Result<const BallDataPack*> EnemySpawnActorController::GetRandomBallDataPack() const
{
	DataChunkManager& dataChunkMgr = DataChunkManager::Get();
	Result<const BallDataChunk*> result = dataChunkMgr.GetDataChunk<BallDataChunk>();
	if (!result.IsSuccess() || result.GetValue()->DataPacks.empty())
	{
		return Result<const BallDataPack*>::Fail(MAKE_ERROR(
			EErrorCode::FAILED_TO_GET_BALL_DATA_PACK_OR_EMPTY, 
			std::format("FAILED_TO_GET_BALL_DATA_PACK_OR_EMPTY(name:{0})", NAME_OF(BallDataChunk))
		));
	}

	const auto& dataPacks = result.GetValue()->DataPacks;
	int32_t randomIdx = MathUtils::GenerateRandomInt(0, dataPacks.size() - 1);
	return Result<const BallDataPack*>::Success(&dataPacks[randomIdx]);
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