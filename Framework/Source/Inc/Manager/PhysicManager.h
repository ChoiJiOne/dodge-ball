#pragma once

#include <memory>
#include <map>

#include <box2d/box2d.h>

#include "Actor/IActor.h"
#include "Manager/IManager.h"
#include "Utils/LogUtils.h"

class PhysicManager : public IManager<PhysicManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(PhysicManager);

	virtual Result<void> Startup() override;
	virtual Result<void> Shutdown() override;

	void Tick(float deltaSeconds);

	b2BodyId CreateRectSensorBody(IActor* ownerActor, const glm::vec2& position, const glm::vec2& size);
	b2BodyId CreateCircleSensorBody(IActor* ownerActor, const glm::vec2& position, float radius);
	
	void DestroySensorBody(b2BodyId bodyId);

private:
	friend class IManager<PhysicManager>;

	PhysicManager() = default;
	virtual ~PhysicManager() = default;

private:
	static const int32_t DEFAULT_SUB_STEP_COUNT = 1;

private:
	b2WorldId _worldId;
};