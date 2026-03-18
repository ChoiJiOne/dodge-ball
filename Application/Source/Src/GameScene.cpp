#include "AppDef.h"
#include "EnemyActor.h"
#include "EnemySpawnActor.h"
#include "GameScene.h"
#include "PlayerActor.h"

Result<void> GameScene::OnEnter()
{
	if (Result<PlayerActor*> result = CreateAndAddActor<PlayerActor>("Player", DEF::SCENE_PLAYER_ACTOR_ORDER); !result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	if (Result<EnemySpawnActor*> result = CreateAndAddActor<EnemySpawnActor>("EnemySpawnActor", DEF::SCENE_ENEMY_SPAWN_ACTOR_ORDER); !result.IsSuccess())
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