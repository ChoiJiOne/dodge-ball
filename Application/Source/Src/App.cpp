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

Result<void> App::OnStartup(const ManagerLocator& managerLocator)
{
	DataChunkManager* dataChunkMgr = managerLocator.GetDataChunkManager();
	if (Result<void> result = dataChunkMgr->LoadDataChunk<PlayerDataChunk>("Resource/Player.bytes"); !result.IsSuccess())
	{
		return result;
	}

	if (Result<void> result = dataChunkMgr->LoadDataChunk<BallDataChunk>("Resource/Ball.bytes"); !result.IsSuccess())
	{
		return result;
	}

	ConfigManager* configMgr = managerLocator.GetConfigManager();
	std::string configPath = std::format("Config/{0}.yaml", NAME_OF(GameConfig));
	if (Result<void> result = configMgr->LoadConfig<GameConfig>(configPath); !result.IsSuccess())
	{
		return result;
	}

	SceneManager* sceneMgr = managerLocator.GetSceneManager();
	if (Result<void> result = sceneMgr->RegisterScene<GameScene>(); !result.IsSuccess())
	{
		return result;
	}

	sceneMgr->Transition<GameScene>();

	return Result<void>::Success();
}

void App::OnPreTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
	InputManager* inputMgr = managerLocator.GetInputManager();
	if (inputMgr->GetKeyPress(EKey::ESCAPE) == EPress::PRESSED)
	{
		RequestQuit();
	}
}

void App::OnTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
	IScene* currentScene = managerLocator.GetSceneManager()->GetCurrentScene();
	const auto& sceneActorMap = currentScene->GetSceneActorMap();
	for (const auto& [key, actor] : sceneActorMap)
	{
		actor->Tick(deltaSeconds);
	}
}

void App::OnPostTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
}

void App::OnRender(const ManagerLocator& managerLocator)
{
	RenderManager* renderMgr = managerLocator.GetRenderManager();
	ActorManager* actorMgr = managerLocator.GetActorManager();

	renderMgr->BeginFrame(0.2f, 0.2f, 0.2f, 1.0f);

	IScene* currentScene = managerLocator.GetSceneManager()->GetCurrentScene();
	const auto& sceneActorMap = currentScene->GetSceneActorMap();

	for (const auto& [key, actor] : sceneActorMap)
	{
		const auto& renderableModelMap = actor->GetRenderableModelMap();
		for (const auto& [actorKey, model] : renderableModelMap)
		{
			renderMgr->Render(model);
		}
	}

	renderMgr->EndFrame();
}

Result<void> App::OnShutdown(const ManagerLocator& managerLocator)
{
	SceneManager* sceneMgr = managerLocator.GetSceneManager();

	IScene* currentScene = sceneMgr->GetCurrentScene();
	currentScene->OnExit();

	sceneMgr->UnregisterScene<GameScene>();

	return Result<void>::Success();
}
