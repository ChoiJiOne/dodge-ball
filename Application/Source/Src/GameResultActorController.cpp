#include <format>

#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Manager/ContextManager.h"
#include "Manager/ConfigManager.h"
#include "Utils/LogUtils.h"

#include "GameConfig.h"
#include "PlayerContext.h"
#include "GameResultActorController.h"
#include "GameResultModel.h"

void GameResultActorController::OnInitialize(IActor* owner)
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

void GameResultActorController::OnRelease()
{
	_context->GetGameOverEvent().UnregisterCallback(NAME_OF(GameResultActorController));

	_model = nullptr;
	_context = nullptr;
}

void GameResultActorController::OnTick(float deltaSeconds)
{
}

Result<void> GameResultActorController::InitializeModel()
{
	Result<GameResultModel*> result = _ownerActor->GetModel<GameResultModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_model = result.GetValue();
	_model->SetVisible(false);
	return Result<void>::Success();
}

Result<void> GameResultActorController::InitializeModelFromConfig()
{
	Result<const GameConfig*> result = ConfigManager::Get().GetConfig<GameConfig>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const GameConfig* config = result.GetValue();

	_model->SetPosition(config->GetGameResultTextPosition());
	_model->SetColor(config->GetGameResultTextColor());
	_model->SetFontSize(config->GetGameResultTextFontSize());

	return Result<void>::Success();
}

Result<void> GameResultActorController::InitializeContext()
{
	Result<PlayerContext*> result = ContextManager::Get().GetContext<PlayerContext>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_context = result.GetValue();
	_context->GetGameOverEvent().RegisterCallback(NAME_OF(GameResultActorController), [this]()
	{
		_model->SetText(std::format("PLAY TIME: {:.1f}s  BEST: {:.1f}s", _context->GetCurrentPlayTime(), _context->GetBestPlayTime()));
		_model->SetVisible(true);
	});

	return Result<void>::Success();
}
