#include "GameConfig.h"
#include "Macro/Macro.h"
#include "Utils/LogUtils.h"

bool GameConfig::TryParse(const YAML::Node& node)
{
	if (!TryGetValue(node, "SpawnRangeMinX", _spawnRangeMinX))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "SpawnRangeMinX");
	}

	if (!TryGetValue(node, "SpawnRangeMaxX", _spawnRangeMaxX))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "SpawnRangeMaxX");
	}

	if (!TryGetValue(node, "SpawnRangeY", _spawnRangeY))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "SpawnRangeY");
	}

	if (!TryGetValue(node, "PlayerRadius", _playerRadius))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayerRadius");
	}

	if (!TryGetValue(node, "PlayerMoveRangeMinX", _playerMoveRangeMinX))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayerMoveRangeMinX");
	}

	if (!TryGetValue(node, "PlayerMoveRangeMaxX", _playerMoveRangeMaxX))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayerMoveRangeMaxX");
	}

	if (!TryGetValue(node, "PlayerMoveRangeY", _playerMoveRangeY))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayerMoveRangeY");
	}

	if (!TryGetValue(node, "IsPlayerStartMovePositive", _isPlayerStartMovePositive))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "IsPlayerStartMovePositive");
	}

	if (!TryGetValue(node, "EnemySize", _enemySize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "EnemySize");
	}

	if (!TryGetValue(node, "EnemyDeadZoneY", _enemyDeadZoneY))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "EnemyDeadZoneY");
	}

	if (!TryGetValue(node, "EnemyFadeOutTime", _enemyFadeOutTime))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "EnemyFadeOutTime");
	}

	if (!TryGetValue(node, "ParticleCount", _particleCount))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "ParticleCount");
	}

	if (!TryGetValue(node, "ParticleMinSize", _particleMinSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "ParticleMinSize");
	}

	if (!TryGetValue(node, "ParticleMaxSize", _particleMaxSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "ParticleMaxSize");
	}

	if (!TryGetValue(node, "ParticleMinSpeed", _particleMinSpeed))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "ParticleMinSpeed");
	}

	if (!TryGetValue(node, "ParticleMaxSpeed", _particleMaxSpeed))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "ParticleMaxSpeed");
	}

	if (!TryGetValue(node, "ParticleLifeTime", _particleLifeTime))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "ParticleLifeTime");
	}

	// CHECKME: 이거 위에 실패해도 무조건 true 반환하는데, 이거 맞는지 확인 필요...!
	return true;
}
