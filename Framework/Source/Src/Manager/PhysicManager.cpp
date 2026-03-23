#include <physac.h>

#include "Manager/PhysicManager.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

Result<void> PhysicManager::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_PHYSIC_MANAGER"));
	}

	InitPhysics();

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> PhysicManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_PHYSIC_MANAGER"));
	}

	ClosePhysics();

	_isInitialized = false;
	return Result<void>::Success();
}
