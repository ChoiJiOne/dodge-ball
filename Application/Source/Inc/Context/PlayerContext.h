#pragma once

#include "Context/IContext.h"
#include "Event/Event.h"
#include "Macro/Macro.h"

#include "App/AppDef.h"

class PlayerContext : public IContext
{
public:
	PlayerContext();
	virtual ~PlayerContext() = default;

	DISALLOW_COPY_AND_ASSIGN(PlayerContext);

	Event<>& GetGameOverEvent() { return _gameOverEvent; }
	Event<int32_t> GetLevelUpEvent() { return _levelUpEvent; }

	float GetCurrentPlayTime() const { return _currentPlayTime; }
	float GetBestPlayTime() const { return _bestPlayTime; }
	int32_t GetLevel() const { return _level; }
	bool IsGameOver() const { return _isGameOver; }

	void AddPlayTime(float deltaSeconds);
	void SetGameOver(bool isGameOver);
	void RecordBestTime();
	void Reset();

private:
	float _currentPlayTime = 0.0f;
	float _bestPlayTime = 0.0f;
	int32_t _level = DEF::PLAYER_START_LEVEL;
	int32_t _maxLevel = DEF::PLAYER_START_LEVEL;
	bool _isGameOver = false;

	Event<int32_t> _levelUpEvent;
	Event<> _gameOverEvent;
};
