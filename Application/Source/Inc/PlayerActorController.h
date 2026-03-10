#pragma once

#include <glm/glm.hpp>

#include "Actor/IActorController.h"
#include "Manager/InputManager.h"

class InputManager;
class BallModel;

class PlayerActorController : public IActorController
{
public:
	PlayerActorController() = default;
	virtual ~PlayerActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(PlayerActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;

private:
	void UpdateMoveDirection();
	void Move(float deltaSeconds);
	void UpdateDirectionByBounds();
	
private:
	InputManager* _inputMgr = nullptr;
	BallModel* _model = nullptr;

	float _boundDistance = 0.0f;
	glm::vec2 _leftBoundPosition;
	glm::vec2 _rightBoundPosition;
};