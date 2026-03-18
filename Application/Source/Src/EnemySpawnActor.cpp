#include "AppDef.h"
#include "EnemySpawnActor.h"
#include "EnemySpawnActorController.h"
#include "EnemySpawnActorModel.h"

EnemySpawnActor::EnemySpawnActor()
{
	AddModel<EnemySpawnActorModel>(DEF::ENEMY_SPAWN_ACTOR_MODEL_ORDER);
	AddController<EnemySpawnActorController>(DEF::ENEMY_SPAWN_ACTOR_CONTROLLER_ORDER);

	_isInitialized = true;
}

EnemySpawnActor::~EnemySpawnActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void EnemySpawnActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}