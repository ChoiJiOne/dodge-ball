#pragma once

#include <map>
#include <memory>

#include "Context/IContext.h"
#include "Manager/IManager.h"
#include "Utils/LogUtils.h"

class ContextManager : public IManager<ContextManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(ContextManager);

	virtual Result<void> Startup() override;
	virtual Result<void> Shutdown() override;

	template <typename TContext, typename... Args>
	Result<void> RegisterContext(Args&&... args)
	{
		std::string key = typeid(TContext).name();
		auto iter = _cacheContextMap.find(key);
		if (iter != _cacheContextMap.end()) {
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_EXIST_CONTEXT,
				std::format("ALREADY_EXIST_CONTEXT:{0}", key)
			));
		}

		std::unique_ptr<TContext> context = std::make_unique<TContext>(std::forward<Args>(args)...);
		_cacheContextMap.emplace(key, std::move(context));

		return Result<void>::Success();
	}

	template <typename TContext>
	void UnregisterContext()
	{
		std::string key = typeid(TContext).name();
		auto iter = _cacheContextMap.find(key);
		if (iter != _cacheContextMap.end())
		{
			iter->second.reset();
			iter->second = nullptr;

			_cacheContextMap.erase(iter);
		}
	}

	template <typename TContext>
	Result<TContext*> GetContext()
	{
		std::string key = typeid(TContext).name();
		auto iter = _cacheContextMap.find(key);
		if (iter == _cacheContextMap.end())
		{
			return Result<TContext*>::Fail(MAKE_ERROR(
				EErrorCode::NOT_FOUND_CONTEXT,
				std::format("NOT_FOUND_CONTEXT:{0}", key)
			));
		}

		TContext* contextPtr = reinterpret_cast<TContext*>(iter->second.get());
		return Result<TContext*>::Success(contextPtr);
	}

private:
	friend class IManager<ContextManager>;

	ContextManager() = default;
	virtual ~ContextManager() = default;

private:
	std::map<std::string, std::unique_ptr<IContext>> _cacheContextMap;
};