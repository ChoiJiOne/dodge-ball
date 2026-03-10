#include "BallModel.h"

BallModel::BallModel()
{
	_position = glm::vec2(300.0f, 400.0f);
	_radius = 30.0f;
	_color = glm::vec4(1.0f, 0.0f, 0.5f, 1.0f);

	_moveSpeed = 100.0f;
	_moveDirection = glm::vec2(0.0f, 0.0f);
}