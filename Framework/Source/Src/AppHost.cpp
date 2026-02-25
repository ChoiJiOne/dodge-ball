#include <raylib.h>

#include "AppHost.h"

AppHost::~AppHost()
{
	if (_isInitialized)
		Shutdown();
}

Result<void> AppHost::Startup()
{
	if (_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_FRAMEWORK_APP"));

	// 여기 임시 코드. 삭제 예정.
	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");
	SetTargetFPS(60);

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
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_FRAMEWORK_APP"));
	
	InputManager& inputMgr = InputManager::Get();
	if (Result<void> result = inputMgr.Shutdown(); !result.IsSuccess())
		return result;

	ActorManager& actorMgr = ActorManager::Get();
	if (Result<void> result = actorMgr.Shutdown(); !result.IsSuccess())
		return result;

	CloseWindow();

	_isInitialized = false;
	return Result<void>::Success();
}