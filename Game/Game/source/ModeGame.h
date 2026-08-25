#pragma once
#include "appframe.h"
#include "CameraBase.h"
#include "Player.h"
#include "Map.h"
#include "Enemy.h"
#include "Score.h"

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
	Enemy _enemy;
	Score _score;

	enum class LoadState 
	{
		Loading,
		Setup,
		Ready
	};
	LoadState _loadState = LoadState::Loading;
}; 
