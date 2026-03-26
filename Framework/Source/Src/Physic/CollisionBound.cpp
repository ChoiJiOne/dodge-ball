#include <raylib.h>

#include "Physic/CollisionBound.h"
#include "Utils/MathUtils.h"

bool CollisionBound::IsCollision(const CollisionBound* otherBound)
{
	Vector2 center1 = MathUtils::ToVector2(_boundCenter);
	float radius1 = _boundRadius;

	Vector2 center2 = MathUtils::ToVector2(otherBound->_boundCenter);
	float radius2 = otherBound->_boundRadius;

	return CheckCollisionCircles(center1, radius1, center2, radius2);
}