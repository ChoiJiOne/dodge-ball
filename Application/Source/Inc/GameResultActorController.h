#pragma once

#include "Actor/IActorController.h"

class GameResultModel;
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
	Result<void> InitializeModelFromConfig();
	Result<void> InitializeContext();

private:
	GameResultModel* _model = nullptr;
	PlayerContext* _context = nullptr;
};
