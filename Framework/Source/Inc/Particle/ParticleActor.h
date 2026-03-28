#pragma once

#include "Actor/IActor.h"

class ParticleActor : public IActor
{
public:
	ParticleActor();
	virtual ~ParticleActor();

	DISALLOW_COPY_AND_ASSIGN(ParticleActor);

	virtual void Release() override;

private:
	static const int32_t PARTICLE_ACTOR_MODEL_ORDER = 1;
	static const int32_t PARTICLE_ACTOR_CONTROLLER_ORDER = 1;
};