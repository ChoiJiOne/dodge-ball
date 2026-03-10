#pragma once

#include <glm/glm.hpp>

#include "Actor/IActorController.h"

class BallModel;

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

private:
	BallModel* _model = nullptr;
};