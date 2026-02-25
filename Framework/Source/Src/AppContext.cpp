#include "AppContext.h"

AppContext::AppContext(
	ActorManager* actorMgr,
	InputManager* inputMgr,
	RenderManager* renderMgr,
	ConfigManager* configMgr
) : _actorMgr(actorMgr), 
	_inputMgr(inputMgr),
	_renderMgr(renderMgr),
	_configMgr(configMgr)
{
}

AppContext::~AppContext()
{
	_actorMgr = nullptr;
	_inputMgr = nullptr;
	_renderMgr = nullptr;
	_configMgr = nullptr;
}