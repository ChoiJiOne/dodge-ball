#pragma once

#include <glm/glm.hpp>

#include "Actor/IActorController.h"
#include "Manager/InputManager.h"

class InputManager;
class PlayerModel;

class PlayerActorController : public IActorController
{
public:
	PlayerActorController() = default;
	virtual ~PlayerActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(PlayerActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override;

private:
	Result<void> InitializeModel();
	Result<void> InitializeModelFromConfig();
	Result<void> InitializeMoveBoundModel();

	void UpdateMoveDirection();
	void Move(float deltaSeconds);
	void UpdateDirectionByBounds();
	void GenerateParticleEffect();
	
private:
	InputManager* _inputMgr = nullptr;
	PlayerModel* _model = nullptr;

	float _moveRangeMinX = 0.0f;
	float _moveRangeMaxX = 0.0f;
};