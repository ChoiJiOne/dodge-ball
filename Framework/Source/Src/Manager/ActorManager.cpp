#include "Manager/ActorManager.h"

Result<void> ActorManager::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_ACTOR_MANAGER"));
	}

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> ActorManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_ACTOR_MANAGER"));
	}

	for (size_t idx = 0; idx < _actorPool.size(); ++idx)
	{
		if (_actorPool[idx].actor)
		{
			if (_actorPool[idx].actor->IsInitialized())
			{
				_actorPool[idx].actor->Release();
			}

			_actorPool[idx].actor.reset();
			_actorPool[idx].isOccupied = false;
		}
	}

	_isInitialized = false;
	return Result<void>::Success();
}

void ActorManager::Destroy(const IActor* actor)
{
	int32_t actorID = -1;
	for (size_t idx = 0; idx < _actorPool.size(); ++idx)
	{
		if (actor == _actorPool[idx].actor.get())
		{
			actorID = static_cast<int32_t>(idx);
			break;
		}
	}

	if (actorID != -1 && _actorPool[actorID].actor)
	{
		if (_actorPool[actorID].actor->IsInitialized())
		{
			_actorPool[actorID].actor->Release();
		}

		_actorPool[actorID].actor.reset();
		_actorPool[actorID].isOccupied = false;
	}
}