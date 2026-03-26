#pragma once

#include <glm/glm.hpp>

#include "Macro/Macro.h"

class CollidableModel
{
public:
	CollidableModel() = default;
	virtual ~CollidableModel() = default;

	DISALLOW_COPY_AND_ASSIGN(CollidableModel);

	const glm::vec2& GetBoundCenter() const { return _boundCenter; }
	const float& GetBoundRadius() const { return _boundRadius; }

	void SetBoundCenter(const glm::vec2& boundCenter) { _boundCenter = boundCenter; }
	void SetBoundRadius(float boundRadius) { _boundRadius = boundRadius; }

	bool IsCollision(const CollidableModel* otherModel);

private:
	glm::vec2 _boundCenter;
	float _boundRadius = 0.0f;
};