#pragma once

#include <functional>

#include <glm/glm.hpp>

#include "Actor/IActorController.h"

#include "EnemyModel.h"

class EnemyActorController : public IActorController
{
public:
	EnemyActorController() = default;
	virtual ~EnemyActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(EnemyActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override { /* EMPTY */ }

private:
	void Move(float deltaSeconds);
	void Rotate(float deltaSeconds);
	void FadeOut(float deltaSeconds);

	void OnNoneStateTick(float deltaSeconds);
	void OnMoveStateTick(float deltaSeconds);
	void OnFadeOutStateTick(float deltaSeconds);
	void OnDeadStateTick(float deltaSeconds);

private:
	EnemyModel* _model = nullptr;
	float _spawnRangeY = 0.0f;
	float _deadZoneY = 0.0f;
	float _fadeOutTime = 0.0f;
	float _elapsedFadeOutTime = 0.0f;
	std::map<EEnemyState, std::function<void(float)>> _onStateTickMap;
};