#pragma once

#include "Actor/IActor.h"

class EnemySpawnActor : public IActor
{
public:
	EnemySpawnActor();
	virtual ~EnemySpawnActor();

	DISALLOW_COPY_AND_ASSIGN(EnemySpawnActor);

	virtual void Release() override;
};