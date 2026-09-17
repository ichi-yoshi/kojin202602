#include "GameWave.h"
#include "Gauge.h"
#include "MagicNumberConfig.h"

GameWave::GameWave() 
{
	_currentWaveIndex = -1; // 初期状態ではウェーブは開始されていない
	_isGameCleared = false;
	_isInterval = false;
}

void GameWave::Initialize() 
{
	_waveList.clear();
	_currentWaveIndex = -1;
	_isGameCleared = false;
	_isInterval = false;

	// ウェーブデータの初期化(左から順に: ウェーブ番号, 敵の数, 制限時間(秒), 出現間隔(秒), スコア倍率)
	_waveList.push_back({ 1, 1, 30, 1.0f });
	_waveList.push_back({ 2, 3, 60, 1.2f });
	_waveList.push_back({ 3, 5, 60, 1.5f });

	StartNextWave();
}

void GameWave::StartNextWave() 
{
	_currentWaveIndex++;

	// ウェーブが全てクリアされたかチェック
	if(_currentWaveIndex >= static_cast<int>(_waveList.size()))
	{
		_isGameCleared = true; // 全てのウェーブをクリアした場合
		_waveTimer.Stop();
		_isInterval = false;
		return;
	}

	// 現在のウェーブデータを取得してタイマーを開始
	_isInterval = false;
	const auto& currentData = _waveList[_currentWaveIndex];
	_waveTimer.Start(currentData.timeLimit);
}

void GameWave::Update(float deltaTime) 
{
	if(_isGameCleared) return;

	// ウェーブの制限時間またはインターバル時間を更新
	if(_isInterval) 
	{
		_intervalTimer.Update(deltaTime);
	}
	else 
	{
		_waveTimer.Update(deltaTime);

		if(IsTimeUp())
		{
			_isInterval = true;
			_intervalTimer.Start(INTERVAL_TIME);
		}
	}
	
}

// ウェーブ番号を取得するメソッド
int GameWave::GetCurrentWaveNumber() const
{
	if(_currentWaveIndex < 0 || _currentWaveIndex >= static_cast<int>(_waveList.size())) return 0;
	return _waveList[_currentWaveIndex].waveNumber;
}

// スコア倍率を取得するメソッド
float GameWave::GetScoreMultiplier() const
{
	if(_currentWaveIndex < 0 || _currentWaveIndex >= static_cast<int>(_waveList.size())) return 1.0f;
	return _waveList[_currentWaveIndex].scoreMultiplier;
}

// このウェーブで出現する敵の数を取得するメソッド
int GameWave::GetTargetEnemyCount() const
{
	if(_currentWaveIndex < 0 || _currentWaveIndex >= static_cast<int>(_waveList.size())) return 0;
	return _waveList[_currentWaveIndex].enemyCount;
}

//// このウェーブでの敵の出現間隔を取得するメソッド
//float GameWave::GetSpawnInterval() const
//{
//	if(_currentWaveIndex < 0 || _currentWaveIndex >= static_cast<int>(_waveList.size())) return 0.0f;
//	return _waveList[_currentWaveIndex].spawnInterval;
//}

void GameWave::Render() const 
{
	if(_isGameCleared || _currentWaveIndex < 0 ) return;
	
	int screenWidth = 0, screenHeight = 0;
	GetScreenState(&screenWidth, &screenHeight, NULL);

	int posX = screenWidth / 2;
	int posY = 30;
	int gaugeWidth = 400;
	int gaugeHeight = 20;

	float remainingRate = 0.0f;
	int fillColor = Color::Green();
	const char* waveText = "";

	// ウェーブ番号の表示
	if(_isInterval) 
	{
		remainingRate = 1.0f - _intervalTimer.GetProgress();
		fillColor = Color::Yellow();
	}
	else 
	{
		remainingRate = 1.0f - _waveTimer.GetProgress();
		if(remainingRate < 0.2f) 
		{
			fillColor = Color::Red();
		}
		else 
		{
			fillColor = Color::Green();
		}
	}

	Gauge timeGauge;
	timeGauge.SetSize(gaugeWidth, gaugeHeight);
	timeGauge.SetPosition(posX, posY, true);
	timeGauge.SetColor(fillColor, Color::White(), Color::Dim());
	timeGauge.Render(remainingRate);
}