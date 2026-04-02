#include "Text/TextModel.h"

#include "Actor/UI/GameResultActor.h"
#include "Actor/UI/GameResultActorController.h"
#include "Actor/UI/GameResultModel.h"

GameResultActor::GameResultActor()
{
	AddModel<GameResultModel>(GAME_RESULT_ACTOR_MODEL_ORDER);
	AddModel<TextModel>(GAME_RESULT_ACTOR_HINT_MODEL_ORDER);
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
