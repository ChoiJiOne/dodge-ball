#pragma once

#include <list>

#include "Actor/IActor.h"
#include "Manager/IManager.h"
#include "Utils/LogUtils.h"

class PhysicManager : public IManager<PhysicManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(PhysicManager);

	virtual Result<void> Startup() override;
	virtual Result<void> Shutdown() override;

	void Tick(const std::map<ActorKey, IActor*>& actorMap);

private:
	friend class IManager<PhysicManager>;

	PhysicManager() = default;
	virtual ~PhysicManager() = default;
};