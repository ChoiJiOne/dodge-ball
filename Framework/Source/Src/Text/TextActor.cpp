#include "Text/TextActor.h"
#include "Text/TextModel.h"

TextActor::TextActor(const TextActorParam& param)
{
	AddModel<TextModel>(TEXT_ACTOR_MODEL_ORDER, param.text, param.position, param.color, param.fontSize);
}

TextActor::TextActor(const std::string& text, const glm::vec2& position, const glm::vec4& color, float fontSize)
{
	AddModel<TextModel>(TEXT_ACTOR_MODEL_ORDER, text, position, color, fontSize);
}

TextActor::~TextActor()
{
	if (_isInitialized)
	{
		Release();
	}
}

void TextActor::Release()
{
	IActor::Release();

	_isInitialized = false;
}