#include "Actor/UI/TabTextActor.h"
#include "Actor/UI/TabTextActorController.h"
#include "Actor/UI/TabTextModel.h"

TabTextActor::TabTextActor()
{
	AddModel<TabTextModel>(TAB_TEXT_ACTOR_MODEL_ORDER);
	AddController<TabTextActorController>(TAB_TEXT_ACTOR_CONTROLLER_ORDER);
}

TabTextActor::~TabTextActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void TabTextActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
