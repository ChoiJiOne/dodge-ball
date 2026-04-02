#pragma once

#include "Actor/IActor.h"

class EnemyActor : public IActor
{
public:
	EnemyActor();
	virtual ~EnemyActor();

	DISALLOW_COPY_AND_ASSIGN(EnemyActor);

	virtual void Release() override;
};