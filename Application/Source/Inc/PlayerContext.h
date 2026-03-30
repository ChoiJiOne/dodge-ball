#pragma once

#include "Context/IContext.h"
#include "Macro/Macro.h"

class PlayerContext : public IContext
{
public:
	PlayerContext() = default;
	virtual ~PlayerContext() = default;

	DISALLOW_COPY_AND_ASSIGN(PlayerContext);

	float GetCurrentPlayTime() const { return _currentPlayTime; }
	float GetBestPlayTime() const { return _bestPlayTime; }
	bool IsGameOver() const { return _isGameOver; }

	void AddPlayTime(float deltaSeconds) { _currentPlayTime += deltaSeconds; }
	void SetGameOver(bool isGameOver) { _isGameOver = isGameOver; }

	void RecordBestTime()
	{
		if (_currentPlayTime > _bestPlayTime)
		{
			_bestPlayTime = _currentPlayTime;
		}
	}

	void Reset()
	{
		_currentPlayTime = 0.0f;
		_isGameOver = false;
	}

private:
	float _currentPlayTime = 0.0f;
	float _bestPlayTime = 0.0f;
	bool _isGameOver = false;
};
