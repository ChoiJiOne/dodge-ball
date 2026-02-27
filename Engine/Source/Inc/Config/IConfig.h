#pragma once

#include <yaml-cpp/yaml.h>

#include "Macro/Macro.h"

class IConfig
{
public:
	IConfig() = default;
	virtual ~IConfig() = default;

	DISALLOW_COPY_AND_ASSIGN(IConfig);

	virtual bool TryParse(const YAML::Node& node) = 0;

protected:
	template <typename T>
	bool TryGetValue(const YAML::Node& node, const std::string& key, T& outValue)
	{
		if (!node[key].IsDefined())
			return false;

		outValue = node[key].as<T>();
		return true;
	}
};