#pragma once

#include <glm/glm.hpp>

class Particle
{
public:
	Particle() = default;
	Particle(const glm::vec2& position, const glm::vec2& direction, const glm::vec4& color, float initialLifeTime, float size, float speed)
		: _position(position)
		, _direction(direction)
		, _color(color)
		, _initialLifeTime(initialLifeTime)
		, _remainingLifeTime(initialLifeTime)
		, _size(size) 
		, _speed(speed) {}
	Particle(Particle&& instance) noexcept = default;
	Particle(const Particle& instance) noexcept = default;

	Particle& operator=(Particle&& instance) noexcept = default;
	Particle& operator=(const Particle& instance) noexcept = default;

	const glm::vec2& GetPosition() const { return _position; }
	const glm::vec2& GetDirection() const { return _direction; }
	const glm::vec4& GetColor() const { return _color; }
	float GetInitialLifeTime() const { return _initialLifeTime; }
	float GetRemainingLifeTime() const { return _remainingLifeTime; }
	float GetSize() const { return _size; }
	float GetSpeed() const { return _speed; }

	void SetPosition(const glm::vec2& position) { _position = position; }
	void SetDirection(const glm::vec2& direction) { _direction = direction; }
	void SetColor(const glm::vec4& color) { _color = color; }
	void SetRemainingLifeTime(float lifeTime) { _remainingLifeTime = lifeTime; }
	void SetSize(float size) { _size = size; }
	void SetSpeed(float speed) { _speed = speed; }

	bool IsAlive() const { return _remainingLifeTime > 0.0f; }

private:
	glm::vec2 _position;
	glm::vec2 _direction;
	glm::vec4 _color;
	float _initialLifeTime = 0.0f;
	float _remainingLifeTime = 0.0f;
	float _size = 0.0f;
	float _speed = 0.0f;
};