#include <format>

#include <raylib.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp> 

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
		
		glm::vec2 size = rect->GetSize();
		glm::vec2 halfSize = size * 0.5f;
		glm::vec2 position = rect->GetPosition();

		Rectangle rectangle { position.x, position.y, size.x, size.y };
		Vector2 pivot = MathUtils::ToVector2(halfSize);
		Color color = MathUtils::ToColor8Bit(rect->GetColor());
		float rotate = rect->GetRotate();

		DrawRectanglePro(rectangle, pivot, rotate, color);
		break;
	}
	case ERenderType::CAPSULE:
	{
		const ICapsuleModel* capsule = dynamic_cast<const ICapsuleModel*>(renderableModel);
		if (!capsule)
		{
			break;
		}

		glm::vec2 size = glm::vec2(capsule->GetRadius() * 2.0f, capsule->GetHeight());
		glm::vec2 halfSize = size * 0.5f;
		glm::vec2 position = capsule->GetPosition();

		Rectangle rectangle{ position.x, position.y, size.x, size.y };
		Vector2 pivot = MathUtils::ToVector2(halfSize);
		Color color = MathUtils::ToColor8Bit(capsule->GetColor());
		float rotate = capsule->GetRotate();

		// 1. 캡슐의 상단, 하단의 위치를 얻는다. (원점 기준)
		glm::vec2 topPosition = glm::vec2(0.0f, halfSize.y);
		glm::vec2 bottomPosition = -topPosition;

		// 2. 캡슐 상단, 하단 좌표를 rotate도 만큼 돌린다.
		float radian = glm::radians(capsule->GetRotate());
		topPosition = glm::rotate(topPosition, radian);
		bottomPosition = glm::rotate(bottomPosition, radian);

		// 3. 돌린 좌표에 +position 만큼 더해준다.
		topPosition += capsule->GetPosition();
		bottomPosition += capsule->GetPosition();

		// 4. 더한 좌표 기준으로 원을 그린다.
		DrawCircleV(MathUtils::ToVector2(topPosition), capsule->GetRadius(), color);
		DrawCircleV(MathUtils::ToVector2(bottomPosition), capsule->GetRadius(), color);
		DrawRectanglePro(rectangle, pivot, rotate, color);
	}
	default:
		break;
	}
}