#include "Actor/IActor.h"
#include "Manager/InputManager.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

#include "BallModel.h"
#include "PlayerBallController.h"

void PlayerBallController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	_inputMgr = InputManager::GetPtr();

	Result<BallModel*> result = _ownerActor->GetModel<BallModel>();
	if (!result.IsSuccess()) // Get이 실패할 수 있을까...?
		LOG_E("FAILED_TO_GET_PLAYER_BALL_MODEL"); // 일단 로그를 찍어보자.
	else
		_model = result.GetValue();
}

void PlayerBallController::OnRelease()
{
	_inputMgr = nullptr;
	_model = nullptr;
}

void PlayerBallController::OnTick(float deltaSeconds)
{
	glm::vec2 position = _model->GetPosition();
	if (_inputMgr->GetKeyPress(EKey::LEFT) == EPress::HELD)
	{
		position.x -= deltaSeconds * _moveSpeed;
	}

	if (_inputMgr->GetKeyPress(EKey::RIGHT) == EPress::HELD)
	{
		position.x += deltaSeconds * _moveSpeed;
	}

	_model->SetPosition(position);
}
