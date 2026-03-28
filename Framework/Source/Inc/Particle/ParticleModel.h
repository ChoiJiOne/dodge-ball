#pragma once

#include <vector>

#include "Actor/IActorModel.h"
#include "Macro/Macro.h"
#include "Particle/Particle.h"
#include "Render/IRenderableModel.h"

enum class EParticleEmitPattern
{
	NONE,
	RADIAL, // 360도 전방향으로 퍼짐
	DIRECTIONAL, // 특정 방향 중심으로 퍼짐 (cone 형태)
	BURST, // 균등한 n방향으로 퍼짐 (폭발)
};

class ParticleModel : public IActorModel, public IRenderableModel
{
public:
	ParticleModel() = default;
	virtual ~ParticleModel() = default;

	DISALLOW_COPY_AND_ASSIGN(ParticleModel);

	virtual ERenderType GetRenderType() const override { return ERenderType::PARTICLE; }

private:
	glm::vec2 _startPosition;
	int32_t _particleCount = 0;
	float _minSize = 0.0f;
	float _maxSize = 0.0f;
	EParticleEmitPattern _emitPattern;
	glm::vec2 _direction;

	std::vector<Particle> _particles;
};