#include <raylib.h>

#include "PlayerDataChunk.h"
#include "BallDataChunk.h"

#include "App.h"
#include "PlayerBallActor.h"

App::App() 
{
}

Result<void> App::OnStartup(const AppContext& appCtx)
{
	DataChunkManager* dataChunkMgr = appCtx.GetDataChunkManager();
	if (Result<void> result = dataChunkMgr->LoadDataChunk<PlayerDataChunk>("Resource/Player.bytes"); !result.IsSuccess())
		return result;

	if (Result<void> result = dataChunkMgr->LoadDataChunk<BallDataChunk>("Resource/Ball.bytes"); !result.IsSuccess())
		return result;

	Result<PlayerBallActor*> result = appCtx.GetActorManager()->CreateActor<PlayerBallActor>("PlayerBall");
	if (!result.IsSuccess())
		return Result<void>::Fail(result.GetError());

	_actors.push_back(result.GetValue());

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
	ActorManager* actorMgr = appCtx.GetActorManager();

	renderMgr->BeginFrame(0.5f, 0.5f, 0.5f, 1.0f);

	for (auto& actor : _actors)
	{
		const auto& renderableModelMap = actor->GetRenderableModelMap();
		for (const auto& [key, model] : renderableModelMap)
		{
			renderMgr->Render(model);
		}
	}
	
	renderMgr->EndFrame();
}

Result<void> App::OnShutdown(const AppContext& appCtx)
{
	return Result<void>::Success();
}
