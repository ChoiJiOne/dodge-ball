#pragma once

#include "Actor/IActorController.h"

#include "Actor/UI/EffectTextModel.h"

class EffectTextActorController : public IActorController
{
public:
	EffectTextActorController() = default;
	virtual ~EffectTextActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(EffectTextActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override { /* EMPTY */ }

private:
	Result<void> InitializeModel();

	void OnActiveStateTick(float deltaSeconds);

private:
	EffectTextModel* _model = nullptr;
};
