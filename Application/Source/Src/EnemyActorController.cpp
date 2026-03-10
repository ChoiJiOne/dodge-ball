#include "Actor/IActor.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

#include "BallModel.h"
#include "EnemyActorController.h"

void EnemyActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	Result<BallModel*> result = _ownerActor->GetModel<BallModel>();
	if (!result.IsSuccess()) // Get이 실패할 수 있을까...?
	{
		LOG_E("FAILED_TO_GET_ENEMY_BALL_MODEL"); // 일단 로그를 찍어보자.
		return;
	}
	else
	{
		_model = result.GetValue();
	}

	_model->SetPosition(glm::vec2(300.0f, 0.0f));
	_model->SetColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	_model->SetRadius(10.0f);
	_model->SetMoveSpeed(100.0f);
	_model->SetMoveDirection(glm::vec2(0.0f, 1.0f));
}

void EnemyActorController::OnRelease()
{
	_model = nullptr;
}

void EnemyActorController::OnTick(float deltaSeconds)
{
	Move(deltaSeconds);
}

void EnemyActorController::Move(float deltaSeconds)
{
	glm::vec2 position = _model->GetPosition();
	glm::vec2 direction = _model->GetMoveDirection();
	float speed = _model->GetMoveSpeed();

	position += direction * deltaSeconds * speed;

	_model->SetPosition(position);
}