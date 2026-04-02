#pragma once

#include "Scene/IScene.h"

class GameScene : public IScene
{
public:
	virtual Result<void> OnEnter() override;
	virtual Result<void> OnExit() override;
};