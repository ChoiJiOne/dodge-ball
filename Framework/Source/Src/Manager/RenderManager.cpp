#include <raylib.h>

#include "Manager/RenderManager.h"

Result<void> RenderManager::Startup()
{
	if (_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_RENDER_MANAGER"));

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> RenderManager::Shutdown()
{
	if (!_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_RENDER_MANAGER"));

	_isInitialized = false;
	return Result<void>::Success();
}

void RenderManager::BeginFrame(float r, float g, float b, float a)
{
	BeginDrawing();

	Vector4 bgColor = { r, g, b, a };
	Color color = ColorFromNormalized(bgColor);
	ClearBackground(color);
}

void RenderManager::EndFrame()
{
	EndDrawing();
}