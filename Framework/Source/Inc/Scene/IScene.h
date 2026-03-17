#pragma once

#include <map>

#include "Actor/IActor.h"
#include "Manager/ActorManager.h"

struct SceneActorKey
{
	int32_t order;
	std::string key;

	bool operator<(const SceneActorKey& instance) const
	{
		if (order != instance.order)
		{
			return order < instance.order;
		}

		return key < instance.key;
	}
};

class IScene
{
public:
	IScene() : _actorMgr(ActorManager::GetPtr()) {}
	virtual ~IScene() { _actorMgr = nullptr; }

	DISALLOW_COPY_AND_ASSIGN(IScene);

	virtual Result<void> OnEnter() = 0;
	virtual Result<void> OnExit() = 0;

	const std::map<SceneActorKey, IActor*>& GetSceneActorMap() const { return _sceneActorMap; }

	template <typename TActor, typename... Args>
	Result<TActor*> CreateAndAddActor(const std::string& key, int32_t order, Args&&... args)
	{
		for (const auto& [sceneActorKey, actor] : _sceneActorMap) 
		{
			if (sceneActorKey.key == key)
			{
				return Result<TActor*>::Fail(MAKE_ERROR(
					EErrorCode::ALREADY_EXIST_ACTOR,
					std::format("ALREADY_EXIST_ACTOR:{0}", key)
				));
			}
		}

		Result<TActor*> result = _actorMgr->CreateActor<TActor>(key, std::forward<Args>(args)...);
		if (!result.IsSuccess())
		{
			return Result<TActor*>::Fail(result.GetError());
		}

		SceneActorKey sceneActorKey{ order, key };
		_sceneActorMap.emplace(sceneActorKey, result.GetValue());
		return result;
	}

	// WARN: 이거 외부에서 _sceneActorMap 루프 돌면서 호출하면 안됨. 반드시 하나의 액터에 대해서만 호출해야 함.
	void RemoveAndDestroyActor(const std::string& key)
	{
		for (auto iter = _sceneActorMap.begin(); iter != _sceneActorMap.end(); ++iter)
		{
			if (iter->first.key == key)
			{
				_sceneActorMap.erase(iter);
				_actorMgr->DestroyActor(iter->first.key);
				return;
			}
		}
	}

	template <typename TActor>
	Result<TActor*> GetActor(const std::string& key)
	{
		for (const auto& [sceneActorKey, sceneActor] : _sceneActorMap) 
		{
			if (sceneActorKey.key == key) 
			{
				TActor* sceneActorPtr = reinterpret_cast<TActor*>(sceneActor.get());
				return Result<TActor*>::Success(sceneActorPtr);
			}
		}

		return Result<TActor*>::Fail(MAKE_ERROR(
			EErrorCode::NOT_FOUND_ACTOR,
			std::format("NOT_FOUND_ACTOR:{0}", key)
		));
	}

protected:
	void ClearActorMap()
	{
		const auto& sceneActorMap = GetSceneActorMap();
		for (const auto& [sceneActorKey, sceneActor] : _sceneActorMap)
		{
			_actorMgr->DestroyActor(sceneActorKey.key);
		}

		_sceneActorMap.clear();
	}

private:
	ActorManager* _actorMgr = nullptr;
	std::map<SceneActorKey, IActor*> _sceneActorMap;
};