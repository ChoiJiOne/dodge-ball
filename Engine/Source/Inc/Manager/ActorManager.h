#pragma once

#include <format>
#include <map>
#include <memory>
#include <string>

#include "Actor/IActor.h"
#include "Manager/IManager.h"

class ActorManager : public IManager<ActorManager> 
{
public:
    DISALLOW_COPY_AND_ASSIGN(ActorManager);

    virtual Result<void> Startup() override;
    virtual Result<void> Shutdown() override;

    template <typename TActor, typename... Args>
    Result<TActor*> CreateActor(const std::string& key, Args &&...args) 
    {
		auto iter = _cacheActorMap.find(key);
		if (iter != _cacheActorMap.end()) {
			return Result<TActor*>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_EXIST_ACTOR,
				std::format("ALREADY_EXIST_ACTOR:{0}", key)
			));
		}

		std::unique_ptr<TActor> actor = std::make_unique<TActor>(std::forward<Args>(args)...);
		TActor* actorPtr = static_cast<TActor*>(actor.get());
		_cacheActorMap.emplace(key, std::move(actor));

		return Result<TActor*>::Success(actorPtr);
	}

	template <typename TActor> 
	Result<TActor*> GetActor(const std::string &key) 
	{
		auto iter = _cacheActorMap.find(key);
		if (iter == _cacheActorMap.end()) 
		{
			return Result<TActor *>::Fail(MAKE_ERROR(
				EErrorCode::NOT_INITIALIZED, 
				std::format("NOT_FOUND_ACTOR:{}", key)
			));
		}

		TActor *actorPtr = static_cast<TActor *>(iter->second.get());
		return Result<TActor*>::Success(actorPtr);
	}

	void DestroyActor(const std::string &key);

private:
	friend class IManager<ActorManager>;

	ActorManager() = default;
	virtual ~ActorManager() = default;

private:
	std::map<std::string, std::unique_ptr<IActor>> _cacheActorMap;
};