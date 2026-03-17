#pragma once

#include "Actor/IActorModel.h"
#include "Render/IRenderableModel.h"

class MoveBoundModel : public IActorModel, public ICapsuleModel
{
public:
	MoveBoundModel() = default;
	MoveBoundModel(
		const glm::vec2& position,
		float radius,
		float height,
		float rotate,
		const glm::vec4& color
	) : _position(position),
		_radius(radius),
		_height(height),
		_rotate(rotate),
		_color(color)
	{}

	virtual ~MoveBoundModel() = default;

	DISALLOW_COPY_AND_ASSIGN(MoveBoundModel);

	virtual glm::vec2 GetPosition() const override { return _position; }
	virtual float GetRadius() const override { return _radius; }
	virtual float GetHeight() const override { return _height; }
	virtual float GetRotate() const override { return _rotate; }
	virtual glm::vec4 GetColor() const override { return _color; }

	void SetPosition(const glm::vec2& position) { _position = position; }
	void SetRadius(float radius) { _radius = radius; }
	void SetHeight(float height) { _height = height; }
	void SetRotate(float rotate) { _rotate = rotate; }
	void SetColor(const glm::vec4& color) { _color = color; }

private:
	glm::vec2 _position;
	float _radius = 0.0f;
	float _height = 0.0f;
	float _rotate = 0.0f;
	glm::vec4 _color;
};