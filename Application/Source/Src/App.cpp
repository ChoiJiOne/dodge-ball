#include <raylib.h>

#include "PlayerDataChunk.h"
#include "BallDataChunk.h"

#include "App.h"
#include "EnemyActor.h"
#include "GameConfig.h"
#include "GameScene.h"
#include "PlayerActor.h"

App::App() 
{
}

Result<void> App::OnStartup(const AppContext& appCtx)
{
	DataChunkManager* dataChunkMgr = appCtx.GetDataChunkManager();
	if (Result<void> result = dataChunkMgr->LoadDataChunk<PlayerDataChunk>("Resource/Player.bytes"); !result.IsSuccess())
	{
		return result;
	}

	if (Result<void> result = dataChunkMgr->LoadDataChunk<BallDataChunk>("Resource/Ball.bytes"); !result.IsSuccess())
	{
		return result;
	}

	ConfigManager* configMgr = appCtx.GetConfigManager();
	std::string configPath = std::format("Config/{0}.yaml", NAME_OF(GameConfig));
	if (Result<void> result = configMgr->LoadConfig<GameConfig>(configPath); !result.IsSuccess())
	{
		return result;
	}

	SceneManager* sceneMgr = appCtx.GetSceneManager();
	if (Result<void> result = sceneMgr->RegisterScene<GameScene>(); !result.IsSuccess())
	{
		return result;
	}

	sceneMgr->Transition<GameScene>();

	return Result<void>::Success();
}

void App::OnPreTick(const AppContext& appCtx, float deltaSeconds)
{
	InputManager* inputMgr = appCtx.GetInputManager();
	if (inputMgr->GetKeyPress(EKey::ESCAPE) == EPress::PRESSED)
	{
		appCtx.RequestQuit();
	}
}

void App::OnTick(const AppContext& appCtx, float deltaSeconds)
{
	IScene* currentScene = appCtx.GetSceneManager()->GetCurrentScene();
	const auto& sceneActorMap = currentScene->GetSceneActorMap();
	for (const auto& [key, actor] : sceneActorMap)
	{
		actor->Tick(deltaSeconds);
	}
}

void App::OnPostTick(const AppContext& appCtx, float deltaSeconds)
{
}

void App::OnRender(const AppContext& appCtx)
{
	RenderManager* renderMgr = appCtx.GetRenderManager();
	ActorManager* actorMgr = appCtx.GetActorManager();

	renderMgr->BeginFrame(0.5f, 0.5f, 0.5f, 1.0f);

	IScene* currentScene = appCtx.GetSceneManager()->GetCurrentScene();
	const auto& sceneActorMap = currentScene->GetSceneActorMap();

	for (const auto& [key, actor] : sceneActorMap)
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
	SceneManager* sceneMgr = appCtx.GetSceneManager();

	IScene* currentScene = sceneMgr->GetCurrentScene();
	currentScene->OnExit();

	sceneMgr->UnregisterScene<GameScene>();

	return Result<void>::Success();
}
