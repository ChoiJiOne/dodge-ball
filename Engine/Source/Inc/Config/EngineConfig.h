#pragma once

#include "Config/IConfig.h"

class EngineConfig : public IConfig
{
public:
	EngineConfig() = default;
	virtual ~EngineConfig() = default;

	const int32_t& GetWindowWidth() const { return _windowWidth; }
	const int32_t& GetWindowHeight() const { return _windowHeight; }
	const std::string& GetWindowTitle() const { return _windowTitle; }
	const int32_t& GetTargetFPS() const { return _targetFPS; }

	virtual bool TryParse(const YAML::Node& node) override;

private:
	int32_t _windowWidth = 800;
	int32_t _windowHeight = 600;
	std::string _windowTitle;
	int32_t _targetFPS = 60;
};