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
	
	// CHECKME: 이거 위에 실패해도 무조건 true 반환하는데, 이거 맞는지 확인 필요...!
	return true;
}
