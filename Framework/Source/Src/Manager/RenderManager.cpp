#include <format>

#include <raylib.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp> 

#include "Macro/Macro.h"
#include "Manager/RenderManager.h"
#include "Particle/ParticleModel.h"
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
	if (!renderableModel || !renderableModel->IsVisible())
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

		glm::vec2 topPosition = glm::vec2(0.0f, halfSize.y);
		glm::vec2 bottomPosition = -topPosition;

		float radian = glm::radians(capsule->GetRotate());
		topPosition = glm::rotate(topPosition, radian);
		bottomPosition = glm::rotate(bottomPosition, radian);

		topPosition += capsule->GetPosition();
		bottomPosition += capsule->GetPosition();

		DrawCircleV(MathUtils::ToVector2(topPosition), capsule->GetRadius(), color);
		DrawCircleV(MathUtils::ToVector2(bottomPosition), capsule->GetRadius(), color);
		DrawRectanglePro(rectangle, pivot, rotate, color);
		break;
	}
	case ERenderType::PARTICLE:
	{
		const ParticleModel* particleModel = dynamic_cast<const ParticleModel*>(renderableModel);
		if (!particleModel)
		{
			break;
		}

		const std::vector<Particle>& particles = particleModel->GetParticles();
		for (const auto& particle : particles)
		{
			if (!particle.IsAlive())
			{
				continue;
			}

			Vector2 position = MathUtils::ToVector2(particle.GetPosition());
			float radius = particle.GetSize();
			Color color = MathUtils::ToColor8Bit(particle.GetColor());

			DrawCircleV(position, radius, color);
		}
		break;
	}
	case ERenderType::TEXT:
	{
		const ITextModel* textModel = dynamic_cast<const ITextModel*>(renderableModel);
		if (!textModel)
		{
			break;
		}

		const std::string& text = textModel->GetText();
		float fontSize = textModel->GetFontSize();
		Color color = MathUtils::ToColor8Bit(textModel->GetColor());

		Font font = GetFontDefault();
		Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1.0f);

		glm::vec2 textPosition = textModel->GetPosition();
		Vector2 position = { textPosition.x - textSize.x * 0.5f, textPosition.y - textSize.y * 0.5f };

		DrawTextEx(font, text.c_str(), position, fontSize, 1.0f, color);
		break;
	}
	default:
		LOG_E("UNDEFINE_RENDER_TYPE(type:{0})", static_cast<int32_t>(renderType));
		break;
	}
}