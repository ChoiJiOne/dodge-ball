#include "EnemyActor.h"
#include "EnemySpawnActor.h"
#include "GameScene.h"
#include "PlayerActor.h"

Result<void> GameScene::OnEnter()
{
	if (Result<PlayerActor*> result = CreateAndAddActor<PlayerActor>("Player", 1); !result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	if (Result<EnemySpawnActor*> result = CreateAndAddActor<EnemySpawnActor>("EnemySpawnActor", 3); !result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	return Result<void>::Success();
}

Result<void> GameScene::OnExit()
{
	ClearActorMap();

	return Result<void>::Success();
}