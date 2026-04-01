#include "AppDef.h"
#include "EnemyActor.h"
#include "EnemySpawnActor.h"
#include "GameOverActor.h"
#include "GameScene.h"
#include "PlayerActor.h"
#include "GameResultActor.h"
#include "PlayTimeActor.h"

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

	if (Result<GameOverActor*> result = CreateAndAddActor<GameOverActor>("GameOverActor", DEF::SCENE_GAME_OVER_ACTOR_ORDER); !result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	if (Result<GameResultActor*> result = CreateAndAddActor<GameResultActor>("GameResultActor", DEF::SCENE_GAME_RESULT_ACTOR_ORDER); !result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	if (Result<PlayTimeActor*> result = CreateAndAddActor<PlayTimeActor>("PlayTimeActor", DEF::SCENE_PLAY_TIME_ACTOR_ORDER); !result.IsSuccess())
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