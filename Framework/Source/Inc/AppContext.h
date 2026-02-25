#pragma once

#include <functional>

#include "Manager/ActorManager.h"
#include "Manager/InputManager.h"
#include "Manager/RenderManager.h"
#include "Manager/ConfigManager.h"

class AppContext
{
public:
	AppContext(
		ActorManager* actorMgr, 
		InputManager* inputMgr,
		RenderManager* renderMgr,
		ConfigManager* configMgr
	);
	virtual ~AppContext();

	DISALLOW_COPY_AND_ASSIGN(AppContext);

	void SetRequestQuit(const std::function<void()>& requestQuit) { _requestQuit = requestQuit; }
	void RequestQuit() const
	{
		if (_requestQuit)
			_requestQuit();
	}

	ActorManager* GetActorManager() const { return _actorMgr; }
	InputManager* GetInputManager() const { return _inputMgr; }
	RenderManager* GetRenderManager() const { return _renderMgr; }
	ConfigManager* GetConfigManager() const { return _configMgr; }

private:
	ActorManager* _actorMgr = nullptr;
	InputManager* _inputMgr = nullptr;
	RenderManager* _renderMgr = nullptr;
	ConfigManager* _configMgr = nullptr;

	std::function<void()> _requestQuit;
};