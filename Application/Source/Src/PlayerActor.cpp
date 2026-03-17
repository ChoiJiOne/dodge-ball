#include "BallModel.h"
#include "MoveBoundModel.h"
#include "PlayerActor.h"
#include "PlayerActorController.h"

PlayerActor::PlayerActor()
{
	AddModel<MoveBoundModel>(1);
	AddModel<BallModel>(2);
	AddController<PlayerActorController>(1);

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
