#include "Manager/RenderManager.h"

Result<void> RenderManager::Startup()
{
	if (_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_RENDER_MANAGER"));

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> RenderManager::Shutdown()
{
	if (!_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_RENDER_MANAGER"));

	_isInitialized = false;
	return Result<void>::Success();
}
