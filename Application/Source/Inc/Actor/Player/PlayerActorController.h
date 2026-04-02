#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "Actor/IActorController.h"
#include "Manager/InputManager.h"

class InputManager;
class PlayerContext;
class PlayerModel;
class EffectTextModel;

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
	Result<void> InitializeMoveSpeed();

	void UpdateContext(float deltaSeconds);
	void UpdateMoveDirection();
	void Move(float deltaSeconds);
	void UpdateDirectionByBounds();
	void GenerateParticleEffect();

	void GenerateTabTextEffect();
	void GenerateLevelUpTextEffect();
	EffectTextModel* FindAvailableEffectTextModel() const;
	EffectTextModel* CreateAndRegisterEffectText();
	void ActivateEffectTextModel(EffectTextModel* model, const std::string& text, float fontSize, const glm::vec4& color, float moveSpeed, float lifetime, float offsetY);

	void UpdateMoveSpeed(int32_t level);

private:
	InputManager* _inputMgr = nullptr;
	PlayerContext* _context = nullptr;
	PlayerModel* _model = nullptr;

	float _moveRangeMinX = 0.0f;
	float _moveRangeMaxX = 0.0f;

	std::vector<EffectTextModel*> _effectTextModelPool;

	float _tabTextMoveSpeed = 0.0f;
	float _tabTextLifeTime = 0.0f;
	float _tabTextFontSize = 0.0f;
	float _tabTextOffsetY = 0.0f;
	glm::vec4 _tabTextColor = glm::vec4(0.0f);

	float _levelUpTextMoveSpeed = 0.0f;
	float _levelUpTextLifeTime = 0.0f;
	float _levelUpTextFontSize = 0.0f;
	float _levelUpTextOffsetY = 0.0f;
	glm::vec4 _levelUpTextColor = glm::vec4(0.0f);
};
