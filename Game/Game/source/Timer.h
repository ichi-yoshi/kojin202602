#pragma once

class Timer
{
public:
	Timer();
	~Timer() = default;

	// タイマーを開始する（指定した秒数でカウントダウン）
	void Start(float seconds);

	// タイマーを停止する
	void Stop();

	// タイマーをリセットする（カウントダウン時間を初期化）
	void Reset();

	// タイマーを更新する（毎フレーム呼び出す）
	void Update(float deltaTime);

	// タイマーを一時停止する
	void Resume();

	// ゲッター
	float GetRemainingTime() const{ return _currentTime; };
	float GetTargetTime() const { return _targetTime; };	

	// タイマーの進捗率を取得する（0.0〜1.0の範囲）
	float GetProgress() const;

	// タイマーが動作中かどうかを取得する
	bool IsRunning() const { return _isRunning; }

	// タイマーが時間切れかどうかを取得する
	bool IsTimeUp() const { return _currentTime <= 0.0f; }

private:
	float _currentTime;	// 現在のカウントダウン時間
	float _targetTime;	// タイマーの目標時間（開始時の秒数）
	bool _isRunning;	// タイマーが動作中かどうか
};

