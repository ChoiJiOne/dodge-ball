#pragma once

#include <cstdint>

class GameTimer
{
public:
	GameTimer();
	GameTimer(GameTimer&& instance) noexcept;
	GameTimer(const GameTimer& instance) noexcept;
	virtual ~GameTimer() = default;

	float GetTotalSeconds() const;
	float GetDeltaSeconds() const;

	void Reset();
	void Start();
	void Stop();
	void Tick();

private:
	double _baseTime = 0.0;
	double _pausedTime = 0.0;
	double _stopTime = 0.0;
	double _prevTime = 0.0;
	double _currTime = 0.0;
	double _deltaTime = 0.0;

	bool _bIsStopped = false;
};