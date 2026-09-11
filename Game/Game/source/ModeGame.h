#pragma once
#include "appframe.h"
#include "CameraBase.h"
#include "Player.h"
#include "Map.h"
#include "Score.h"
#include "GameWave.h"
#include "EnemyInfo.h"
#include "GameTitle.h"
#include "GameResult.h"
#include "GameLoad.h"
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
	CameraBase	_cam;		// カメラ
	Player _player;			// プレイヤー
	Map _map;				// マップ
	Score _score;			// スコア管理
	GameWave _gameWave;     // ウェーブ管理
	std::vector<std::unique_ptr<EnemyBase>> _enemies; // 複数の敵を管理する配列
	GameTitle _title;		// タイトル画面の管理
	GameResult _gameResult; // ゲーム結果画面の管理
	GameLoad _gameLoad;		// ロード画面の管理

	// ゲームのロード状態を管理する列挙型
	enum class LoadState 
	{
		Title,
		Loading,
		Ready,
		Result
	};
	LoadState _loadState = LoadState::Title;	// 現在のロード状態を保持する変数

	// 敵を現在のウェーブに応じて生成する関数
	void SpawnEnemiesForCurrentWave();
}; 
