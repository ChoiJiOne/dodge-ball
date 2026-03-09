#pragma once

#include "Actor/IActorModel.h"
#include "Render/IRenderableModel.h"

class PlayerBallModel : public IActorModel, public ICircleModel
{
public:
	PlayerBallModel();
	virtual ~PlayerBallModel();

	DISALLOW_COPY_AND_ASSIGN(PlayerBallModel);

	virtual glm::vec2 GetPosition() const override { return _position; }
	virtual float GetRadius() const override { return _radius; }
	virtual glm::vec4 GetColor() const override { return _color; }

	void SetPosition(const glm::vec2& position) { _position = position; }

private:
	glm::vec2 _position;
	float _radius;
	glm::vec4 _color;
};