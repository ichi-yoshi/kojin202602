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
	void StartNextWave();

	int GetCurrentWaveNumber() const;
	float GetScoreMultiplier() const;
	int GetTargetEnemyCount() const;
	//float GetSpawnInterval() const;

	float GetRemainingTime() const {return _waveTimer.GetRemainingTime();}
	bool IsTimeUp() const { return _waveTimer.IsTimeUp(); }
	bool IsGameCleared() const { return _isGameCleared; }

private:
	std::vector<WaveData> _waveList; // ウェーブデータのリスト
	int _currentWaveIndex;
	bool _isGameCleared;

	Timer _waveTimer; // ウェーブの制限時間を管理するタイマー
};

