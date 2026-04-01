#include "GameResultActor.h"
#include "GameResultActorController.h"
#include "GameResultModel.h"

GameResultActor::GameResultActor()
{
	AddModel<GameResultModel>(GAME_RESULT_ACTOR_MODEL_ORDER);
	AddController<GameResultActorController>(GAME_RESULT_ACTOR_CONTROLLER_ORDER);

	_isInitialized = true;
}

GameResultActor::~GameResultActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void GameResultActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
