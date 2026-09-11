#include "Timer.h"
#include <algorithm> 

Timer::Timer() 
{
	_currentTime = 0.0f;
	_targetTime = 0.0f;
	_isRunning = false;
}

void Timer::Start(float seconds)
{
	_targetTime = seconds;
	_currentTime = seconds;
	_isRunning = true;
}

void Timer::Stop()
{
	_isRunning = false;
}

void Timer::Reset()
{
	_currentTime = _targetTime;
	_isRunning = false;
}

void Timer::Resume() 
{
	// タイマーが停止中で、残り時間がある場合に再開する
	if(_currentTime > 0.0f)
	{
		_isRunning = true;
	}
}

void Timer::Update(float deltaTime) 
{
	if(!_isRunning) return;

	_currentTime -= deltaTime;
	if(_currentTime <= 0.0f)
	{
		_currentTime = 0.0f;
		_isRunning = false;
	}
}

float Timer::GetProgress() const
{
	// 進捗率を計算する（0.0〜1.0の範囲）
	if(_targetTime <= 0.0f) return 1.0f;
	return std::clamp(1.0f - (_currentTime / _targetTime), 0.0f, 1.0f);
}