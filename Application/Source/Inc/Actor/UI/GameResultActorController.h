#pragma once

#include "Actor/IActorController.h"

class GameResultModel;
class TextModel;
class PlayerContext;

class GameResultActorController : public IActorController
{
public:
	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override {}

private:
	Result<void> InitializeModel();
	Result<void> InitializeHintModel();
	Result<void> InitializeModelFromConfig();
	Result<void> InitializeContext();

private:
	GameResultModel* _model = nullptr;
	TextModel* _hintModel = nullptr;
	PlayerContext* _context = nullptr;

	float _blinkTime = 0.0f;
	float _blinkSpeed = 3.0f;
};
