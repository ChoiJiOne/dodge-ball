#pragma once

#include "Actor/IActorController.h"
#include "Manager/InputManager.h"

class InputManager;
class PlayerBallModel;

class PlayerBallController : public IActorController
{
public:
	PlayerBallController() = default;
	virtual ~PlayerBallController() = default;

	DISALLOW_COPY_AND_ASSIGN(PlayerBallController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;

private:
	InputManager* _inputMgr = nullptr;
	PlayerBallModel* _model = nullptr;
};