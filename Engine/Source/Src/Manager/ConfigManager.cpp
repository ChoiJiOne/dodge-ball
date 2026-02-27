#include "Manager/ConfigManager.h"
#include "Utils/LogUtils.h"
#include "Macro/Macro.h"

Result<void> ConfigManager::Startup()
{
	if (_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::ALREADY_INITIALIZED, "FAILED_TO_STARTUP_RENDER_MANAGER"));

	_isInitialized = true;
	return Result<void>::Success();
}

Result<void> ConfigManager::Shutdown()
{
	if (!_isInitialized)
		return Result<void>::Fail(MAKE_ERROR(EErrorCode::NOT_INITIALIZED, "FAILED_TO_SHUTDOWN_RENDER_MANAGER"));

	_isInitialized = false;
	return Result<void>::Success();
}

void ConfigManager::Destroy(const std::string& key)
{

}

bool ConfigManager::LoadConfigFromFile(const std::string& filePath, IConfig* outConfig)
{
	try
	{
		YAML::Node node = YAML::LoadFile(filePath);
		return outConfig->TryParse(node);
	}
	catch (const YAML::Exception& e)
	{
		LOG_E("FAILED_TO_LOAD_CONFIG_FROM_FILE(path:{0})", filePath);
		return false;
	}
}
