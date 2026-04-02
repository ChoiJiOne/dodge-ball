#include <format>

#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Manager/ContextManager.h"
#include "Manager/ConfigManager.h"
#include "Text/TextModel.h"
#include "Utils/LogUtils.h"

#include "Config/GameConfig.h"
#include "Actor/UI/PlayTimeActorController.h"
#include "Context/PlayerContext.h"

void PlayTimeActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	if (Result<void> result = InitializeModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeModelFromConfig(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MODEL_FROM_CONFIG(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeContext(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_CONTEXT(msg:{0})", result.GetError().GetMessage());
		return;
	}
}

void PlayTimeActorController::OnRelease()
{
	_context->GetGameOverEvent().UnregisterCallback(NAME_OF(PlayTimeActorController));

	_model = nullptr;
	_context = nullptr;
}

void PlayTimeActorController::OnTick(float deltaSeconds)
{
	if (!_model->IsVisible())
		return;

	_model->SetText(std::format("{:.1f}s", _context->GetCurrentPlayTime()));
}

Result<void> PlayTimeActorController::InitializeModel()
{
	Result<TextModel*> result = _ownerActor->GetModel<TextModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_model = result.GetValue();
	_model->SetVisible(true);
	return Result<void>::Success();
}

Result<void> PlayTimeActorController::InitializeModelFromConfig()
{
	Result<const GameConfig*> result = ConfigManager::Get().GetConfig<GameConfig>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const GameConfig* config = result.GetValue();

	_model->SetText("0.0s");
	_model->SetPosition(config->GetPlayTimeTextPosition());
	_model->SetColor(config->GetPlayTimeTextColor());
	_model->SetFontSize(config->GetPlayTimeTextFontSize());

	return Result<void>::Success();
}

Result<void> PlayTimeActorController::InitializeContext()
{
	Result<PlayerContext*> result = ContextManager::Get().GetContext<PlayerContext>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_context = result.GetValue();
	_context->GetGameOverEvent().RegisterCallback(NAME_OF(PlayTimeActorController), [this]()
	{
		_model->SetVisible(false);
	});

	return Result<void>::Success();
}
