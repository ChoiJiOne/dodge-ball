#include <glm/glm.hpp>

#include "Actor/IActor.h"
#include "Macro/Macro.h"
#include "Manager/ConfigManager.h"
#include "Manager/InputManager.h"
#include "Manager/SceneManager.h"
#include "Text/TextModel.h"
#include "Utils/LogUtils.h"

#include "Actor/UI/TitleActorController.h"
#include "Actor/UI/TitleTextModel.h"
#include "Config/GameConfig.h"
#include "Scene/GameScene.h"

void TitleActorController::OnInitialize(IActor* owner)
{
	IActorController::OnInitialize(owner);

	_inputMgr = InputManager::GetPtr();
	_sceneMgr = SceneManager::GetPtr();

	if (Result<void> result = InitializeTitleModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_TITLE_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeHintModel(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_HINT_MODEL(msg:{0})", result.GetError().GetMessage());
		return;
	}

	if (Result<void> result = InitializeModelsFromConfig(); !result.IsSuccess())
	{
		LOG_E("FAILED_TO_INITIALIZE_MODELS_FROM_CONFIG(msg:{0})", result.GetError().GetMessage());
		return;
	}
}

void TitleActorController::OnRelease()
{
	_titleModel = nullptr;
	_hintModel = nullptr;
	_inputMgr = nullptr;
	_sceneMgr = nullptr;
}

void TitleActorController::OnTick(float deltaSeconds)
{
	_blinkTime += deltaSeconds;

	glm::vec4 color = _hintModel->GetColor();
	color.a = (glm::sin(_blinkTime * _blinkSpeed) + 1.0f) * 0.5f;
	_hintModel->SetColor(color);

	if (_inputMgr->GetKeyPress(EKey::SPACE) == EPress::PRESSED)
	{
		_sceneMgr->Transition<GameScene>();
	}
}

Result<void> TitleActorController::InitializeTitleModel()
{
	Result<TitleTextModel*> result = _ownerActor->GetModel<TitleTextModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_titleModel = result.GetValue();
	_titleModel->SetText("DODGE BALL");
	return Result<void>::Success();
}

Result<void> TitleActorController::InitializeHintModel()
{
	Result<TextModel*> result = _ownerActor->GetModel<TextModel>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	_hintModel = result.GetValue();
	_hintModel->SetText("PRESS SPACE TO START");
	return Result<void>::Success();
}

Result<void> TitleActorController::InitializeModelsFromConfig()
{
	Result<const GameConfig*> result = ConfigManager::Get().GetConfig<GameConfig>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const GameConfig* config = result.GetValue();

	_titleModel->SetPosition(config->GetTitleTextPosition());
	_titleModel->SetColor(config->GetTitleTextColor());
	_titleModel->SetFontSize(config->GetTitleTextFontSize());

	_hintModel->SetPosition(config->GetTitleHintTextPosition());
	_hintModel->SetColor(config->GetTitleHintTextColor());
	_hintModel->SetFontSize(config->GetTitleHintTextFontSize());
	_blinkSpeed = config->GetTitleHintTextBlinkSpeed();

	return Result<void>::Success();
}
