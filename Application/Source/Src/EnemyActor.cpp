#include "AppDef.h"
#include "EnemyActor.h"
#include "EnemyActorController.h"
#include "EnemyModel.h"

EnemyActor::EnemyActor()
{
	AddModel<EnemyModel>(DEF::ENEMY_AACTOR_MODEL_ORDER);
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
