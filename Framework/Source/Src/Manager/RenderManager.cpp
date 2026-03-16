#include <format>

#include <raylib.h>

#include "Macro/Macro.h"
#include "Manager/RenderManager.h"
#include "Utils/LogUtils.h"
#include "Utils/MathUtils.h"

Result<void> RenderManager::Startup()
{
	if (_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_RENDER_MANAGER"));
	}

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> RenderManager::Shutdown()
{
	if (!_isInitialized)
	{
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_RENDER_MANAGER"));
	}

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

void RenderManager::Render(const IRenderableModel* renderableModel)
{
	if (!renderableModel)
	{
		return;
	}

	ERenderType renderType = renderableModel->GetRenderType();

	switch (renderType)
	{
	case ERenderType::CIRCLE:
	{
		const ICircleModel* circle = dynamic_cast<const ICircleModel*>(renderableModel);
		if (!circle)
		{
			break;
		}

		Vector2 pos = MathUtils::ToVector2(circle->GetPosition());
		float radius = circle->GetRadius();
		Color color = MathUtils::ToColor8Bit(circle->GetColor());

		DrawCircleV(pos, radius, color);
		break;
	}
	case ERenderType::RECTANGLE:
	{
		const IRectModel* rect = dynamic_cast<const IRectModel*>(renderableModel);
		if (!rect)
		{
			break;
		}

		Vector2 pos = MathUtils::ToVector2(rect->GetPosition());
		Vector2 size = MathUtils::ToVector2(rect->GetSize());
		Color color = MathUtils::ToColor8Bit(rect->GetColor());

		DrawRectangleV(pos, size, color);
		break;
	}
	default:
		break;
	}
}