#pragma once

#include "Actor/IActor.h"

class EnemyActor : public IActor
{
public:
	EnemyActor();
	virtual ~EnemyActor();

	virtual void Release() override;
};