#include "Particle/ParticleActor.h"
#include "Particle/ParticleActorController.h"
#include "Particle/ParticleModel.h"
#include "Utils/LogUtils.h"
#include "Utils/MathUtils.h"
#include "Macro/Macro.h"

void ParticleActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	if (Result<ParticleModel*> result = _ownerActor->GetModel<ParticleModel>(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_PARTICLE_MODEL");
		return;
	}
	else
	{
		_model = result.GetValue();
	}
}

void ParticleActorController::OnRelease()
{
	_model = nullptr;
}

void ParticleActorController::OnTick(float deltaSeconds)
{
	std::vector<Particle>& particles = _model->GetParticles();
	for (Particle& particle : particles)
	{
		OnTick(particle, deltaSeconds);
	}
}

void ParticleActorController::OnTick(Particle& particle, float deltaSeconds)
{
	if (!particle.IsAlive())
	{
		return;
	}

	glm::vec2 position = particle.GetPosition();
	const glm::vec2& direction = particle.GetDirection();
	float speed = particle.GetSpeed();
	position += direction * speed * deltaSeconds;
	particle.SetPosition(position);

	float initialLifeTime = particle.GetInitialLifeTime();
	float remainingLifeTime = particle.GetRemainingLifeTime();
	remainingLifeTime -= deltaSeconds;
	remainingLifeTime = glm::clamp(remainingLifeTime, 0.0f, initialLifeTime);
	particle.SetRemainingLifeTime(remainingLifeTime);

	glm::vec4 color = particle.GetColor();
	color.a = remainingLifeTime / initialLifeTime;
	particle.SetColor(color);
}