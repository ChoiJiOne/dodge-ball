#include <raylib.h>

#include "PlayerDataChunk.h"
#include "BallDataChunk.h"

#include "App.h"

App::App() 
{
}

Result<void> App::OnStartup(const AppContext& appCtx)
{
	DataChunkManager* dataChunkMgr = appCtx.GetDataChunkManager();
	Result<const PlayerDataChunk*> result = dataChunkMgr->LoadDataChunk<PlayerDataChunk>("Resource/Player.bytes");

	return Result<void>::Success();
}

void App::OnPreTick(const AppContext& appCtx, float deltaSeconds)
{
	InputManager* inputMgr = appCtx.GetInputManager();
	if (inputMgr->GetKeyPress(EKey::ESCAPE) == EPress::PRESSED)
		appCtx.RequestQuit();
}

void App::OnTick(const AppContext& appCtx, float deltaSeconds)
{
	for (auto& actor : _actors)
		actor->Tick(deltaSeconds);
}

void App::OnPostTick(const AppContext& appCtx, float deltaSeconds)
{
}

void App::OnRender(const AppContext& appCtx)
{
	RenderManager* renderMgr = appCtx.GetRenderManager();
	renderMgr->BeginFrame(1.0f, 1.0f, 1.0f, 1.0f);
	{
		DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
	}
	renderMgr->EndFrame();
}

Result<void> App::OnShutdown(const AppContext& appCtx)
{
	return Result<void>::Success();
}