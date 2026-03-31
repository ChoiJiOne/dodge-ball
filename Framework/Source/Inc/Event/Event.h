#pragma once

#include <functional>
#include <string>
#include <map>

#include "Error/Result.h"

template <typename... Args>
class Event
{
public:
	Result<void> RegisterCallback(const std::string& key, std::function<void(Args...)> callback)
	{
		const auto& iter = _listenerMap.find(key);
		if (iter != _listenerMap.end())
		{
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_EXIST_EVENT_CALLBACK,
				std::format("ALREADY_EXIST_EVENT_CALLBACK:{0}", key)
			));
		}

		_listenerMap.emplace(key, callback);
		return Result<void>::Success();
	}

	void UnregisterCallback(const std::string& key)
	{
		const auto& iter = _listenerMap.find(key);
		if (iter != _listenerMap.end())
		{
			_listenerMap.erase(key);
		}
	}

	void Invoke(Args... args)
	{
		for (auto& [key, callback] : _listenerMap)
		{
			if (callback != nullptr)
			{
				callback(args...);
			}
		}
	}

private:
	std::map<std::string, std::function<void(Args...)>> _listenerMap;
};