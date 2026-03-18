#include "AppDef.h"
#include "BallModel.h"
#include "EnemyActor.h"
#include "EnemyActorController.h"

EnemyActor::EnemyActor()
{
	AddModel<BallModel>(DEF::ENEMY_ACTOR_BALL_MODEL_ORDER);
	AddController<EnemyActorController>(DEF::ENEMY_ACTOR_CONTROLLER_ORDER);

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
