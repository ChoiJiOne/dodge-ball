#include "App/AppDef.h"
#include "Actor/UI/TitleActor.h"
#include "Scene/TitleScene.h"

Result<void> TitleScene::OnEnter()
{
	if (Result<TitleActor*> result = CreateAndAddActor<TitleActor>(DEF::TITLE_ACTOR_NAME, DEF::SCENE_TITLE_ACTOR_ORDER); !result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	return Result<void>::Success();
}

Result<void> TitleScene::OnExit()
{
	ClearActorMap();

	return Result<void>::Success();
}
