#include "Actor/Physics/MoveBoundModel.h"
#include "Actor/Player/PlayerActor.h"
#include "Actor/Player/PlayerActorController.h"
#include "Actor/Player/PlayerModel.h"
#include "App/AppDef.h"

PlayerActor::PlayerActor()
{
	AddModel<MoveBoundModel>(DEF::PLAYER_ACTOR_MOVE_BOUND_MODEL_ORDER);
	AddModel<PlayerModel>(DEF::PLAYER_ACTOR_MODEL_ORDER);
	AddController<PlayerActorController>(DEF::PLAYER_ACTOR_CONTROLLER_ORDER);

	_isInitialized = true;
}

PlayerActor::~PlayerActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void PlayerActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
