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
		LOG_E("FAILED_TO_GET_PARTICLE_MODEL"); // 일단 로그를 찍어보자.
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
	// Something...
}