#pragma once

#include "Actor/IActor.h"

class GameOverActor : public IActor
{
public:
	GameOverActor();
	virtual ~GameOverActor();

	DISALLOW_COPY_AND_ASSIGN(GameOverActor);

	virtual void Release() override;

private:
	static const int32_t GAME_OVER_ACTOR_MODEL_ORDER = 1;
	static const  int32_t GAME_OVER_ACTOR_CONTROLLER_ORDER = 1;
};