#pragma once

#include "Scene/IScene.h"

class TitleScene : public IScene
{
public:
	virtual Result<void> OnEnter() override;
	virtual Result<void> OnExit() override;
};
