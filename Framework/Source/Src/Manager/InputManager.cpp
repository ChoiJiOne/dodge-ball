#include <raylib.h>

#include "Macro/GameAssert.h"
#include "Manager/InputManager.h"

Result<void> InputManager::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_INPUT_MANAGER"));
	}

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> InputManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_INPUT_MANAGER"));
	}

	_isInitialized = false;
	return Result<void>::Success();
}

EPress InputManager::GetKeyPress(const EKey& key)
{
	int32_t keyCode = static_cast<int>(key);
	int32_t keyState = GetKeyStateEx(keyCode);
	return static_cast<EPress>(keyState);
}