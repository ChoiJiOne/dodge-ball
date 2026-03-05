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
	Result<void> LoadConfig(const std::string& filePath)
	{
		std::string key = typeid(TConfig).name();
		const auto& iter = _configMap.find(key);
		if (iter != _configMap.end())
		{
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::ALREADY_LOAD_CONFIG,
				std::format("ALREADY_LOAD_CONFIG:{0}", filePath)
			));
		}

		// NOTE: yaml-cpp에서 예외 던져서 여기서는 try-catch 사용)
		try
		{
			std::unique_ptr<IConfig> config = std::make_unique<TConfig>();

			YAML::Node node = YAML::LoadFile(filePath);
			if (!config->TryParse(node))
			{
				return Result<void>::Fail(MAKE_ERROR(
					EErrorCode::FAILED_TO_PARSE_CONFIG,
					std::format("FAILED_TO_PARSE_CONFIG:{0}", filePath)
				));
			}

			_configMap.emplace(key, std::move(config));
		}
		catch (const YAML::Exception& e)
		{
			return Result<void>::Fail(MAKE_ERROR(
				EErrorCode::FAILED_TO_LOAD_CONFIG,
				std::format("FAILED_TO_LOAD_CONFIG:{0}", filePath)
			));
		}

		return Result<void>::Success();
	}

	template <typename TConfig>
	Result<const TConfig*> GetConfig()
	{
		std::string key = typeid(TConfig).name();
		auto iter = _configMap.find(key);
		if (iter == _configMap.end())
		{
			return Result<const TConfig*>::Fail(MAKE_ERROR(
				EErrorCode::NOT_FOUND_CONFIG,
				std::format("NOT_FOUND_CONFIG:{0}", key)
			));
		}

		const TConfig* configPtr = reinterpret_cast<const TConfig*>(iter->second.get());
		return Result<const TConfig*>::Success(configPtr);
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