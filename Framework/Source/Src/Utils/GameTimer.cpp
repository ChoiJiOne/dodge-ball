#include <raylib.h>

#include <GameTimer.h>

GameTimer::GameTimer()
	: _baseTime(0.0)
	, _pausedTime(0)
	, _stopTime(0)
	, _prevTime(0)
	, _currTime(0)
	, _deltaTime(-1.0)
	, _bIsStopped(false) {}

GameTimer::GameTimer(GameTimer&& instance) noexcept
	: _deltaTime(instance._deltaTime)
	, _baseTime(instance._baseTime)
	, _pausedTime(instance._pausedTime)
	, _prevTime(instance._prevTime)
	, _currTime(instance._currTime)
	, _bIsStopped(instance._bIsStopped) {}

GameTimer::GameTimer(const GameTimer& instance) noexcept
	: _deltaTime(instance._deltaTime)
	, _baseTime(instance._baseTime)
	, _pausedTime(instance._pausedTime)
	, _prevTime(instance._prevTime)
	, _currTime(instance._currTime)
	, _bIsStopped(instance._bIsStopped) {}

float GameTimer::GetTotalSeconds() const
{
	if (_bIsStopped)
	{
		return static_cast<float>((_stopTime - _pausedTime) - _baseTime);
	}
	else
	{
		return static_cast<float>((_currTime - _pausedTime) - _baseTime);
	}
}

float GameTimer::GetDeltaSeconds() const
{
	return static_cast<float>(_deltaTime);
}

void GameTimer::Reset()
{
	double currTime = GetTime();

	_baseTime = currTime;
	_prevTime = currTime;
	_stopTime = 0.0;
	_pausedTime = 0.0;
	_bIsStopped = false;
}

void GameTimer::Start()
{
	if (!_bIsStopped)
	{
		return;
	}

	double startTime = GetTime();

	_pausedTime += (startTime - _stopTime);
	_prevTime = startTime;
	_stopTime = 0.0;
	_bIsStopped = false;
}

void GameTimer::Stop()
{
	if (_bIsStopped)
	{
		return;
	}

	_stopTime = GetTime();
	_bIsStopped = true;
}

void GameTimer::Tick()
{
	if (_bIsStopped)
	{
		_deltaTime = 0.0;
		return;
	}

	_currTime = GetTime();
	_deltaTime = (_currTime - _prevTime);
	_prevTime = _currTime;

	if (_deltaTime < 0.0)
	{
		_deltaTime = 0.0;
	}
}