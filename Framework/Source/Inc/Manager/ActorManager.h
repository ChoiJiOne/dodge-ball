#pragma once

#include <memory>
#include <vector>

#include "IActor.h"
#include "IManager.h"

class ActorManager : public IManager<ActorManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(ActorManager);

	virtual Result<void> Startup() override;
	virtual Result<void> Shutdown() override;

	template <typename TActor, typename... Args>
	TActor* Create(Args&&... args)
	{
		int32_t actorID = -1;
		for (size_t idx = 0; idx < _actorPool.size(); ++idx)
		{
			if (!_actorPool[idx].actor && !_actorPool[idx].isOccupied)
			{
				actorID = idx;
				break;
			}
		}

		if (actorID == -1)
		{
			_actorPool.emplace_back();
			actorID = static_cast<int32_t>(_actorPool.size()) - 1;
		}

		_actorPool[actorID].isOccupied = true;
		_actorPool[actorID].actor = std::make_unique<TActor>(args...);

		return static_cast<TActor*>(_actorPool[actorID].actor.get());
	}

	void Destroy(const IActor* actor);

private:
	friend class IManager<ActorManager>;

	ActorManager() = default;
	virtual ~ActorManager() = default;

	struct ActorPoolSlot
	{
		std::unique_ptr<IActor> actor;
		bool isOccupied = false;
	};

private:
	std::vector<ActorPoolSlot> _actorPool;
};