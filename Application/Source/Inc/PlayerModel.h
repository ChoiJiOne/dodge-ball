#pragma once

#include "Actor/IActorModel.h"
#include "Physic/CollidableModel.h"
#include "Render/IRenderableModel.h"

class PlayerModel : public IActorModel, public ICircleModel, public CollidableModel
{
public:
	PlayerModel() = default;
	PlayerModel(
		const glm::vec2& position,
		const glm::vec4& color,
		float radius,
		float moveSpeed,
		const glm::vec2& moveDirection
	) : _position(position),
		_color(color),
		_radius(radius),
		_moveSpeed(moveSpeed),
		_moveDirection(moveDirection)
	{}

	virtual ~PlayerModel() = default;

	DISALLOW_COPY_AND_ASSIGN(PlayerModel);

	virtual glm::vec2 GetPosition() const override { return _position; }
	virtual glm::vec4 GetColor() const override { return _color; }
	virtual float GetRadius() const override { return _radius; }
	float GetMoveSpeed() const { return _moveSpeed; }
	glm::vec2 GetMoveDirection() const { return _moveDirection; }
	bool IsDead() const { return _isDead; }

	void SetPosition(const glm::vec2& position) 
	{ 
		_position = position;
		SetBoundCenter(_position);
	}

	void SetColor(const glm::vec4& color) { _color = color; }
	void SetRadius(float radius) 
	{ 
		_radius = radius; 
		SetBoundRadius(radius);
	}

	void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }
	void SetMoveDirection(const glm::vec2& moveDirection) { _moveDirection = moveDirection; }
	void SetDead(bool isDead) { _isDead = isDead; }

private:
	glm::vec2 _position = glm::vec2(0.0f);
	glm::vec4 _color = glm::vec4(0.0f);
	float _radius = 0.0f;

	float _moveSpeed = 0.0f;
	glm::vec2 _moveDirection = glm::vec2(0.0f);

	bool _isDead = false;
};