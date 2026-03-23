#pragma once

#include <glm/glm.hpp>

#include "Actor/IActorController.h"

class EnemyModel;

class EnemyActorController : public IActorController
{
public:
	EnemyActorController() = default;
	virtual ~EnemyActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(EnemyActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;

private:
	void Move(float deltaSeconds);
	void Rotate(float deltaSeconds);

private:
	EnemyModel* _model = nullptr;
};