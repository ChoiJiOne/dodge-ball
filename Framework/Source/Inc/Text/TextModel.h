#pragma once

#include "Actor/IActorModel.h"
#include "Render/IRenderableModel.h"

class TextModel : public IActorModel, public ITextModel
{
public:
	TextModel() = default;
	TextModel(const std::string& text, const glm::vec2& position, const glm::vec4& color, float fontSize)
		: _text(text)
		, _position(position)
		, _color(color)
		, _fontSize(fontSize) {}
	virtual ~TextModel() = default;

	DISALLOW_COPY_AND_ASSIGN(TextModel);

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
	glm::vec2 _position;
	glm::vec4 _color;
	float _fontSize = 0.0f;
};