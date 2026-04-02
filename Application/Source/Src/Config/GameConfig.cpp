#include "Config/GameConfig.h"
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

	if (!TryGetValue(node, "TabTextMoveSpeed", _tabTextMoveSpeed))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TabTextMoveSpeed");
	}

	if (!TryGetValue(node, "TabTextLifeTime", _tabTextLifeTime))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TabTextLifeTime");
	}

	if (!TryGetValue(node, "TabTextFontSize", _tabTextFontSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TabTextFontSize");
	}

	if (!TryGetValue(node, "TabTextOffsetY", _tabTextOffsetY))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TabTextOffsetY");
	}

	YAML::Node gameOverPositionNode;
	if (TryGetNode(node, "GameOverTextPosition", gameOverPositionNode))
	{
		if (!TryGetValue(gameOverPositionNode, "x", _gameOverTextPosition.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameOverTextPosition.x");
		}
		
		if (!TryGetValue(gameOverPositionNode, "y", _gameOverTextPosition.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameOverTextPosition.y");
		}
	}
	
	YAML::Node gameOverColorNode;
	if (TryGetNode(node, "GameOverTextColor", gameOverColorNode))
	{
		if (!TryGetValue(gameOverColorNode, "r", _gameOverTextColor.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameOverTextColor.x");
		}

		if (!TryGetValue(gameOverColorNode, "g", _gameOverTextColor.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameOverTextColor.y");
		}

		if (!TryGetValue(gameOverColorNode, "b", _gameOverTextColor.z))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameOverTextColor.y");
		}

		if (!TryGetValue(gameOverColorNode, "a", _gameOverTextColor.w))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameOverTextColor.y");
		}
	}

	if (!TryGetValue(node, "GameOverTextFontSize", _gameOverTextFontSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameOverTextFontSize");
	}

	YAML::Node playerInfoPositionNode;
	if (TryGetNode(node, "GameResultTextPosition", playerInfoPositionNode))
	{
		if (!TryGetValue(playerInfoPositionNode, "x", _gameResultTextPosition.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultTextPosition.x");
		}

		if (!TryGetValue(playerInfoPositionNode, "y", _gameResultTextPosition.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultTextPosition.y");
		}
	}

	YAML::Node playerInfoColorNode;
	if (TryGetNode(node, "GameResultTextColor", playerInfoColorNode))
	{
		if (!TryGetValue(playerInfoColorNode, "r", _gameResultTextColor.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultTextColor.r");
		}

		if (!TryGetValue(playerInfoColorNode, "g", _gameResultTextColor.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultTextColor.g");
		}

		if (!TryGetValue(playerInfoColorNode, "b", _gameResultTextColor.z))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultTextColor.b");
		}

		if (!TryGetValue(playerInfoColorNode, "a", _gameResultTextColor.w))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultTextColor.a");
		}
	}

	if (!TryGetValue(node, "GameResultTextFontSize", _gameResultTextFontSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultTextFontSize");
	}

	YAML::Node gameResultHintPositionNode;
	if (TryGetNode(node, "GameResultHintTextPosition", gameResultHintPositionNode))
	{
		if (!TryGetValue(gameResultHintPositionNode, "x", _gameResultHintTextPosition.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultHintTextPosition.x");
		}

		if (!TryGetValue(gameResultHintPositionNode, "y", _gameResultHintTextPosition.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultHintTextPosition.y");
		}
	}

	YAML::Node gameResultHintColorNode;
	if (TryGetNode(node, "GameResultHintTextColor", gameResultHintColorNode))
	{
		if (!TryGetValue(gameResultHintColorNode, "r", _gameResultHintTextColor.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultHintTextColor.r");
		}

		if (!TryGetValue(gameResultHintColorNode, "g", _gameResultHintTextColor.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultHintTextColor.g");
		}

		if (!TryGetValue(gameResultHintColorNode, "b", _gameResultHintTextColor.z))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultHintTextColor.b");
		}

		if (!TryGetValue(gameResultHintColorNode, "a", _gameResultHintTextColor.w))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultHintTextColor.a");
		}
	}

	if (!TryGetValue(node, "GameResultHintTextFontSize", _gameResultHintTextFontSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultHintTextFontSize");
	}

	if (!TryGetValue(node, "GameResultHintTextBlinkSpeed", _gameResultHintTextBlinkSpeed))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "GameResultHintTextBlinkSpeed");
	}

	YAML::Node playTimePositionNode;
	if (TryGetNode(node, "PlayTimeTextPosition", playTimePositionNode))
	{
		if (!TryGetValue(playTimePositionNode, "x", _playTimeTextPosition.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayTimeTextPosition.x");
		}

		if (!TryGetValue(playTimePositionNode, "y", _playTimeTextPosition.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayTimeTextPosition.y");
		}
	}

	YAML::Node playTimeColorNode;
	if (TryGetNode(node, "PlayTimeTextColor", playTimeColorNode))
	{
		if (!TryGetValue(playTimeColorNode, "r", _playTimeTextColor.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayTimeTextColor.r");
		}

		if (!TryGetValue(playTimeColorNode, "g", _playTimeTextColor.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayTimeTextColor.g");
		}

		if (!TryGetValue(playTimeColorNode, "b", _playTimeTextColor.z))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayTimeTextColor.b");
		}

		if (!TryGetValue(playTimeColorNode, "a", _playTimeTextColor.w))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayTimeTextColor.a");
		}
	}

	if (!TryGetValue(node, "PlayTimeTextFontSize", _playTimeTextFontSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "PlayTimeTextFontSize");
	}

	YAML::Node titlePositionNode;
	if (TryGetNode(node, "TitleTextPosition", titlePositionNode))
	{
		if (!TryGetValue(titlePositionNode, "x", _titleTextPosition.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleTextPosition.x");
		}

		if (!TryGetValue(titlePositionNode, "y", _titleTextPosition.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleTextPosition.y");
		}
	}

	YAML::Node titleColorNode;
	if (TryGetNode(node, "TitleTextColor", titleColorNode))
	{
		if (!TryGetValue(titleColorNode, "r", _titleTextColor.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleTextColor.r");
		}

		if (!TryGetValue(titleColorNode, "g", _titleTextColor.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleTextColor.g");
		}

		if (!TryGetValue(titleColorNode, "b", _titleTextColor.z))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleTextColor.b");
		}

		if (!TryGetValue(titleColorNode, "a", _titleTextColor.w))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleTextColor.a");
		}
	}

	if (!TryGetValue(node, "TitleTextFontSize", _titleTextFontSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleTextFontSize");
	}

	YAML::Node titleHintPositionNode;
	if (TryGetNode(node, "TitleHintTextPosition", titleHintPositionNode))
	{
		if (!TryGetValue(titleHintPositionNode, "x", _titleHintTextPosition.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleHintTextPosition.x");
		}

		if (!TryGetValue(titleHintPositionNode, "y", _titleHintTextPosition.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleHintTextPosition.y");
		}
	}

	YAML::Node titleHintColorNode;
	if (TryGetNode(node, "TitleHintTextColor", titleHintColorNode))
	{
		if (!TryGetValue(titleHintColorNode, "r", _titleHintTextColor.x))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleHintTextColor.r");
		}

		if (!TryGetValue(titleHintColorNode, "g", _titleHintTextColor.y))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleHintTextColor.g");
		}

		if (!TryGetValue(titleHintColorNode, "b", _titleHintTextColor.z))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleHintTextColor.b");
		}

		if (!TryGetValue(titleHintColorNode, "a", _titleHintTextColor.w))
		{
			LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleHintTextColor.a");
		}
	}

	if (!TryGetValue(node, "TitleHintTextFontSize", _titleHintTextFontSize))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleHintTextFontSize");
	}

	if (!TryGetValue(node, "TitleHintTextBlinkSpeed", _titleHintTextBlinkSpeed))
	{
		LOG_E("FAILED_TO_GET_GAME_CONFIG_VALUE(name:{0})", "TitleHintTextBlinkSpeed");
	}

	return true;
}
