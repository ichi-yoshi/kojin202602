#pragma once

class Timer
{
public:
	Timer();
	~Timer() = default;

	void Start(float seconds);

	void Stop();

	void Reset();

	void Update(float deltaTime);

	void Resume();

	float GetRemainingTime() const{ return _currentTime; };
	float GetTargetTime() const { return _targetTime; };	

	float GetProgress() const;

	bool IsRunning() const { return _isRunning; }
	bool IsTimeUp() const { return _currentTime <= 0.0f; }

private:
	float _currentTime;
	float _targetTime;
	bool _isRunning;
};

