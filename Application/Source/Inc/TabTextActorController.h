#pragma once

#include "Actor/IActorController.h"

#include "TabTextModel.h"

class TabTextActorController : public IActorController
{
public:
	TabTextActorController() = default;
	virtual ~TabTextActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(TabTextActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override { /* EMPTY */ }

private:
	Result<void> InitializeModel();

	void OnActiveStateTick(float deltaSeconds);

private:
	TabTextModel* _model = nullptr;
};
