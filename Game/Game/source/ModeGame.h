#pragma once
#include "appframe.h"
#include "CameraBase.h"
#include "Player.h"
#include "Map.h"
#include "Score.h"
#include "GameWave.h"
#include "EnemyInfo.h"
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

	enum class LoadState 
	{
		Loading,
		Setup,
		Ready
	};
	LoadState _loadState = LoadState::Loading;

	void SpawnEnemiesForCurrentWave();
}; 
