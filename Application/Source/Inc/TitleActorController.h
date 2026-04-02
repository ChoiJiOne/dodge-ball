#pragma once

#include "Actor/IActorController.h"

class TitleTextModel;
class TextModel;
class InputManager;
class SceneManager;

class TitleActorController : public IActorController
{
public:
	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override {}

private:
	Result<void> InitializeTitleModel();
	Result<void> InitializeHintModel();
	Result<void> InitializeModelsFromConfig();

private:
	TitleTextModel* _titleModel = nullptr;
	TextModel* _hintModel = nullptr;
	InputManager* _inputMgr = nullptr;
	SceneManager* _sceneMgr = nullptr;

	float _blinkTime = 0.0f;
	float _blinkSpeed = 3.0f;
};
