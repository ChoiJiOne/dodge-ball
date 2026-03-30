#pragma once

#include "Actor/IActor.h"

struct TextActorParam
{
	std::string text;
	glm::vec2 position;
	glm::vec4 color;
	float fontSize;
};

class TextActor : public IActor
{
public:
	TextActor(const TextActorParam& param);
	TextActor(const std::string& text, const glm::vec2& position, const glm::vec4& color, float fontSize);
	virtual ~TextActor();

	DISALLOW_COPY_AND_ASSIGN(TextActor);

	virtual void Release() override;

private:
	static const int32_t TEXT_ACTOR_MODEL_ORDER = 1;
};