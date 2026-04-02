#pragma once

#include <glm/glm.hpp>

#include "Actor/IActorModel.h"
#include "Render/IRenderableModel.h"

class TitleTextModel : public IActorModel, public ITextModel
{
public:
	TitleTextModel() = default;
	virtual ~TitleTextModel() = default;

	DISALLOW_COPY_AND_ASSIGN(TitleTextModel);

	virtual const std::string& GetText() const override { return _text; }
	virtual glm::vec2 GetPosition() const override { return _position; }
	virtual float GetFontSize() const override { return _fontSize; }
	virtual glm::vec4 GetColor() const override { return _color; }

	void SetText(const std::string& text) { _text = text; }
	void SetPosition(const glm::vec2& position) { _position = position; }
	void SetFontSize(float fontSize) { _fontSize = fontSize; }
	void SetColor(const glm::vec4& color) { _color = color; }

private:
	std::string _text;
	glm::vec2 _position = glm::vec2(0.0f);
	glm::vec4 _color = glm::vec4(1.0f);
	float _fontSize = 24.0f;
};
