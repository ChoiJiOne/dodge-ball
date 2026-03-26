#include <box2d/box2d.h>

#include "Manager/PhysicManager.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

Result<void> PhysicManager::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_PHYSIC_MANAGER"));
	}

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> PhysicManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_PHYSIC_MANAGER"));
	}
	
	_isInitialized = false;
	return Result<void>::Success();
}

void PhysicManager::Tick(const std::map<ActorKey, IActor*>& actorMap, float deltaSeconds)
{
	for (auto iter = actorMap.begin(); iter != actorMap.end(); ++iter)
	{
		IActor* currentActor = iter->second;
		if (currentActor->GetCollidableModelMap().empty()) // NOTE: 비어 있으면 아무 것도 안함.
		{
			continue;
		}

		for (auto subIter = std::next(iter); subIter != actorMap.end(); ++subIter)
		{
			IActor* targetActor = subIter->second;
			if (targetActor->GetCollidableModelMap().empty())
			{
				continue;
			}

			for (auto& [currentKey, currentBound] : currentActor->GetCollidableModelMap())
			{
				for (auto& [targetKey, targetBound] : targetActor->GetCollidableModelMap())
				{
					if (IsCollidable(currentBound, targetBound) && currentBound->IsCollision(targetBound))
					{
						currentActor->OnCollision(targetActor);
						targetActor->OnCollision(currentActor);
					}
				}
			}
		}
	}
}

bool PhysicManager::IsCollidable(CollidableModel* lhsModel, CollidableModel* rhsModel)
{
	return lhsModel->IsCollidable() && rhsModel->IsCollidable();
}
