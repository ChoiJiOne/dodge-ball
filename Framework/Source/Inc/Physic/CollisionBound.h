#pragma once

#include <glm/glm.hpp>

#include "Macro/Macro.h"

// TODO: 나중에 프레임워크에 물리 엔진 관련 처리 구현 시 이거 수정 필요. 
// 일단 필요해서 땜빵(?)으로 넣음 (이게 최선일까...?)
class CollisionBound
{
public:
	CollisionBound() = default;
	virtual ~CollisionBound() = default;

	DISALLOW_COPY_AND_ASSIGN(CollisionBound);

	const glm::vec2& GetBoundCenter() const { return _boundCenter; }
	const float& GetBoundRadius() const { return _boundRadius; }

	void SetBoundCenter(const glm::vec2& boundCenter) { _boundCenter = boundCenter; }
	void SetBoundRadius(float boundRadius) { _boundRadius = boundRadius; }

	bool IsCollision(const CollisionBound* otherBound);

private:
	glm::vec2 _boundCenter;
	float _boundRadius = 0.0f;
};