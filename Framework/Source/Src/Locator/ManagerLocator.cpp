#include "Locator/ManagerLocator.h"

ManagerLocator::ManagerLocator(
	ActorManager* actorMgr,
	ConfigManager* configMgr,
	ContextManager* contextMgr,
	DataChunkManager* dataChunkMgr,
	InputManager* inputMgr,
	PhysicManager* physicMgr,
	RenderManager* renderMgr,
	SceneManager* sceneMgr
) : _actorMgr(actorMgr),
	_configMgr(configMgr),
	_contextMgr(contextMgr),
	_dataChunkMgr(dataChunkMgr),
	_inputMgr(inputMgr),
	_physicMgr(physicMgr),
	_renderMgr(renderMgr),
	_sceneMgr(sceneMgr)
{
}

ManagerLocator::~ManagerLocator()
{
	_actorMgr = nullptr;
	_configMgr = nullptr;
	_contextMgr = nullptr;
	_dataChunkMgr = nullptr;
	_inputMgr = nullptr;
	_physicMgr = nullptr;
	_renderMgr = nullptr;
	_sceneMgr = nullptr;
}