#pragma once

#include "Actor/IActorModel.h"
#include "Render/IRenderableModel.h"

enum class ETabTextState
{
	IDLE = 0x00,
	ACTIVE = 0x01,
	DEAD = 0x02,
};

class TabTextModel : public IActorModel, public ITextModel
{
public:
	TabTextModel() = default;
	virtual ~TabTextModel() = default;

	DISALLOW_COPY_AND_ASSIGN(TabTextModel);

	virtual const std::string& GetText() const override { return _text; }
	virtual glm::vec2 GetPosition() const override { return _position; }
	virtual float GetFontSize() const override { return _fontSize; }
	virtual glm::vec4 GetColor() const override { return _color; }

	ETabTextState GetState() const { return _state; }
	float GetInitialLifeTime() const { return _initialLifeTime; }
	float GetRemainingLifeTime() const { return _remainingLifeTime; }
	float GetMoveSpeed() const { return _moveSpeed; }

	void SetText(const std::string& text) { _text = text; }
	void SetPosition(const glm::vec2& pos) { _position = pos; }
	void SetFontSize(float fontSize) { _fontSize = fontSize; }
	void SetColor(const glm::vec4& color) { _color = color; }
	void SetState(ETabTextState state) { _state = state; }
	void SetInitialLifeTime(float lifeTime) 
	{ 
		_initialLifeTime = lifeTime; 
		_remainingLifeTime = lifeTime;
	}
	void SetRemainingLifeTime(float lifeTime){ _remainingLifeTime = lifeTime; }
	void SetMoveSpeed(float speed) { _moveSpeed = speed; }

private:
	std::string _text;
	glm::vec2 _position = glm::vec2(0.0f);
	glm::vec4 _color = glm::vec4(0.0f);
	float _fontSize = 0.0f;

	ETabTextState _state = ETabTextState::IDLE;
	float _initialLifeTime = 0.0f;
	float _remainingLifeTime = 0.0f;
	float _moveSpeed = 0.0f;
};
