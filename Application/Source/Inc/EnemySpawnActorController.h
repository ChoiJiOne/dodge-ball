#pragma once

#include <glm/glm.hpp>

#include "Actor/IActorController.h"

class EnemyDataPack;
class EnemyModel;
class EnemySpawnActorModel;

class EnemySpawnActorController : public IActorController
{
public:
	EnemySpawnActorController() = default;
	virtual ~EnemySpawnActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(EnemySpawnActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override { /* EMPTY */ }

private:
	void SpawnEnemyActor();
	void SetEnemyModel(EnemyModel* model);

	Result<const EnemyDataPack*> GetRandomEnemyDataPack() const;
	glm::vec4 ConvertColorFromColorData(const std::vector<float>& colorData) const;

private:
	EnemySpawnActorModel* _model = nullptr;

	std::map<std::string, int32_t> _enemyActorKeyMap;
	std::vector<EnemyModel*> _enemyModelPool;

	float _spawnTime = 1.0f;
	float _timeSinceLastSpawn = 0.0f;
	int32_t _spawnedCount = 0;

	float _spawnRangeMinX = 0.0f;
	float _spawnRangeMaxX = 0.0f;
	float _spawnRangeY = 0.0f;
	int32_t _enemySize = 0;
};