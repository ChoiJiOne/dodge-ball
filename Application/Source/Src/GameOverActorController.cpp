#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Manager/ContextManager.h"
#include "Manager/InputManager.h"
#include "Text/TextModel.h"
#include "Utils/LogUtils.h"

#include "AppDef.h"
#include "GameOverActorController.h"
#include "PlayerContext.h"

void GameOverActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	_inputMgr = InputManager::GetPtr();

	if (Result<void> result = InitializeModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeContext(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_CONTEXT(msg:{0})", result.GetError().GetMessage());
		return;
	}
}

void GameOverActorController::OnRelease()
{
	_model = nullptr;
}

void GameOverActorController::OnTick(float deltaSeconds)
{

}

Result<void> GameOverActorController::InitializeModel()
{
	Result<TextModel*> result = _ownerActor->GetModel<TextModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_model = result.GetValue();
	_model->SetVisible(false);
	return Result<void>::Success();
}

Result<void> GameOverActorController::InitializeContext()
{
	ContextManager& contextMgr = ContextManager::Get();
	Result<PlayerContext*> result = contextMgr.GetContext<PlayerContext>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_context = result.GetValue();

	Event<>& gameOverEvent = _context->GetGameOverEvent();
	gameOverEvent.RegisterCallback(NAME_OF(GameOverActorController),
		[this]() 
		{
			_model->SetVisible(true);
		});

	return Result<void>::Success();
}