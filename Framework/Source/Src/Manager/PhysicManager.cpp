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

void PhysicManager::Tick(float deltaSeconds)
{
	b2World_Step(_worldId, deltaSeconds, DEFAULT_SUB_STEP_COUNT);

	b2SensorEvents sensorEvents = b2World_GetSensorEvents(_worldId);
	for (int32_t idx = 0; idx < sensorEvents.beginCount; ++idx)
	{
		const b2SensorBeginTouchEvent& sensorEvent = sensorEvents.beginEvents[idx];

		b2BodyId visitorBody = b2Shape_GetBody(sensorEvent.visitorShapeId);
		b2BodyId sensorBody = b2Shape_GetBody(sensorEvent.sensorShapeId);

		IActor* visitorActor = reinterpret_cast<IActor*>(b2Body_GetUserData(visitorBody));
		IActor* sensorActor = reinterpret_cast<IActor*>(b2Body_GetUserData(sensorBody));
		if (visitorActor && sensorActor)
		{
			visitorActor->OnCollision(sensorActor);
			sensorActor->OnCollision(visitorActor);
		}
	}
}

b2BodyId PhysicManager::CreateRectSensorBody(IActor* ownerActor, const glm::vec2& position, const glm::vec2& size)
{
	if (!b2World_IsValid(_worldId) || !ownerActor)
	{
		return b2_nullBodyId;
	}

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = { position.x, position.y };
	bodyDef.userData = reinterpret_cast<void*>(ownerActor);

	b2BodyId bodyId = b2CreateBody(_worldId, &bodyDef);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.isSensor = true;

	b2Polygon box = b2MakeBox(size.x / 2.0f, size.y / 2.0f);
	b2CreatePolygonShape(bodyId, &shapeDef, &box);
	return bodyId;
}

b2BodyId PhysicManager::CreateCircleSensorBody(IActor* ownerActor, const glm::vec2& position, float radius)
{
	if (!b2World_IsValid(_worldId) || !ownerActor)
	{
		return b2_nullBodyId;
	}

	b2BodyDef bodyDef = b2DefaultBodyDef();
	bodyDef.type = b2_dynamicBody;
	bodyDef.position = { position.x, position.y };
	bodyDef.userData = reinterpret_cast<void*>(ownerActor);

	b2BodyId bodyId = b2CreateBody(_worldId, &bodyDef);
	b2ShapeDef shapeDef = b2DefaultShapeDef();
	shapeDef.isSensor = true;

	b2Circle circle = { {0.0f, 0.0f}, radius };
	b2CreateCircleShape(bodyId, &shapeDef, &circle);
	return bodyId;
}

void PhysicManager::DestroySensorBody(b2BodyId bodyId)
{
	if (b2Body_IsValid(bodyId))
	{
		b2DestroyBody(bodyId);
	}
}
