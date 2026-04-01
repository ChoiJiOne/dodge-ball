#pragma once

#include "Actor/IActorController.h"

class TextModel;
class PlayerContext;

class PlayTimeActorController : public IActorController
{
public:
	PlayTimeActorController() = default;
	virtual ~PlayTimeActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(PlayTimeActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override {}

private:
	Result<void> InitializeModel();
	Result<void> InitializeModelFromConfig();
	Result<void> InitializeContext();

private:
	TextModel* _model = nullptr;
	PlayerContext* _context = nullptr;
};
