#include "EnemySpawnActor.h"
#include "EnemySpawnActorController.h"
#include "EnemySpawnActorModel.h"

EnemySpawnActor::EnemySpawnActor()
{
	AddModel<EnemySpawnActorModel>();
	AddController<EnemySpawnActorController>();

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