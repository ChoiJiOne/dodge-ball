#pragma once

#include <map>
#include <memory>

#include "Config/IConfig.h"
#include "Manager/IManager.h"
#include "Utils/LogUtils.h"

class ConfigManager : public IManager<ConfigManager>
{
public:
	DISALLOW_COPY_AND_ASSIGN(ConfigManager);

	virtual Result<void> Startup() override;
	virtual Result<void> Shutdown() override;

	template <typename TConfig>
	TConfig* Create(const std::string& path, const std::string& key)
	{
		const auto& iter = _configMap.find(key);
		if (iter != _configMap.end())
		{
			LOG_E("ALREADY_EXIST_CONFIG(path:{0}, key:{1})", path, key);
			return reinterpret_cast<TConfig*>(iter->second.get());
		}

		_configMap.insert({ key, std::make_unique<TConfig>() });

		IConfig* config = _configMap[key].get();
		if (!LoadConfigFromFile(path, config))
		{
			LOG_E("FAILED_TO_LOAD_CONFIG_FROM_FILE(path:{0}, key:{1})", path, key);
			return nullptr;
		}

		return reinterpret_cast<TConfig*>(config);
	}

	void Destroy(const std::string& key);

	template <typename TConfig>
	TConfig* GetConfig(const std::string& key)
	{
		return nullptr;
	}

private:
	bool LoadConfigFromFile(const std::string& filePath, IConfig* outConfig);

private:
	friend class IManager<ConfigManager>;

	ConfigManager() = default;
	virtual ~ConfigManager() = default;

private:
	std::map<std::string, std::unique_ptr<IConfig>> _configMap;
};