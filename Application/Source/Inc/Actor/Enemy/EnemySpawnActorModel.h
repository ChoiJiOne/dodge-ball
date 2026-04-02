#pragma once

#include "Actor/IActorModel.h"

class EnemySpawnActorModel : public IActorModel
{
public:
	EnemySpawnActorModel() = default;
	virtual ~EnemySpawnActorModel() = default;

	DISALLOW_COPY_AND_ASSIGN(EnemySpawnActorModel);
};