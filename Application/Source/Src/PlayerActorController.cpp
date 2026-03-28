#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Manager/ConfigManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Particle/ParticleActor.h"
#include "Utils/LogUtils.h"

#include "AppDef.h"
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

		_model->SetPosition(position);
		_model->SetColor(color);
		_model->SetRadius(radius);
		_model->SetMoveSpeed(moveSpeed);
		_model->SetMoveDirection(moveDirection);
		_model->SetCollidable(true);
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
	_inputMgr = nullptr;
	_model = nullptr;
}

void PlayerActorController::OnTick(float deltaSeconds)
{
	if (_model->IsDead())
	{
		return;
	}

	UpdateMoveDirection();
	Move(deltaSeconds);
	UpdateDirectionByBounds();
}

void PlayerActorController::OnCollision(IActor* actor)
{
	if (_model->IsDead())
	{
		return;
	}

	_model->SetDead(true);

	SceneManager& sceneMgr = SceneManager::Get();
	IScene* currentScene = sceneMgr.GetCurrentScene();

	// 일단 생성 코드 추가
	std::string key = "Particle";
	ParticleActorParam param {
		_model->GetPosition(),
		200,
		5.0f,
		20.0f,
		100.0f,
		500.0f,
		1.0f,
		_model->GetColor()
	};

	if (Result<ParticleActor*> result = currentScene->CreateAndAddActor<ParticleActor>(key, DEF::SCENE_PARTICLE_ACTOR_ORDER, param); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_CREATE_OR_ADD_PARTICLE_ACTOR(key:{0})", key);
		return;
	}
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