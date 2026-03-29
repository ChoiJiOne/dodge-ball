#pragma once

#include <glm/glm.hpp>

enum class ERenderType
{
	NONE = 0x00,
	RECTANGLE = 0x01,
	CIRCLE = 0x02,
	CAPSULE = 0x03,
	PARTICLE = 0x04,
	// 여기에 계속 추가.
};

class IRenderableModel
{
public:
	virtual ~IRenderableModel() = default;
	virtual ERenderType GetRenderType() const = 0;
	
	bool IsVisible() const { return _isVisible; }
	void SetVisible(bool isVisible) { _isVisible = isVisible; }

private:
	bool _isVisible = true;
};

class ICircleModel : public virtual IRenderableModel
{
public:
	virtual ~ICircleModel() = default;
	virtual ERenderType GetRenderType() const override { return ERenderType::CIRCLE; }

	virtual glm::vec2 GetPosition() const = 0;
	virtual float GetRadius() const = 0;
	virtual glm::vec4 GetColor() const = 0;
};

class IRectModel : public virtual IRenderableModel
{
public:
	virtual ~IRectModel() = default;
	virtual ERenderType GetRenderType() const override { return ERenderType::RECTANGLE; }

	virtual glm::vec2 GetPosition() const = 0;
	virtual glm::vec2 GetSize() const = 0;
	virtual float GetRotate() const = 0;
	virtual glm::vec4 GetColor() const = 0;
};

class ICapsuleModel : public virtual IRenderableModel
{
public:
	virtual ~ICapsuleModel() = default;
	virtual ERenderType GetRenderType() const override { return ERenderType::CAPSULE; }

	virtual glm::vec2 GetPosition() const = 0;
	virtual float GetRadius() const = 0;
	virtual float GetHeight() const = 0;
	virtual float GetRotate() const = 0;
	virtual glm::vec4 GetColor() const = 0;
};
