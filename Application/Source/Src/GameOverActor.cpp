#include "Text/TextModel.h"

#include "AppDef.h"
#include "GameOverActor.h"
#include "GameOverActorController.h"

GameOverActor::GameOverActor()
{
	AddModel<TextModel>(GAME_OVER_ACTOR_MODEL_ORDER);
	AddController<GameOverActorController>(GAME_OVER_ACTOR_CONTROLLER_ORDER);

	_isInitialized = true;
}

GameOverActor::~GameOverActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void GameOverActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
