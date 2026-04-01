#pragma once

#include <glm/glm.hpp>

#include "Config/IConfig.h"

class GameConfig : public IConfig
{
public:
	GameConfig() = default;
	~GameConfig() = default;

	const int32_t& GetSpawnRangeMinX() const { return _spawnRangeMinX; }
	const int32_t& GetSpawnRangeMaxX() const { return _spawnRangeMaxX; }
	const int32_t& GetSpawnRangeY() const { return _spawnRangeY; }

	const float& GetPlayerRadius() const { return _playerRadius; }
	const int32_t& GetPlayerMoveRangeMinX() const { return _playerMoveRangeMinX; }
	const int32_t& GetPlayerMoveRangeMaxX() const { return _playerMoveRangeMaxX; }
	const int32_t& GetPlayerMoveRangeY() const { return _playerMoveRangeY; }

	bool IsPlayerStartMovePositive() const { return _isPlayerStartMovePositive; }

	const int32_t& GetEnemySize() const { return _enemySize; }
	const int32_t& GetEnemyDeadZoneY() const { return _enemyDeadZoneY; }
	const float& GetEnemyFadeOutTime() const { return _enemyFadeOutTime; }

	const int32_t& GetParticleCount() const { return _particleCount; }
	const float& GetParticleMinSize() const { return _particleMinSize; }
	const float& GetParticleMaxSize() const { return _particleMaxSize; }
	const float& GetParticleMinSpeed() const { return _particleMinSpeed; }
	const float& GetParticleMaxSpeed() const { return _particleMaxSpeed; }
	const float& GetParticleLifeTime() const { return _particleLifeTime; }

	const float& GetTabTextMoveSpeed() const { return _tabTextMoveSpeed; }
	const float& GetTabTextLifeTime() const { return _tabTextLifeTime; }
	const float& GetTabTextFontSize() const { return _tabTextFontSize; }
	const float& GetTabTextOffsetY() const { return _tabTextOffsetY; }

	const glm::vec2& GetGameOverTextPosition() const { return _gameOverTextPosition; }
	const glm::vec4& GetGameOverTextColor() const { return _gameOverTextColor; }
	const float& GetGameOverTextFontSize() const { return _gameOverTextFontSize; }

	const glm::vec2& GetGameResultTextPosition() const { return _gameResultTextPosition; }
	const glm::vec4& GetGameResultTextColor() const { return _gameResultTextColor; }
	const float& GetGameResultTextFontSize() const { return _gameResultTextFontSize; }

	const glm::vec2& GetGameResultHintTextPosition() const { return _gameResultHintTextPosition; }
	const glm::vec4& GetGameResultHintTextColor() const { return _gameResultHintTextColor; }
	const float& GetGameResultHintTextFontSize() const { return _gameResultHintTextFontSize; }
	const float& GetGameResultHintTextBlinkSpeed() const { return _gameResultHintTextBlinkSpeed; }

	virtual bool TryParse(const YAML::Node& node) override;

private:
	int32_t _spawnRangeMinX = 0;
	int32_t _spawnRangeMaxX = 600;
	int32_t _spawnRangeY = -50;

	float _playerRadius = 15.0f;
	int32_t _playerMoveRangeMinX = 0;
	int32_t _playerMoveRangeMaxX = 600;
	int32_t _playerMoveRangeY = 650;
	
	bool _isPlayerStartMovePositive = true;

	int32_t _enemySize = 50;
	int32_t _enemyDeadZoneY = 700;
	float _enemyFadeOutTime = 1.0f;

	int32_t _particleCount = 200;
	float _particleMinSize = 5;
	float _particleMaxSize = 20;
	float _particleMinSpeed = 50;
	float _particleMaxSpeed = 200;
	float _particleLifeTime = 1.0f;

	float _tabTextMoveSpeed = 80.0f;
	float _tabTextLifeTime  = 1.0f;
	float _tabTextFontSize  = 24.0f;
	float _tabTextOffsetY   = 30.0f;

	glm::vec2 _gameOverTextPosition = glm::vec2(300.0f, 300.0f);
	glm::vec4 _gameOverTextColor = glm::vec4(1.0f, 0.5f, 0.1f, 1.0f);
	float _gameOverTextFontSize = 80.0f;

	glm::vec2 _gameResultTextPosition = glm::vec2(300.0f, 420.0f);
	glm::vec4 _gameResultTextColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float _gameResultTextFontSize = 32.0f;

	glm::vec2 _gameResultHintTextPosition = glm::vec2(300.0f, 500.0f);
	glm::vec4 _gameResultHintTextColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	float _gameResultHintTextFontSize = 24.0f;
	float _gameResultHintTextBlinkSpeed = 3.0f;
};