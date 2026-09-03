#pragma once
#include "appframe.h"
#include "CameraBase.h"
#include "Player.h"
#include "Map.h"
#include "Score.h"
#include "GameWave.h"
#include "EnemyInfo.h"
#include "GameTitle.h"
#include <memory>

// モード
class ModeGame : public ModeBase
{
	typedef ModeBase base;
public:
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();

protected:
	CameraBase	_cam;
	Player _player;
	Map _map;
	Score _score;
	GameWave _gameWave;                            // ウェーブ管理
	std::vector<std::unique_ptr<EnemyBase>> _enemies; // 複数の敵を管理する配列
	GameTitle _title; // タイトル画面の管理

	enum class LoadState 
	{
		Title,
		Loading,
		Ready
	};
	LoadState _loadState = LoadState::Title;

	// 敵を現在のウェーブに応じて生成する関数
	void SpawnEnemiesForCurrentWave();
}; 
