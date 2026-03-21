#include "App/AppContext.h"

AppContext::AppContext(
	ActorManager* actorMgr,
	ConfigManager* configMgr,
	ContextManager* contextMgr,
	DataChunkManager* dataChunkMgr,
	InputManager* inputMgr,
	RenderManager* renderMgr,
	SceneManager* sceneMgr
) : _actorMgr(actorMgr),
	_configMgr(configMgr),
	_contextMgr(contextMgr),
	_dataChunkMgr(dataChunkMgr),
	_inputMgr(inputMgr),
	_renderMgr(renderMgr),
	_sceneMgr(sceneMgr)
{
}

AppContext::~AppContext()
{
	_actorMgr = nullptr;
	_configMgr = nullptr;
	_contextMgr = nullptr;
	_dataChunkMgr = nullptr;
	_inputMgr = nullptr;
	_renderMgr = nullptr;
	_sceneMgr = nullptr;
}