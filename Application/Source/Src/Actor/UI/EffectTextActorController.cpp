#include <glm/glm.hpp>

#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Utils/LogUtils.h"

#include "Actor/UI/EffectTextActorController.h"

void EffectTextActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	if (Result<void> result = InitializeModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MODEL(msg:{0})", result.GetError().GetMessage());
	}
}

void EffectTextActorController::OnRelease()
{
	_model = nullptr;
}

void EffectTextActorController::OnTick(float deltaSeconds)
{
	if (_model->GetState() == EEffectTextState::ACTIVE)
	{
		OnActiveStateTick(deltaSeconds);
	}
}

Result<void> EffectTextActorController::InitializeModel()
{
	Result<EffectTextModel*> result = _ownerActor->GetModel<EffectTextModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_model = result.GetValue();
	return Result<void>::Success();
}

void EffectTextActorController::OnActiveStateTick(float deltaSeconds)
{
	float remaining = glm::max(0.0f, _model->GetRemainingLifeTime() - deltaSeconds);
	_model->SetRemainingLifeTime(remaining);

	glm::vec2 position = _model->GetPosition();
	position.y -= _model->GetMoveSpeed() * deltaSeconds;
	_model->SetPosition(position);

	glm::vec4 color = _model->GetColor();
	color.a = remaining / _model->GetInitialLifeTime();
	_model->SetColor(color);

	if (remaining <= 0.0f)
	{
		_model->SetVisible(false);
		_model->SetState(EEffectTextState::DEAD);
	}
}
