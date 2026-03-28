#pragma once

#include "Actor/IActorController.h"

class ParticleModel;

class ParticleActorController : public IActorController
{
public:
	ParticleActorController() = default;
	virtual ~ParticleActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(ParticleActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override { /* EMPTY */ }

private:
	ParticleModel* _model = nullptr;
};