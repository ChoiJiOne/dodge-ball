#pragma once

#include <functional>

#include "Manager/ActorManager.h"
#include "Manager/ConfigManager.h"
#include "Manager/ContextManager.h"
#include "Manager/DataChunkManager.h"
#include "Manager/InputManager.h"
#include "Manager/PhysicManager.h"
#include "Manager/RenderManager.h"
#include "Manager/SceneManager.h"

class ManagerLocator
{
public:
	ManagerLocator(
		ActorManager* actorMgr,
		ConfigManager* configMgr,
		ContextManager* contextMgr,
		DataChunkManager* dataChunkMgr,
		InputManager* inputMgr,
		PhysicManager* physicMgr,
		RenderManager* renderMgr,
		SceneManager* sceneMgr
	);
	virtual ~ManagerLocator();

	DISALLOW_COPY_AND_ASSIGN(ManagerLocator);

	ActorManager* GetActorManager() const { return _actorMgr; }
	ConfigManager* GetConfigManager() const { return _configMgr; }
	ContextManager* GetContextManager() const { return _contextMgr; }
	DataChunkManager* GetDataChunkManager() const { return _dataChunkMgr; }
	InputManager* GetInputManager() const { return _inputMgr; }
	PhysicManager* GetPhysicManager() const { return _physicMgr; }
	RenderManager* GetRenderManager() const { return _renderMgr; }
	SceneManager* GetSceneManager() const { return _sceneMgr; }

private:
	ActorManager* _actorMgr = nullptr;
	ConfigManager* _configMgr = nullptr;
	ContextManager* _contextMgr = nullptr;
	DataChunkManager* _dataChunkMgr = nullptr;
	InputManager* _inputMgr = nullptr;
	PhysicManager* _physicMgr = nullptr;
	RenderManager* _renderMgr = nullptr;
	SceneManager* _sceneMgr = nullptr;
};