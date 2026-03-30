#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Actor/IActorController.h"
#include "Manager/InputManager.h"

class InputManager;
class PlayerContext;
class PlayerModel;
class TabTextModel;

class PlayerActorController : public IActorController
{
public:
	PlayerActorController() = default;
	virtual ~PlayerActorController() = default;

	DISALLOW_COPY_AND_ASSIGN(PlayerActorController);

	virtual void OnInitialize(IActor* owner) override;
	virtual void OnRelease() override;
	virtual void OnTick(float deltaSeconds) override;
	virtual void OnCollision(IActor* actor) override;

private:
	Result<void> InitializeContext();
	Result<void> InitializeModel();
	Result<void> InitializeModelFromConfig();
	Result<void> InitializeMoveBoundModel();

	void UpdateContext(float deltaSeconds);
	void UpdateMoveDirection();
	void Move(float deltaSeconds);
	void UpdateDirectionByBounds();
	void GenerateParticleEffect();

	void GenerateTabTextEffect();
	TabTextModel* FindAvailableTabTextModel() const;
	TabTextModel* CreateAndRegisterTabText();
	void ActivateTabTextModel(TabTextModel* model);

private:
	InputManager* _inputMgr = nullptr;
	PlayerContext* _context = nullptr;
	PlayerModel* _model = nullptr;

	float _moveRangeMinX = 0.0f;
	float _moveRangeMaxX = 0.0f;

	std::vector<TabTextModel*> _tabTextModelPool;

	float _tabTextMoveSpeed = 0.0f;
	float _tabTextLifeTime  = 0.0f;
	float _tabTextFontSize  = 0.0f;
	float _tabTextOffsetY   = 0.0f;
};