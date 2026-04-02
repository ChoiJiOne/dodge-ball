#include "PlayerDataChunk.h"
#include "EnemyDataChunk.h"

#include "App/App.h"
#include "Actor/Enemy/EnemyActor.h"
#include "Config/GameConfig.h"
#include "Scene/GameScene.h"
#include "Actor/Player/PlayerActor.h"
#include "Context/PlayerContext.h"
#include "Scene/TitleScene.h"

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

	if (Result<void> result = dataChunkMgr->LoadDataChunk<EnemyDataChunk>("Resource/Enemy.bytes"); !result.IsSuccess())
	{
		return result;
	}

	ConfigManager* configMgr = managerLocator.GetConfigManager();
	std::string configPath = std::format("Config/{0}.yaml", NAME_OF(GameConfig));
	if (Result<void> result = configMgr->LoadConfig<GameConfig>(configPath); !result.IsSuccess())
	{
		return result;
	}

	ContextManager* contextMgr = managerLocator.GetContextManager();
	if (Result<void> result = contextMgr->RegisterContext<PlayerContext>(); !result.IsSuccess())
	{
		return result;
	}

	SceneManager* sceneMgr = managerLocator.GetSceneManager();
	if (Result<void> result = sceneMgr->RegisterScene<TitleScene>(); !result.IsSuccess())
	{
		return result;
	}

	if (Result<void> result = sceneMgr->RegisterScene<GameScene>(); !result.IsSuccess())
	{
		return result;
	}

	sceneMgr->TransitionImmediate<TitleScene>();

	return Result<void>::Success();
}

void App::OnPreTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
	InputManager* inputMgr = managerLocator.GetInputManager();
	if (inputMgr->GetKeyPress(EKey::ESCAPE) == EPress::PRESSED || inputMgr->IsAppCloseRequested())
	{
		RequestQuit();
		return;
	}
}

void App::OnTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
	IScene* currentScene = managerLocator.GetSceneManager()->GetCurrentScene();
	const auto& sceneActorMap = currentScene->GetSceneActorMap();
	for (const auto& [key, actor] : sceneActorMap)
	{
		actor->OnTick(deltaSeconds);
	}
}

void App::OnPostTick(const ManagerLocator& managerLocator, float deltaSeconds)
{
	IScene* currentScene = managerLocator.GetSceneManager()->GetCurrentScene();
	const auto& sceneActorMap = currentScene->GetSceneActorMap();

	PhysicManager* physicMgr = managerLocator.GetPhysicManager();
	physicMgr->Tick(sceneActorMap, deltaSeconds);
}

void App::OnRender(const ManagerLocator& managerLocator)
{
	RenderManager* renderMgr = managerLocator.GetRenderManager();

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

	sceneMgr->UnregisterScene<TitleScene>();
	sceneMgr->UnregisterScene<GameScene>();

	ContextManager* contextMgr = managerLocator.GetContextManager();
	contextMgr->UnregisterContext<PlayerContext>();

	return Result<void>::Success();
}

void App::RequestQuit()
{
	IApp::ExecuteRequestQuit();
}
