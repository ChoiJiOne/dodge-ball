#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Manager/ContextManager.h"
#include "Manager/ConfigManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Text/TextModel.h"
#include "Utils/LogUtils.h"

#include "AppDef.h"
#include "GameConfig.h"
#include "GameOverActorController.h"
#include "GameScene.h"
#include "PlayerContext.h"

void GameOverActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	_inputMgr = InputManager::GetPtr();
	_sceneMgr = SceneManager::GetPtr();

	if (Result<void> result = InitializeModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeModelFromConfig(); !result.IsSuccess())
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
	Event<>& gameOverEvent = _context->GetGameOverEvent();
	gameOverEvent.UnregisterCallback(NAME_OF(GameOverActorController));

	_model = nullptr;
}

void GameOverActorController::OnTick(float deltaSeconds)
{
	if (!_context->IsGameOver())
	{
		return;
	}

	if (_inputMgr->GetKeyPress(EKey::SPACE) == EPress::PRESSED)
	{
		_sceneMgr->Transition<GameScene>();
	}
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

Result<void> GameOverActorController::InitializeModelFromConfig()
{
	ConfigManager& configMgr = ConfigManager::Get();
	Result<const GameConfig*> result = configMgr.GetConfig<GameConfig>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const GameConfig* config = result.GetValue();

	_model->SetText("GAME OVER");
	_model->SetPosition(config->GetGameOverTextPosition());
	_model->SetColor(config->GetGameOverTextColor());
	_model->SetFontSize(config->GetGameOverTextFontSize());

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
	gameOverEvent.RegisterCallback(NAME_OF(GameOverActorController), [this]() { _model->SetVisible(true); });

	return Result<void>::Success();
}