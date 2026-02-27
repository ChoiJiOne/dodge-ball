#include "Config/EngineConfig.h"
#include "Macro/Macro.h"
#include "Utils/LogUtils.h"

bool EngineConfig::TryParse(const YAML::Node& node)
{
	if (!TryGetValue(node, "WindowWidth", _windowWidth))
		LOG_E("FAILED_TO_GET_ENGINE_CONFIG_VALUE(name:{0})", "WindowWidth");

	if (!TryGetValue(node, "WindowHeight", _windowHeight))
		LOG_E("FAILED_TO_GET_ENGINE_CONFIG_VALUE(name:{0})", "WindowHeight");

	if (!TryGetValue(node, "WindowTitle", _windowTitle))
		LOG_E("FAILED_TO_GET_ENGINE_CONFIG_VALUE(name:{0})", "WindowTitle");

	if (!TryGetValue(node, "TargetFPS", _targetFPS))
		LOG_E("FAILED_TO_GET_ENGINE_CONFIG_VALUE(name:{0})", "TargetFPS");

	// CHECKME: 이거 위에 실패해도 무조건 true 반환하는데, 이거 맞는지 확인 필요...!
	return true;
}