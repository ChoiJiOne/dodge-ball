#pragma once

#include "Actor/IActor.h"

class EffectTextActor : public IActor
{
public:
	EffectTextActor();
	virtual ~EffectTextActor();

	DISALLOW_COPY_AND_ASSIGN(EffectTextActor);

	virtual void Release() override;

private:
	static const int32_t EFFECT_TEXT_ACTOR_MODEL_ORDER      = 1;
	static const int32_t EFFECT_TEXT_ACTOR_CONTROLLER_ORDER = 1;
};
