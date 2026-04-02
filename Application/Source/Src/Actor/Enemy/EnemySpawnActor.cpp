#include "Actor/Enemy/EnemySpawnActor.h"
#include "Actor/Enemy/EnemySpawnActorController.h"
#include "Actor/Enemy/EnemySpawnActorModel.h"
#include "App/AppDef.h"

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