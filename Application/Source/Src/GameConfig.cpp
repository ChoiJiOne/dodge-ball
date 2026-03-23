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
	
	// CHECKME: 이거 위에 실패해도 무조건 true 반환하는데, 이거 맞는지 확인 필요...!
	return true;
}
