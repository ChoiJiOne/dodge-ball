#include "BallModel.h"
#include "MoveBoundModel.h"
#include "PlayerActor.h"
#include "PlayerActorController.h"

PlayerActor::PlayerActor()
{
	AddModel<BallModel>();
	AddModel<MoveBoundModel>();
	AddController<PlayerActorController>();

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
