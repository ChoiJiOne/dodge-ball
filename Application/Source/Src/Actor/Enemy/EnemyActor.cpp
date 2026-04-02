#include "Actor/Enemy/EnemyActor.h"
#include "Actor/Enemy/EnemyActorController.h"
#include "Actor/Enemy/EnemyModel.h"
#include "App/AppDef.h"

EnemyActor::EnemyActor()
{
	AddModel<EnemyModel>(DEF::ENEMY_ACTOR_MODEL_ORDER);
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
