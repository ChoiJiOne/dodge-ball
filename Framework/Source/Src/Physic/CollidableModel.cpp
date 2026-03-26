#include <raylib.h>

#include "Physic/CollidableModel.h"
#include "Utils/MathUtils.h"

bool CollidableModel::IsCollision(const CollidableModel* otherModel)
{
	Vector2 center1 = MathUtils::ToVector2(_boundCenter);
	float radius1 = _boundRadius;

	Vector2 center2 = MathUtils::ToVector2(otherModel->_boundCenter);
	float radius2 = otherModel->_boundRadius;

	return CheckCollisionCircles(center1, radius1, center2, radius2);
}