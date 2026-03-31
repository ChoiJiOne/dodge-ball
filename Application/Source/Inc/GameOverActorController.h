#pragma once

#include "Actor/IActorController.h"

class InputManager;
class TextModel;
class PlayerContext;

class GameOverActorController : public IActorController
{
public:
	GameOverActorController() = default;
	virtual ~GameOverActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(GameOverActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override { /* EMPTY */ }

private:
	Result<void> InitializeModel();
	Result<void> InitializeModelFromConfig();
	Result<void> InitializeContext();

private:
	TextModel* _model = nullptr;
	InputManager* _inputMgr = nullptr;
	PlayerContext* _context = nullptr;
};