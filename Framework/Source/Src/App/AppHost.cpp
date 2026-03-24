#include <raylib.h>

#include "App/AppHost.h"
#include "Config/CoreConfig.h"

AppHost::~AppHost()
{
	if (_isInitialized)
		Shutdown();
}

Result<void> AppHost::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_APP_HOST"));
	}

	ConfigManager& configMgr = ConfigManager::Get();
	if (Result<void> result = configMgr.Startup(); !result.IsSuccess())
	{
		return result;
	}

	DataChunkManager& dataChunkMgr = DataChunkManager::Get();
	if (Result<void> result = dataChunkMgr.Startup(); !result.IsSuccess())
	{
		return result;
	}

	ContextManager& contextMgr = ContextManager::Get();
	if (Result<void> result = contextMgr.Startup(); !result.IsSuccess())
	{
		return result;
	}

	RenderManager& renderMgr = RenderManager::Get();
	if (Result<void> result = renderMgr.Startup(); !result.IsSuccess())
	{
		return result;
	}

	InputManager& inputMgr = InputManager::Get();
	if (Result<void> result = inputMgr.Startup(); !result.IsSuccess())
	{
		return result;
	}

	ActorManager& actorMgr = ActorManager::Get();
	if (Result<void> result = actorMgr.Startup(); !result.IsSuccess())
	{
		return result;
	}

	SceneManager& sceneMgr = SceneManager::Get();
	if (Result<void> result = sceneMgr.Startup(); !result.IsSuccess())
	{
		return result;
	}

	PhysicManager& physicMgr = PhysicManager::Get();
	if (Result<void> result = physicMgr.Startup(); !result.IsSuccess())
	{
		return result;
	}

	std::string configPath = std::format("Config/{0}.yaml", NAME_OF(CoreConfig));
	if (Result<void> result = configMgr.LoadConfig<CoreConfig>(configPath); !result.IsSuccess())
	{
		return result;
	}
	
	Result<const CoreConfig*> result = configMgr.GetConfig<CoreConfig>();
	if (!result.IsSuccess())
	{
		return Result<void>::Fail(result.GetError());
	}

	const CoreConfig* coreConfig = result.GetValue();
	int32_t windowWidth = coreConfig->GetWindowWidth();
	int32_t windowHeight = coreConfig->GetWindowHeight();
	const std::string& windowTitle = coreConfig->GetWindowTitle();
	int32_t targetFPS = coreConfig->GetTargetFPS();
	
	InitWindow(windowWidth, windowHeight, windowTitle.c_str());
	SetTargetFPS(targetFPS);

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> AppHost::Run(IApp& app)
{
	ManagerLocator managerLocator(
		ActorManager::GetPtr(),
		ConfigManager::GetPtr(),
		ContextManager::GetPtr(),
		DataChunkManager::GetPtr(),
		InputManager::GetPtr(),
		PhysicManager::GetPtr(),
		RenderManager::GetPtr(),
		SceneManager::GetPtr()
	);
	app.SetRequestQuit([this]() { _isQuit = true; });

	if (Result<void> result = app.OnStartup(managerLocator); !result.IsSuccess())
	{
		return result;
	}

	while (!_isQuit)
	{
		_timer.Tick();
		app.OnPreTick(managerLocator, _timer.GetDeltaSeconds());
		app.OnTick(managerLocator, _timer.GetDeltaSeconds());
		app.OnPostTick(managerLocator, _timer.GetDeltaSeconds());
		app.OnRender(managerLocator);
	}

	if (Result<void> result = app.OnShutdown(managerLocator); !result.IsSuccess())
	{
		return result;
	}

	return Result<void>::Success();
}

Result<void> AppHost::Shutdown()
{
	if (!_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_APP_HOST"));
	
	CloseWindow();

	PhysicManager& physicMgr = PhysicManager::Get();
	if (Result<void> result = physicMgr.Shutdown(); !result.IsSuccess())
	{
		return result;
	}

	ContextManager& contextMgr = ContextManager::Get();
	if (Result<void> result = contextMgr.Shutdown(); !result.IsSuccess())
	{
		return result;
	}

	SceneManager& sceneMgr = SceneManager::Get();
	if (Result<void> result = sceneMgr.Shutdown(); !result.IsSuccess())
	{
		return result;
	}
	
	InputManager& inputMgr = InputManager::Get();
	if (Result<void> result = inputMgr.Shutdown(); !result.IsSuccess())
	{
		return result;
	}

	ActorManager& actorMgr = ActorManager::Get();
	if (Result<void> result = actorMgr.Shutdown(); !result.IsSuccess())
	{
		return result;
	}
	
	RenderManager& renderMgr = RenderManager::Get();
	if (Result<void> result = renderMgr.Shutdown(); !result.IsSuccess())
	{
		return result;
	}

	DataChunkManager& dataChunkMgr = DataChunkManager::Get();
	if (Result<void> result = dataChunkMgr.Shutdown(); !result.IsSuccess())
	{
		return result;
	}

	ConfigManager& configMgr = ConfigManager::Get();
	if (Result<void> result = configMgr.Shutdown(); !result.IsSuccess())
	{
		return result;
	}

	_isInitialized = false;
	return Result<void>::Success();
}