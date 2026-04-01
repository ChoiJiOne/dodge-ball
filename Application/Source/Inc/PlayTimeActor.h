#pragma once

#include "Actor/IActor.h"

class PlayTimeActor : public IActor
{
public:
	PlayTimeActor();
	virtual ~PlayTimeActor();

	DISALLOW_COPY_AND_ASSIGN(PlayTimeActor);

	virtual void Release() override;

private:
	static const int32_t PLAY_TIME_ACTOR_MODEL_ORDER = 1;
	static const int32_t PLAY_TIME_ACTOR_CONTROLLER_ORDER = 1;
};
