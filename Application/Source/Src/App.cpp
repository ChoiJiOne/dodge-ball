#include <raylib.h>

#include "App.h"

//#include "DataPack/TestDataChunk.h"

App::App() 
{
}

Result<void> App::OnStartup(const AppContext& appCtx)
{
	//std::string inputFilePath = "Resource/TestData.bytes";

	//int32_t dataSize = 0;
	//uint8_t* fileData = LoadFileData(inputFilePath.c_str(), &dataSize);

	//msgpack::object_handle oh = msgpack::unpack(reinterpret_cast<const char*>(fileData), dataSize);
	//msgpack::object deserialized = oh.get();

	//TestDataChunk loadedData;
	//deserialized.convert(loadedData);

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