#pragma once

#include <glm/glm.hpp>

class Particle
{
public:
	Particle() = default;
	Particle(const glm::vec2& position, const glm::vec2& velocity, const glm::vec4& color, float initialLifeTime, float size)
		: _position(position)
		, _velocity(velocity)
		, _color(color)
		, _initialLifeTime(initialLifeTime)
		, _remainingLifeTime(initialLifeTime)
		, _size(size) {}
	Particle(Particle&& instance) noexcept = default;
	Particle(const Particle& instance) noexcept = default;

	Particle& operator=(Particle&& instance) noexcept = default;
	Particle& operator=(const Particle& instance) noexcept = default;

	const glm::vec2& GetPosition() const { return _position; }
	const glm::vec2& GetVelocity() const { return _velocity; }
	const glm::vec4& GetColor() const { return _color; }
	float GetInitialLifeTime() const { return _initialLifeTime; }
	float GetRemainingLifeTime() const { return _remainingLifeTime; }
	float GetSize() const { return _size; }

	void SetPosition(const glm::vec2& position) { _position = position; }
	void SetVelocity(const glm::vec2& velocity) { _velocity = velocity; }
	void SetColor(const glm::vec4& color) { _color = color; }
	void SetRemainingLifeTime(float lifeTime) { _remainingLifeTime = lifeTime; }
	void SetSize(float size) { _size = size; }

	bool IsAlive() const { return _remainingLifeTime > 0.0f; }

private:
	glm::vec2 _position;
	glm::vec2 _velocity;
	glm::vec4 _color;
	float _initialLifeTime = 0.0f;
	float _remainingLifeTime = 0.0f;
	float _size = 0.0f;
};