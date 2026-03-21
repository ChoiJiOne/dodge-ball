#pragma once

#include "Locator/ManagerLocator.h"
#include "Result.h"

class IApp
{
public:
	virtual Result<void> OnStartup(const ManagerLocator& managerLocator) = 0;
	virtual void OnPreTick(const ManagerLocator& managerLocator, float deltaSeconds) = 0;
	virtual void OnTick(const ManagerLocator& managerLocator, float deltaSeconds) = 0;
	virtual void OnPostTick(const ManagerLocator& managerLocator, float deltaSeconds) = 0;
	virtual void OnRender(const ManagerLocator& managerLocator) = 0;
	virtual Result<void> OnShutdown(const ManagerLocator& managerLocator) = 0;
	virtual void RequestQuit() = 0;

	void SetRequestQuit(const std::function<void()>& requestQuit) { _requestQuit = requestQuit; }
	void ExecuteRequestQuit() const
	{
		if (_requestQuit)
		{
			_requestQuit();
		}
	}
	
private:
	std::function<void()> _requestQuit;
};