#pragma once

#include <functional>

#include "Manager/ActorManager.h"
#include "Manager/InputManager.h"

class AppContext
{
public:
	AppContext(ActorManager* actorMgr, InputManager* inputMgr);
	virtual ~AppContext();

	DISALLOW_COPY_AND_ASSIGN(AppContext);

	void SetRequestQuit(const std::function<void()>& requestQuit) { _requestQuit = requestQuit; }
	void RequestQuit() const
	{
		if (_requestQuit)
		{
			_requestQuit();
		}
	}

	ActorManager* GetActorManager() const { return _actorMgr; }
	InputManager* GetInputManager() const { return _inputMgr; }

private:
	ActorManager* _actorMgr = nullptr;
	InputManager* _inputMgr = nullptr;

	std::function<void()> _requestQuit;
};