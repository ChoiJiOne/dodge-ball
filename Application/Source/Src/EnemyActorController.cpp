#include "Manager/ConfigManager.h"

#include "Actor/IActor.h"
#include "GameConfig.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

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

	ConfigManager& configMgr = ConfigManager::Get();
	if (Result<const GameConfig*> result = configMgr.GetConfig<GameConfig>(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG");
		return;
	}
	else
	{
		const GameConfig* config = result.GetValue();
		_spawnRangeY = static_cast<float>(config->GetSpawnRangeY());
		_deadZoneY = static_cast<float>(config->GetEnemyDeadZoneY());
		_fadeOutTime = config->GetEnemyFadeOutTime();
	}

	_onStateTickMap.emplace(EEnemyState::NONE, std::bind(&EnemyActorController::OnNoneStateTick, this, std::placeholders::_1));
	_onStateTickMap.emplace(EEnemyState::MOVE, std::bind(&EnemyActorController::OnMoveStateTick, this, std::placeholders::_1));
	_onStateTickMap.emplace(EEnemyState::FADE_OUT, std::bind(&EnemyActorController::OnFadeOutStateTick, this, std::placeholders::_1));
	_onStateTickMap.emplace(EEnemyState::DEAD, std::bind(&EnemyActorController::OnDeadStateTick, this, std::placeholders::_1));
}

void EnemyActorController::OnRelease()
{
	_model = nullptr;
}

void EnemyActorController::OnTick(float deltaSeconds)
{
	EEnemyState state = _model->GetState();
	auto iter = _onStateTickMap.find(state);
	if (iter != _onStateTickMap.end())
	{
		const std::function<void(float)>& onStateTick = iter->second;
		onStateTick(deltaSeconds);
	}
}

void EnemyActorController::Move(float deltaSeconds)
{
	glm::vec2 position = _model->GetPosition();
	glm::vec2 direction = _model->GetMoveDirection();
	float speed = _model->GetMoveSpeed();

	position += direction * deltaSeconds * speed;
	position.y = glm::clamp<float>(position.y, 0.0f, _deadZoneY);

	_model->SetPosition(position);
	if (position.y >= _deadZoneY)
	{
		_elapsedFadeOutTime = 0.0f;
		_model->SetState(EEnemyState::FADE_OUT);
	}
}

void EnemyActorController::Rotate(float deltaSeconds)
{
	float rotate = _model->GetRotate();
	float rotationSpeed = _model->GetRotationSpeed();

	rotate += rotationSpeed * deltaSeconds;

	_model->SetRotate(rotate);
}

void EnemyActorController::FadeOut(float deltaSeconds)
{
	_elapsedFadeOutTime += deltaSeconds;
	_elapsedFadeOutTime = glm::clamp<float>(_elapsedFadeOutTime, 0.0f, _fadeOutTime);

	float scale = 1.0f - (_elapsedFadeOutTime / _fadeOutTime);

	glm::vec4 color = _model->GetColor();
	color.a *= scale;

	_model->SetColor(color);
	if (color.a <= 0.0f)
	{
		_model->SetState(EEnemyState::DEAD);
	}
}

void EnemyActorController::OnNoneStateTick(float deltaSeconds)
{
	// NOTHING...
}

void EnemyActorController::OnMoveStateTick(float deltaSeconds)
{
	Move(deltaSeconds);
	Rotate(deltaSeconds);
}

void EnemyActorController::OnFadeOutStateTick(float deltaSeconds)
{
	FadeOut(deltaSeconds);
	Rotate(deltaSeconds);
}

void EnemyActorController::OnDeadStateTick(float deltaSeconds)
{
	// NOTHING...
}