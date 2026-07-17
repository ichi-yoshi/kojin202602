#include "AppFrame.h"
#include "ModeGame.h"

bool ModeGame::Initialize() 
{
	if (!base::Initialize()) { return false; }

	_cam.Initialize();

	if(!_map.Initialize()) { return false; }
	if(!_player.Initialize()) { return false; }
	_enemy.Initialize(_map);

	return true;
}

bool ModeGame::Terminate()
{
	base::Terminate();
	_player.Terminate();
	_map.Terminate();
	return true;
}

bool ModeGame::Process() 
{
	base::Process();
	
	_player.Update(_cam, _map);
	_enemy.Update(_map, _player.GetPosition());

	_map.SetCollisionVisible(_player.IsViewCollision());
	return true;
}

bool ModeGame::Render()
{
    base::Render();

    // 3D基本設定
    SetUseZBuffer3D(TRUE);
    SetWriteZBuffer3D(TRUE);
    SetUseBackCulling(TRUE);

    // ライト設定
    SetUseLighting(TRUE);

    // プレイヤーの現在地に基づいたポイントライト設定
    VECTOR playerPos = _player.GetPosition();
    SetGlobalAmbientLight(GetColorF(0.f, 0.0f, 0.0f, 0.0f));
    ChangeLightTypePoint(VAdd(playerPos, VGet(0, 50.f, 0)), 1000.0f, 0.0f, 0.005f, 0.0f);

    // カメラ設定適用
    _cam.ApplyCamera();

    // 描画
    //_player.Render();
    _map.Render();
	_enemy.Render();

    return true;
}
