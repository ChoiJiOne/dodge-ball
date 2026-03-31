#include "Text/TextModel.h"

#include "AppDef.h"
#include "GameOverActor.h"
#include "GameOverActorController.h"

GameOverActor::GameOverActor()
{
	// TODO: Remove Hard coding.
	AddModel<TextModel>(GAME_OVER_ACTOR_MODEL_ORDER, "GAME OVER", glm::vec2(300.0f, 200.0f), glm::vec4(1.0f, 0.5f, 0.1f, 1.0f), 60.0f);
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
