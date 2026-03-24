#include <raylib.h>

#include "PlayerDataChunk.h"
#include "EnemyDataChunk.h"

#include "App.h"

App::App() 
{
}

Result<void> App::OnStartup(const ManagerLocator& managerLocator)
{
	return Result<void>::Success();
}

void App::OnPreTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
	InputManager* inputMgr = managerLocator.GetInputManager();
	if (inputMgr->GetKeyPress(EKey::ESCAPE) == EPress::PRESSED || inputMgr->IsAppCloseRequested())
	{
		RequestQuit();
	}
}

void App::OnTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
}

void App::OnPostTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
}

void App::OnRender(const ManagerLocator& managerLocator)
{
	RenderManager* renderMgr = managerLocator.GetRenderManager();
	ActorManager* actorMgr = managerLocator.GetActorManager();

	renderMgr->BeginFrame(0.2f, 0.2f, 0.2f, 1.0f);

	renderMgr->EndFrame();
}

Result<void> App::OnShutdown(const ManagerLocator& managerLocator)
{
	return Result<void>::Success();
}

void App::RequestQuit()
{
	IApp::ExecuteRequestQuit();
}
