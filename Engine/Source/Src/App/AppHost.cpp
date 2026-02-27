#include <raylib.h>

#include "App/AppHost.h"
#include "Config/EngineConfig.h"

AppHost::~AppHost()
{
	if (_isInitialized)
		Shutdown();
}

Result<void> AppHost::Startup()
{
	if (_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_APP_HOST"));

	ConfigManager& configMgr = ConfigManager::Get();
	if (Result<void> result = configMgr.Startup(); !result.IsSuccess())
		return result;

	RenderManager& renderMgr = RenderManager::Get();
	if (Result<void> result = renderMgr.Startup(); !result.IsSuccess())
		return result;

	InputManager& inputMgr = InputManager::Get();
	if (Result<void> result = inputMgr.Startup(); !result.IsSuccess())
		return result;

	ActorManager& actorMgr = ActorManager::Get();
	if (Result<void> result = actorMgr.Startup(); !result.IsSuccess())
		return result;

	std::string configKey = NAME_OF(EngineConfig);
	std::string configPath = std::format("Config/{0}.yaml", configKey);
	EngineConfig* engineConfig = configMgr.Create<EngineConfig>(configPath, configKey);
	if (engineConfig == nullptr)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::FAILED_TO_LOAD_CONFIG, "FAILED_TO_LOAD_ENGINE_CONFIG"));

	int32_t windowWidth = engineConfig->GetWindowWidth();
	int32_t windowHeight = engineConfig->GetWindowHeight();
	const std::string& windowTitle = engineConfig->GetWindowTitle();
	int32_t targetFPS = engineConfig->GetTargetFPS();
	
	InitWindow(windowWidth, windowHeight, windowTitle.c_str());
	SetTargetFPS(targetFPS);

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> AppHost::Run(IApp& app)
{
	AppContext ctx(
		ActorManager::GetPtr(), 
		InputManager::GetPtr(),
		RenderManager::GetPtr(),
		ConfigManager::GetPtr()
	);
	ctx.SetRequestQuit([this]() { _isQuit = true; });

	if (Result<void> result = app.OnStartup(ctx); !result.IsSuccess())
		return result;

	while (!_isQuit)
	{
		_timer.Tick();
		app.OnPreTick(ctx, _timer.GetDeltaSeconds());
		app.OnTick(ctx, _timer.GetDeltaSeconds());
		app.OnPostTick(ctx, _timer.GetDeltaSeconds());
		app.OnRender(ctx);
	}

	if (Result<void> result = app.OnShutdown(ctx); !result.IsSuccess())
		return result;

	return Result<void>::Success();
}

Result<void> AppHost::Shutdown()
{
	if (!_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_APP_HOST"));
	
	CloseWindow();
	
	InputManager& inputMgr = InputManager::Get();
	if (Result<void> result = inputMgr.Shutdown(); !result.IsSuccess())
		return result;

	ActorManager& actorMgr = ActorManager::Get();
	if (Result<void> result = actorMgr.Shutdown(); !result.IsSuccess())
		return result;
	
	RenderManager& renderMgr = RenderManager::Get();
	if (Result<void> result = renderMgr.Shutdown(); !result.IsSuccess())
		return result;

	ConfigManager& configMgr = ConfigManager::Get();
	if (Result<void> result = configMgr.Shutdown(); !result.IsSuccess())
		return result;

	_isInitialized = false;
	return Result<void>::Success();
}