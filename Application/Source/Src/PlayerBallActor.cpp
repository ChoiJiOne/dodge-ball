#include "BallModel.h"
#include "PlayerBallActor.h"
#include "PlayerBallController.h"

PlayerBallActor::PlayerBallActor()
{
	AddModel<BallModel>();
	AddController<PlayerBallController>();

	_isInitialized = true;
}

PlayerBallActor::~PlayerBallActor()
{
	if (_isInitialized)
		Release();
}

void PlayerBallActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}
