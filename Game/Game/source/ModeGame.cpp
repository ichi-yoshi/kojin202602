#include "AppFrame.h"
#include "ModeGame.h"

bool ModeGame::Initialize() 
{
	if (!base::Initialize()) { return false; }

    _cam.Initialize();

	SetUseASyncLoadFlag(TRUE);
	
	_map.Initialize();
	_player.Initialize();
	_enemy.Initialize(_map);

	SetUseASyncLoadFlag(FALSE);

	_loadState = LoadState::Loading;
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
	
    if(_loadState == LoadState::Loading)
    {
        // 全てのアセットの非同期ロードが完了したか確認
        if(GetASyncLoadNum() == 0) // 現在進行中の非同期ロード数が 0 になったら完了
        {
            _loadState = LoadState::Ready;
        }
        else
        {
            // ロード中なのでゲームロジックの更新はスキップ
            return true;
        }
    }

	_player.Update(_cam, _map);
	_enemy.Update(_map, _player.GetPosition());

	_map.SetCollisionVisible(_player.IsViewCollision());
	return true;
}

bool ModeGame::Render()
{
    base::Render();

    if(_loadState == LoadState::Loading)
    {
        // ロード中の画面描画（「Loading...」の文字を表示するなど）
        DrawString(100, 100, "NOW LOADING...", GetColor(255, 255, 255));
        return true;
    }

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
