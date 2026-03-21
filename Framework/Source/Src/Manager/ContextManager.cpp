#include "Manager/ContextManager.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

Result<void> ContextManager::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_CONTEXT_MANAGER"));
	}

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> ContextManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_CONTEXT_MANAGER"));
	}

	for (auto& [key, context] : _cacheContextMap)
	{
		context.reset();
		context = nullptr;
	}

	_isInitialized = false;
	return Result<void>::Success();
}
