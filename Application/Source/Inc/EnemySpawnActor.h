#pragma once

#include "Actor/IActor.h"

class EnemySpawnActor : public IActor
{
public:
	EnemySpawnActor();
	virtual ~EnemySpawnActor();

	virtual void Release() override;
};