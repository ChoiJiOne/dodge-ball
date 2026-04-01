#pragma once

#include "Actor/IActor.h"

class GameResultActor : public IActor
{
public:
	GameResultActor();
	virtual ~GameResultActor();

	DISALLOW_COPY_AND_ASSIGN(GameResultActor);

	virtual void Release() override;

private:
	static const int32_t GAME_RESULT_ACTOR_MODEL_ORDER = 1;
	static const int32_t GAME_RESULT_ACTOR_HINT_MODEL_ORDER = 2;
	static const int32_t GAME_RESULT_ACTOR_CONTROLLER_ORDER = 1;
};
