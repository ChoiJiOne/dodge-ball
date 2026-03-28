#pragma once

#include <vector>

#include "Actor/IActorModel.h"
#include "Macro/Macro.h"
#include "Particle/Particle.h"
#include "Render/IRenderableModel.h"

class ParticleModel : public IActorModel, public IRenderableModel
{
public:
	ParticleModel(
		const glm::vec2& startPosition,
		int32_t count,
		float minSize,
		float maxSize,
		float minSpeed,
		float maxSpeed,
		float lifeTime,
		const glm::vec4& color
	);
	virtual ~ParticleModel() = default;

	DISALLOW_COPY_AND_ASSIGN(ParticleModel);

	virtual ERenderType GetRenderType() const override { return ERenderType::PARTICLE; }
	std::vector<Particle>& GetParticles() { return _particles; }
	const std::vector<Particle>& GetParticles() const { return _particles; }

private:
	std::vector<Particle> GenerateParticles(int32_t count) const;

private:
	glm::vec2 _startPosition;
	float _minSize = 0.0f;
	float _maxSize = 0.0f;
	float _minSpeed = 0.0f;
	float _maxSpeed = 0.0f;
	float _lifeTime = 0.0f;
	glm::vec4 _color;

	std::vector<Particle> _particles;
};