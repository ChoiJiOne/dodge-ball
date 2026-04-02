#pragma once

#include "Actor/IActor.h"

class TabTextActor : public IActor
{
public:
	TabTextActor();
	virtual ~TabTextActor();

	DISALLOW_COPY_AND_ASSIGN(TabTextActor);

	virtual void Release() override;

private:
	static const int32_t TAB_TEXT_ACTOR_MODEL_ORDER = 1;
	static const int32_t TAB_TEXT_ACTOR_CONTROLLER_ORDER = 1;
};
