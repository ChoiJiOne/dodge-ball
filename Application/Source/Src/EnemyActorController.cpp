#include "Actor/IActor.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

#include "EnemyModel.h"
#include "EnemyActorController.h"

void EnemyActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	if (Result<EnemyModel*> result = _ownerActor->GetModel<EnemyModel>(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_ENEMY_BALL_MODEL");
		return;
	}
	else
	{
		_model = result.GetValue();
	}
}

void EnemyActorController::OnRelease()
{
	_model = nullptr;
}

void EnemyActorController::OnTick(float deltaSeconds)
{
	Move(deltaSeconds);
	Rotate(deltaSeconds);
}

void EnemyActorController::Move(float deltaSeconds)
{
	glm::vec2 position = _model->GetPosition();
	glm::vec2 direction = _model->GetMoveDirection();
	float speed = _model->GetMoveSpeed();

	position += direction * deltaSeconds * speed;

	_model->SetPosition(position);
}

void EnemyActorController::Rotate(float deltaSeconds)
{
	float rotate = _model->GetRotate();
	float rotationSpeed = _model->GetRotationSpeed();

	rotate += rotationSpeed * deltaSeconds;

	_model->SetRotate(rotate);
}