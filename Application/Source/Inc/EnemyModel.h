#pragma once

#include "Actor/IActorModel.h"
#include "Render/IRenderableModel.h"

enum class EEnemyState
{
	NONE = 0x00,
	MOVE = 0x01,
	FADE_OUT = 0x02,
	DEAD = 0x03,
};

class EnemyModel : public IActorModel, public IRectModel
{
public:
	EnemyModel() = default;

	virtual ~EnemyModel() = default;

	DISALLOW_COPY_AND_ASSIGN(EnemyModel);

	virtual glm::vec2 GetPosition() const override { return _position; }
	virtual glm::vec2 GetSize() const override { return _size; }
	virtual glm::vec4 GetColor() const override { return _color; }
	virtual float GetRotate() const override { return _rotate; }

	float GetMoveSpeed() const { return _moveSpeed; }
	float GetRotationSpeed() const { return _rotationSpeed; }
	glm::vec2 GetMoveDirection() const { return _moveDirection; }

	EEnemyState GetState() const { return _state; }

	void SetPosition(const glm::vec2& position) { _position = position; }
	void SetSize(const glm::vec2& size) { _size = size; }
	void SetColor(const glm::vec4& color) { _color = color; }
	void SetRotate(float rotate) { _rotate = rotate; }

	void SetMoveSpeed(float moveSpeed) { _moveSpeed = moveSpeed; }
	void SetRotationSpeed(float rotationSpeed) { _rotationSpeed = rotationSpeed; }
	void SetMoveDirection(const glm::vec2& moveDirection) { _moveDirection = moveDirection; }

	void SetState(EEnemyState state) { _state = state; }
		
private:
	glm::vec2 _position = glm::vec2(0.0f);
	glm::vec2 _size = glm::vec2(0.0f);
	glm::vec4 _color = glm::vec4(0.0f);
	float _rotate = 0.0f;

	float _moveSpeed = 0.0f;
	float _rotationSpeed = 0.0f;
	glm::vec2 _moveDirection = glm::vec2(0.0f);

	EEnemyState _state;
};