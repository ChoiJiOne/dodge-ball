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

	b2WorldDef worldDef = b2DefaultWorldDef(); // TODO: 향후 세부적인 World 설정은 CoreConfig 에서 설정할 예정.
	worldDef.gravity = (b2Vec2)(0.0f, 0.0f);

	_worldId = b2CreateWorld(&worldDef);

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> PhysicManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_PHYSIC_MANAGER"));
	}

	if (b2World_IsValid(_worldId))
	{
		b2DestroyWorld(_worldId);
		_worldId = b2_nullWorldId;
	}

	_isInitialized = false;
	return Result<void>::Success();
}

void PhysicManager::Tick(const std::map<ActorKey, IActor*>& actorMap, float deltaSeconds)
{

}
