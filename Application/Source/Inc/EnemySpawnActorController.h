#pragma once

#include "Actor/IActorController.h"

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

private:
	EnemySpawnActorModel* _model = nullptr;
	float _spawnTime = 2.0f;
	float _stepTime = 0.0f;
	int32_t _count = 0;
};