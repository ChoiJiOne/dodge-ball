#include "Actor/UI/EffectTextActor.h"
#include "Actor/UI/EffectTextActorController.h"
#include "Actor/UI/EffectTextModel.h"

EffectTextActor::EffectTextActor()
{
	AddModel<EffectTextModel>(EFFECT_TEXT_ACTOR_MODEL_ORDER);
	AddController<EffectTextActorController>(EFFECT_TEXT_ACTOR_CONTROLLER_ORDER);
}

EffectTextActor::~EffectTextActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void EffectTextActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
