#pragma once

#include <cstdint>

namespace DEF
{
	// Const
	constexpr int32_t INVALID_IDX = -1;

	// PlayerActor
	constexpr int32_t PLAYER_ACTOR_MOVE_BOUND_MODEL_ORDER = 1;
	constexpr int32_t PLAYER_ACTOR_BALL_MODEL_ORDER = 2;
	constexpr int32_t PLAYER_ACTOR_CONTROLLER_ORDER = 1;

	// EnemyActor
	constexpr int32_t ENEMY_AACTOR_MODEL_ORDER = 1;
	constexpr int32_t ENEMY_ACTOR_CONTROLLER_ORDER = 1;

	// EnemySpawnActor
	constexpr int32_t ENEMY_SPAWN_ACTOR_MODEL_ORDER = 1;
	constexpr int32_t ENEMY_SPAWN_ACTOR_CONTROLLER_ORDER = 1;

	// Scene
	constexpr int32_t SCENE_PLAYER_ACTOR_ORDER = 1; // CHECKME: 우선 순위가 여기에 박혀 있어야 할까...?
	constexpr int32_t SCENE_ENEMY_ACTOR_ORDER = 2;
	constexpr int32_t SCENE_ENEMY_SPAWN_ACTOR_ORDER = 3;
}