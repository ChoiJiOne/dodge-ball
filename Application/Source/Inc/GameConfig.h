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

	virtual bool TryParse(const YAML::Node& node) override;
	
private:
	int32_t _spawnRangeMinX = 0;
	int32_t _spawnRangeMaxX = 600;
	int32_t _spawnRangeY = -50;
};