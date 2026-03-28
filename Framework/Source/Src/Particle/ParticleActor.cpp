#include "Particle/ParticleActor.h"
#include "Particle/ParticleActorController.h"
#include "Particle/ParticleModel.h"

ParticleActor::ParticleActor(
	const glm::vec2& startPosition,
	int32_t count,
	float minSize,
	float maxSize,
	float minSpeed,
	float maxSpeed,
	float lifeTime,
	const glm::vec4& color
)
{
	AddModel<ParticleModel>(PARTICLE_ACTOR_MODEL_ORDER, startPosition, count, minSize, maxSize, minSpeed, maxSpeed, lifeTime, color);
	AddController<ParticleActorController>(PARTICLE_ACTOR_CONTROLLER_ORDER);
}

ParticleActor::ParticleActor(const ParticleActorParam& param)
{
	AddModel<ParticleModel>(PARTICLE_ACTOR_MODEL_ORDER, param.startPosition, param.count, param.minSize, param.maxSize, param.minSpeed, param.maxSpeed, param.lifeTime, param.color);
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