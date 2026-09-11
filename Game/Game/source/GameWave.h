#pragma once
#include <vector>
#include "Timer.h"

struct WaveData 
{
	int waveNumber; // ウェーブ番号
	int enemyCount; // このウェーブで出現する敵の数
	int timeLimit;  // このウェーブの制限時間（秒）
	//float spawnInterval; // 敵が出現する間隔（秒）
	float scoreMultiplier; // このウェーブのスコア倍率
};

class GameWave
{
public:
	GameWave();
	~GameWave() = default;

	void Initialize();
	void Update(float deltaTime);

	// 次のウェーブを開始するメソッド
	void StartNextWave();

	// ゲッター
	int GetCurrentWaveNumber() const;
	float GetScoreMultiplier() const;
	int GetTargetEnemyCount() const;
	//float GetSpawnInterval() const;

	float GetRemainingTime() const {return _waveTimer.GetRemainingTime();}
	float GetIntervalTime()const { return _intervalTimer.GetRemainingTime(); }

	// ウェーブの制限時間が終了したかどうかを返すメソッド
	bool IsTimeUp() const { return !_waveTimer.IsRunning() && _waveTimer.GetRemainingTime() <= 0.0f; }

	// ウェーブ間のインターバル時間が終了したかどうかを返すメソッド
	bool IsIntervalTimeUp() const { return !_intervalTimer.IsRunning() && _intervalTimer.GetRemainingTime() <= 0.0f; }

	// ウェーブ間のインターバル中かどうかを返すメソッド
	bool IsInterval() const { return _isInterval; }

	// ゲームがクリアされたかどうかを返すメソッド
	bool IsGameCleared() const { return _isGameCleared; }

private:
	std::vector<WaveData> _waveList; // ウェーブデータのリスト
	int _currentWaveIndex;	// 現在のウェーブのインデックス
	bool _isGameCleared;	// ゲームがクリアされたかどうかのフラグ

	Timer _waveTimer;		// ウェーブの制限時間を管理するタイマー
	Timer _intervalTimer;	// ウェーブ間のインターバル時間を管理するタイマー
	bool _isInterval;		// インターバル中かどうかのフラグ

	static constexpr float INTERVAL_TIME = 5.0f; // ウェーブ間のインターバル時間（秒）
};

