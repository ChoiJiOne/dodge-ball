#include "Particle/ParticleModel.h"
#include "Utils/MathUtils.h"

ParticleModel::ParticleModel(
	const glm::vec2& startPosition,
	int32_t count,
	float minSize,
	float maxSize,
	float minSpeed,
	float maxSpeed,
	float lifeTime,
	const glm::vec4& color
	)
	: _startPosition(startPosition)
	, _minSize(minSize)
	, _maxSize(maxSize)
	, _minSpeed(minSpeed)
	, _maxSpeed(maxSpeed)
	, _lifeTime(lifeTime)
	, _color(color)
{
	_particles = GenerateParticles(count);
}

std::vector<Particle> ParticleModel::GenerateParticles(int32_t count) const
{
	std::vector<Particle> particles;
	particles.reserve(count);

	for (int32_t idx = 0; idx < count; ++idx)
	{
		float directionX = MathUtils::GenerateRandomFloat(-1.0f, 1.0f);
		float directionY = MathUtils::GenerateRandomFloat(-1.0f, 1.0f);
		float size = MathUtils::GenerateRandomFloat(_minSize, _maxSize);
		float speed = MathUtils::GenerateRandomFloat(_minSpeed, _maxSpeed);
		glm::vec2 direction = glm::normalize(glm::vec2(directionX, directionY));

		particles.emplace_back(_startPosition, direction, _color, _lifeTime, size, speed);
	}

	return particles;
}
