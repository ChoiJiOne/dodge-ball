#include "Manager/ActorManager.h"

Result<void> ActorManager::Startup()
{
	if (_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_ACTOR_MANAGER"));

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> ActorManager::Shutdown()
{
	if (!_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_ACTOR_MANAGER"));

	for (auto& [key, actorPtr] : _cacheActorMap)
	{
		if (actorPtr)
		{
			if (actorPtr->IsInitialized())
				actorPtr->Release();

			actorPtr.reset();
		}
	}
	_cacheActorMap.clear();

	_isInitialized = false;
	return Result<void>::Success();
}

void ActorManager::DestroyActor(const std::string& key)
{
	auto iter = _cacheActorMap.find(key);
	if (iter != _cacheActorMap.end())
	{
		if (iter->second && iter->second->IsInitialized())
			iter->second->Release();
		
		_cacheActorMap.erase(iter);
	}
}