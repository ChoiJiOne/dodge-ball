#pragma once

#include <cstdint>
#include <string>

namespace DEF
{
	// Const
	constexpr int32_t INVALID_IDX = -1;
	constexpr float ANGLE_90_DEG = 90.0f;

	// PlayerActor
	constexpr int32_t PLAYER_ACTOR_MOVE_BOUND_MODEL_ORDER = 1;
	constexpr int32_t PLAYER_ACTOR_MODEL_ORDER = 2;
	constexpr int32_t PLAYER_ACTOR_CONTROLLER_ORDER = 1;

	// EnemyActor
	constexpr int32_t ENEMY_ACTOR_MODEL_ORDER = 1;
	constexpr int32_t ENEMY_ACTOR_CONTROLLER_ORDER = 1;

	// EnemySpawnActor
	constexpr int32_t ENEMY_SPAWN_ACTOR_MODEL_ORDER = 1;
	constexpr int32_t ENEMY_SPAWN_ACTOR_CONTROLLER_ORDER = 1;
	
	// Scene
	constexpr int32_t SCENE_TITLE_ACTOR_ORDER = 1;
	constexpr int32_t SCENE_PLAYER_ACTOR_ORDER = 1;
	constexpr int32_t SCENE_ENEMY_ACTOR_ORDER = 2;
	constexpr int32_t SCENE_ENEMY_SPAWN_ACTOR_ORDER = 3;
	constexpr int32_t SCENE_TAB_TEXT_ACTOR_ORDER = 3;
	constexpr int32_t SCENE_PARTICLE_ACTOR_ORDER = 4;
	constexpr int32_t SCENE_GAME_OVER_ACTOR_ORDER = 5;
	constexpr int32_t SCENE_GAME_RESULT_ACTOR_ORDER = 6;
	constexpr int32_t SCENE_PLAY_TIME_ACTOR_ORDER = 7;

	// Player
	constexpr int32_t PLAYER_START_LEVEL = 1;

	// ActorKey
	const static std::string TITLE_ACTOR_NAME = "TitleActor";
	const static std::string PLAYER_ACTOR_NAME = "Player";
	const static std::string ENEMY_SPAWN_ACTOR_NAME = "EnemySpawnActor";
	const static std::string GAME_OVER_ACTOR_NAME = "GameOverActor";
	const static std::string GAME_RESULT_ACTOR_NAME = "GameResultActor";
	const static std::string PLAY_TIME_ACTOR_NAME = "PlayTimeActor";
	const static std::string PARTICLE_ACTOR_NAME = "PARTICLE_ACTOR";
	const static std::string TAB_TEXT_ACTOR_KEY_PREFIX = "TAB_TEXT_ACTOR";
}