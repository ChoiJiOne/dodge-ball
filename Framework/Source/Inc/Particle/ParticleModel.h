#pragma once

#include "Render/IRenderableModel.h"

class ParticleModel : public IRenderableModel
{
public:
	ParticleModel() = default;
	virtual ~ParticleModel() = default;

	virtual ERenderType GetRenderType() const override { return ERenderType::PARTICLE; }

private:

};