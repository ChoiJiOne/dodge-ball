#include "BallModel.h"
#include "EnemyActor.h"
#include "EnemyActorController.h"

EnemyActor::EnemyActor()
{
	AddModel<BallModel>();
	AddController<EnemyActorController>();

	_isInitialized = true;
}

EnemyActor::~EnemyActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void EnemyActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
