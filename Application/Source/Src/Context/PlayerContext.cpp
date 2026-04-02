#include "Manager/DataChunkManager.h"

#include "Context/PlayerContext.h"

PlayerContext::PlayerContext()
{
}

void PlayerContext::AddPlayTime(float deltaSeconds)
{
	_currentPlayTime += deltaSeconds;
}

void PlayerContext::SetGameOver(bool isGameOver)
{
	_isGameOver = isGameOver;
	if (isGameOver)
	{
		_gameOverEvent.Invoke();
	}
}

void PlayerContext::RecordBestTime()
{
	if (_currentPlayTime > _bestPlayTime)
	{
		_bestPlayTime = _currentPlayTime;
	}
}

void PlayerContext::Reset()
{
	_currentPlayTime = 0.0f;
	_level = DEF::PLAYER_START_LEVEL;
	_isGameOver = false;
}