#include "Manager/SceneManager.h"

Result<void> SceneManager::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_SCENE_MANAGER"));
	}

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> SceneManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_SCENE_MANAGER"));
	}

	for (auto& [Key, scene] : _cacheSceneMap)
	{
		scene.reset();
		scene = nullptr;
	}

	_isInitialized = false;
	return Result<void>::Success();
}