#include "Particle/ParticleActor.h"
#include "Particle/ParticleActorController.h"
#include "Particle/ParticleModel.h"

ParticleActor::ParticleActor()
{
	AddModel<ParticleModel>(PARTICLE_ACTOR_MODEL_ORDER);
	AddController<ParticleActorController>(PARTICLE_ACTOR_CONTROLLER_ORDER);
}

ParticleActor::~ParticleActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void ParticleActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}