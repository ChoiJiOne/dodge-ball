#pragma once

#include <functional>

#include "Utils/GameTimer.h"
#include "IApp.h"

class InputManager;

class AppHost
{
public:
	AppHost() = default;
	virtual ~AppHost();

	Result<void> Startup();
	Result<void> Run(IApp& app);
	Result<void> Shutdown();

private:
	bool _isInitialized = false;
	bool _isQuit = false;

	std::function<void(float)> _processTick;
	GameTimer _timer;
};