#include "AppDef.h"
#include "BallModel.h"
#include "MoveBoundModel.h"
#include "PlayerActor.h"
#include "PlayerActorController.h"

PlayerActor::PlayerActor()
{
	AddModel<MoveBoundModel>(DEF::PLAYER_ACTOR_MOVE_BOUND_MODEL_ORDER);
	AddModel<BallModel>(DEF::PLAYER_ACTOR_MODEL_ORDER);
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
