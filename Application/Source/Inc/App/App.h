#pragma once

#include "Locator/ManagerLocator.h"

#include "App/IApp.h"

class App : public IApp
{
public:
	App();
	virtual ~App() {}

	DISALLOW_COPY_AND_ASSIGN(App);

	virtual Result<void> OnStartup(const ManagerLocator& managerLocator) override;
	virtual void OnPreTick(const ManagerLocator& managerLocator, float deltaSeconds) override;
	virtual void OnTick(const ManagerLocator& managerLocator, float deltaSeconds) override;
	virtual void OnPostTick(const ManagerLocator& managerLocator, float deltaSeconds) override;
	virtual void OnRender(const ManagerLocator& managerLocator) override;
	virtual Result<void> OnShutdown(const ManagerLocator& managerLocator) override;
	virtual void RequestQuit() override;
};