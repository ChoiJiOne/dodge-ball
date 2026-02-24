#include "AppHost.h"

AppHost::~AppHost()
{
	if (_isInitialized)
	{
		Shutdown();
	}
}

Result<void> AppHost::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_FRAMEWORK_APP"));
	}

	InputManager& inputMgr = InputManager::Get();
	Result<void> resultInputMgrStartup = inputMgr.Startup();
	if (!resultInputMgrStartup.IsSuccess())
	{
		return resultInputMgrStartup;
	}

	ActorManager& actorMgr = ActorManager::Get();
	Result<void> resultActorMgrStartup = actorMgr.Startup();
	if (!resultActorMgrStartup.IsSuccess())
	{
		return resultActorMgrStartup;
	}

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> AppHost::Run(IApp& app)
{
	AppContext ctx(ActorManager::GetPtr(), InputManager::GetPtr());
	ctx.SetRequestQuit([this]() { _isQuit = true; });

	Result<void> resultStartup = app.OnStartup(ctx);
	if (!resultStartup.IsSuccess())
	{
		return resultStartup;
	}

	while (!_isQuit)
	{
		UpdateTick(ctx.GetInputManager());
		app.OnPreTick(ctx, _timer.GetDeltaSeconds());
		app.OnTick(ctx, _timer.GetDeltaSeconds());
		app.OnPostTick(ctx, _timer.GetDeltaSeconds());
		app.OnRender(ctx);
	}

	Result<void> resultShutdown = app.OnShutdown(ctx);
	if (!resultShutdown.IsSuccess())
	{
		return resultShutdown;
	}

	return Result<void>::Success();
}

Result<void> AppHost::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_FRAMEWORK_APP"));
	}

	ActorManager& actorMgr = ActorManager::Get();
	Result<void> resultActorMgrShutdown = actorMgr.Shutdown();
	if (!resultActorMgrShutdown.IsSuccess())
	{
		return resultActorMgrShutdown;
	}

	InputManager& inputMgr = InputManager::Get();
	Result<void> resultInputMgrShutdown = inputMgr.Shutdown();
	if (!resultInputMgrShutdown.IsSuccess())
	{
		return resultInputMgrShutdown;
	}

	_isInitialized = false;
	return Result<void>::Success();
}

void AppHost::UpdateTick(InputManager* inputMgr)
{
	_timer.Tick();
}