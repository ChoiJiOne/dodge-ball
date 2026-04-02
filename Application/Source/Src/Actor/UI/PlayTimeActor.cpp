#include "Text/TextModel.h"

#include "Actor/UI/PlayTimeActor.h"
#include "Actor/UI/PlayTimeActorController.h"

PlayTimeActor::PlayTimeActor()
{
	AddModel<TextModel>(PLAY_TIME_ACTOR_MODEL_ORDER);
	AddController<PlayTimeActorController>(PLAY_TIME_ACTOR_CONTROLLER_ORDER);

	_isInitialized = true;
}

PlayTimeActor::~PlayTimeActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void PlayTimeActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
