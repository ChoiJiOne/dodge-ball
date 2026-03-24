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

	void Tick(const std::map<ActorKey, IActor*>& actorMap, float deltaSeconds);



private:
	friend class IManager<PhysicManager>;

	PhysicManager() = default;
	virtual ~PhysicManager() = default;

private:
	b2WorldId _worldId;
};