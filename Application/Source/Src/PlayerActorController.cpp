#include "Actor/IActor.h"
#include "Manager/ConfigManager.h"
#include "Manager/InputManager.h"
#include "Manager/PhysicManager.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

#include "GameConfig.h"
#include "MoveBoundModel.h"
#include "PlayerActorController.h"
#include "PlayerModel.h"

void PlayerActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	_inputMgr = InputManager::GetPtr();

	if (Result<PlayerModel*> result = _ownerActor->GetModel<PlayerModel>(); !result.IsSuccess()) // Get이 실패할 수 있을까...?
	{
		LOG_E("FAILED_TO_GET_PLAYER_BALL_MODEL"); // 일단 로그를 찍어보자.
		return;
	}
	else
	{
		_model = result.GetValue();
	}
	
	ConfigManager& configMgr = ConfigManager::Get();
	if (Result<const GameConfig*> result = configMgr.GetConfig<GameConfig>(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG");
		return;
	}
	else
	{
		const GameConfig* config = result.GetValue();
		_moveRangeMinX = static_cast<float>(config->GetPlayerMoveRangeMinX());
		_moveRangeMaxX = static_cast<float>(config->GetPlayerMoveRangeMaxX());
		float moveRangeX = (_moveRangeMinX + _moveRangeMaxX) * 0.5f;
		float moveRangeY = static_cast<float>(config->GetPlayerMoveRangeY());
		bool isStartMovePositive = config->IsPlayerStartMovePositive();

		glm::vec2 position(moveRangeX, moveRangeY);
		glm::vec4 color(1.0f, 1.0f, 1.0f, 1.0f); // DataPack 기반으로 설정할 예정.
		float radius = 15.0f; // DataPack 기반으로 설정할 예정.
		float moveSpeed = 500.0f;  // DataPack 기반으로 설정할 예정.
		glm::vec2 moveDirection(isStartMovePositive ? +1.0f : -1.0f, 0.0f);

		PhysicManager& physicMgr = PhysicManager::Get();
		b2BodyId bodyId = physicMgr.CreateCircleSensorBody(owner, position, radius);

		_model->SetPosition(position);
		_model->SetColor(color);
		_model->SetRadius(radius);
		_model->SetMoveSpeed(moveSpeed);
		_model->SetMoveDirection(moveDirection);
		_model->SetBodyId(bodyId);
	}
	
	if (Result<MoveBoundModel*> result = _ownerActor->GetModel<MoveBoundModel>(); !result.IsSuccess()) // Get이 실패할 수 있을까...?
	{
		LOG_E("FAILED_TO_GET_MOVE_BOUND_MODEL"); // 일단 로그를 찍어보자.
		return;
	}
	else
	{
		MoveBoundModel* moveBoundModel = result.GetValue();

		moveBoundModel->SetPosition(_model->GetPosition());
		moveBoundModel->SetRadius(_model->GetRadius() * 1.5f);
		moveBoundModel->SetHeight(_moveRangeMaxX - _moveRangeMinX);
		moveBoundModel->SetRotate(90.0f);
		moveBoundModel->SetColor(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
	}
}

void PlayerActorController::OnRelease()
{
	PhysicManager& physicMgr = PhysicManager::Get();
	physicMgr.DestroySensorBody(_model->GetBodyId());

	_inputMgr = nullptr;
	_model = nullptr;
}

void PlayerActorController::OnTick(float deltaSeconds)
{
	UpdateMoveDirection();
	Move(deltaSeconds);
	UpdateDirectionByBounds();
}

void PlayerActorController::OnCollision(IActor* actor)
{

}

void PlayerActorController::UpdateMoveDirection()
{
	glm::vec2 direction = _model->GetMoveDirection();
	if (_inputMgr->GetKeyPress(EKey::SPACE) == EPress::PRESSED)
	{
		direction.x *= -1.0f;
	}
	_model->SetMoveDirection(direction);
}

void PlayerActorController::Move(float deltaSeconds)
{
	glm::vec2 position = _model->GetPosition();
	glm::vec2 direction = _model->GetMoveDirection();
	float speed = _model->GetMoveSpeed();

	position += direction * deltaSeconds * speed;
	position.x = glm::clamp(position.x, _moveRangeMinX, _moveRangeMaxX);

	_model->SetPosition(position);
}

void PlayerActorController::UpdateDirectionByBounds()
{
	glm::vec2 position = _model->GetPosition();
	glm::vec2 direction = _model->GetMoveDirection();

	if ((position.x <= _moveRangeMinX && direction.x < 0.0f) ||
		(position.x >= _moveRangeMaxX && direction.x > 0.0f))
	{
		direction.x *= -1.0f;
		_model->SetMoveDirection(direction);
	}
}