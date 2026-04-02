#pragma once

#include "Actor/IActor.h"

class TitleActor : public IActor
{
public:
	TitleActor();
	virtual ~TitleActor();

	DISALLOW_COPY_AND_ASSIGN(TitleActor);

	virtual void Release() override;

private:
	static const int32_t TITLE_ACTOR_TITLE_MODEL_ORDER = 1;
	static const int32_t TITLE_ACTOR_HINT_MODEL_ORDER = 2;
	static const int32_t TITLE_ACTOR_CONTROLLER_ORDER = 1;
};
