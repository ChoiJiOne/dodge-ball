#include "Manager/ConfigManager.h"
#include "Manager/PhysicManager.h"

#include "Actor/IActor.h"
#include "Config/GameConfig.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

#include "Actor/Enemy/EnemyActorController.h"

void EnemyActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	if (Result<void> result = InitializeModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeFromConfig(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_FROM_CONFIG(msg:{0})", result.GetError().GetMessage());
		return;
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

void EnemyActorController::OnCollision(IActor* actor)
{
}

Result<void> EnemyActorController::InitializeModel()
{
	Result<EnemyModel*> result = _ownerActor->GetModel<EnemyModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_model = result.GetValue();
	return Result<void>::Success();
}

Result<void> EnemyActorController::InitializeFromConfig()
{
	ConfigManager& configMgr = ConfigManager::Get();
	Result<const GameConfig*> result = configMgr.GetConfig<GameConfig>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const GameConfig* config = result.GetValue();
	_spawnRangeY = static_cast<float>(config->GetSpawnRangeY());
	_deadZoneY = static_cast<float>(config->GetEnemyDeadZoneY());
	_fadeOutTime = config->GetEnemyFadeOutTime();

	return Result<void>::Success();
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