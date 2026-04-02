#include "PlayerDataChunk.h"

#include "Manager/DataChunkManager.h"

#include "Context/PlayerContext.h"

PlayerContext::PlayerContext()
{
	Result<const PlayerDataChunk*> result = DataChunkManager::Get().GetDataChunk<PlayerDataChunk>();
	if (result.IsSuccess())
	{
		_dataChunk = result.GetValue();
		_level = _dataChunk->DataPacks.front().Level;
		_maxLevel = _dataChunk->DataPacks.back().Level;
	}
}

void PlayerContext::AddPlayTime(float deltaSeconds)
{
	_currentPlayTime += deltaSeconds;

	if (_dataChunk == nullptr || _level == _maxLevel)
	{
		return;
	}

	int32_t idx = _dataChunk->LevelToIdx.at(_level);
	const PlayerDataPack& nextLevelDataPack = _dataChunk->DataPacks[idx + 1];
	if (_currentPlayTime >= nextLevelDataPack.LevelUpTime)
	{
		_level = nextLevelDataPack.Level;
		_levelUpEvent.Invoke(_level);
	}
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