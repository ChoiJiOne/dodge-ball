#pragma once

#include "Actor/IActor.h"

class PlayerActor : public IActor
{
public:
	PlayerActor();
	virtual ~PlayerActor();

	DISALLOW_COPY_AND_ASSIGN(PlayerActor);

	virtual void Release() override;
};