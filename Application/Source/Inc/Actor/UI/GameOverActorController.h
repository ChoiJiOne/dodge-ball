#pragma once

#include "Actor/IActorController.h"

class InputManager;
class SceneManager;
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
	SceneManager* _sceneMgr = nullptr;
	PlayerContext* _context = nullptr;
	float _inputDelay = 0.0f;
	float _elapsedTime = 0.0f;
};