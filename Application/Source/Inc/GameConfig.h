#pragma once

#include "Config/IConfig.h"

class GameConfig : public IConfig
{
public:
	GameConfig() = default;
	~GameConfig() = default;

	const int32_t& GetSpawnRangeMinX() const { return _spawnRangeMinX; }
	const int32_t& GetSpawnRangeMaxX() const { return _spawnRangeMaxX; }
	const int32_t& GetSpawnRangeY() const { return _spawnRangeY; }

	const int32_t& GetPlayerMoveRangeMinX() const { return _playerMoveRangeMinX; }
	const int32_t& GetPlayerMoveRangeMaxX() const { return _playerMoveRangeMaxX; }
	const int32_t& GetPlayerMoveRangeY() const { return _playerMoveRangeY; }

	bool IsPlayerStartMovePositive() const { return _isPlayerStartMovePositive; }

	const int32_t& GetEnemySize() const { return _enemySize; }
	const int32_t& GetEnemyDeadZoneY() const { return _enemyDeadZoneY; }
	const float& GetEnemyFadeOutTime() const { return _enemyFadeOutTime; }

	virtual bool TryParse(const YAML::Node& node) override;
	
private:
	// NOTE: 일단, GameConfig.yaml의 선언된 순서와 맞춤.
	int32_t _spawnRangeMinX = 0;
	int32_t _spawnRangeMaxX = 600;
	int32_t _spawnRangeY = -50;

	int32_t _playerMoveRangeMinX = 0;
	int32_t _playerMoveRangeMaxX = 600;
	int32_t _playerMoveRangeY = 650;
	
	bool _isPlayerStartMovePositive = true;

	int32_t _enemySize = 50;
	int32_t _enemyDeadZoneY = 700;
	float _enemyFadeOutTime = 1.0f;
};