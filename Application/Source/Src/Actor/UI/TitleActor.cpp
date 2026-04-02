#include "Text/TextModel.h"

#include "Actor/UI/TitleActor.h"
#include "Actor/UI/TitleActorController.h"
#include "Actor/UI/TitleTextModel.h"

TitleActor::TitleActor()
{
	AddModel<TitleTextModel>(TITLE_ACTOR_TITLE_MODEL_ORDER);
	AddModel<TextModel>(TITLE_ACTOR_HINT_MODEL_ORDER);
	AddController<TitleActorController>(TITLE_ACTOR_CONTROLLER_ORDER);

	_isInitialized = true;
}

TitleActor::~TitleActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void TitleActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
