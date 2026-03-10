#pragma once

#include "Actor/IActorModel.h"
#include "Render/IRenderableModel.h"

class BallModel : public IActorModel, public ICircleModel
{
public:
	BallModel();
	virtual ~BallModel() = default;

	DISALLOW_COPY_AND_ASSIGN(BallModel);

	virtual glm::vec2 GetPosition() const override { return _position; }
	virtual glm::vec4 GetColor() const override { return _color; }
	virtual float GetRadius() const override { return _radius; }

	float GetMoveSpeed() const { return _moveSpeed; }
	glm::vec2 GetMoveDirection() const { return _moveDirection; }

	void SetPosition(const glm::vec2& position) { _position = position; }
	void SetColor(const glm::vec4& color) { _color = color; }
	void SetRadius(float radius) { _radius = radius; }

	void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }
	void SetMoveDirection(const glm::vec2& moveDirection) { _moveDirection = moveDirection; }

private:
	glm::vec2 _position;
	glm::vec4 _color;
	float _radius;

	float _moveSpeed = 0.0f;
	glm::vec2 _moveDirection;
};