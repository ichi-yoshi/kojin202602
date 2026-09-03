#include "AppFrame.h"
#include "ModeGame.h"
#include "MagicNumberConfig.h"
#include "ResourceManager.h"

bool ModeGame::Initialize() 
{
	if (!base::Initialize()) { return false; }

    _cam.Initialize();

	_title.Initialize();

    _loadState = LoadState::Title;
    
	return true;
}

bool ModeGame::Terminate()
{
	base::Terminate();
	_player.Terminate();
	_map.Terminate();
	return true;
}

// 敵の生成と配置を一括管理するヘルパー関数
void ModeGame::SpawnEnemiesForCurrentWave()
{
    _enemies.clear(); // 前のウェーブの敵を消去
	int targetCount = _gameWave.GetTargetEnemyCount(); // 現在のウェーブで出現する敵の数を取得
    int currentWave = _gameWave.GetCurrentWaveNumber();// 現在のウェーブ番号を取得

    for(int i = 0; i < targetCount; ++i)
    {
		// EnemyInfoのインスタンスを作成し、初期化とA*探索のセットアップを行う
        auto enemy = std::make_unique<EnemyInfo>(EnemyType::Enemy1);
            enemy->Initialize(_map);
			enemy->SetupAStar(_map);

            // 1ウェーブ目は固定初期位置（重なり防止でずらす）、2ウェーブ目以降はランダム出現
            if(currentWave <= 1)
            {
                VECTOR spawnPos = EnemyType::Enemy1.initialPos;
            }
            else
            {
                // プレイヤーの周囲にランダム出現させる
                enemy->SetRandomSpawnPos(_map, _player.GetPosition());
            }

        _enemies.push_back(std::move(enemy));
    }
}

bool ModeGame::Process() 
{
	base::Process();
	
	if(_loadState == LoadState::Title)
	{
		_title.Update();

		if(_title.IsFinished())
		{
			_gameLoad.Update();

            _loadState = LoadState::Loading;
		}
		return true;
	}

    if(_loadState == LoadState::Loading)
    {
        // 全てのアセットの非同期ロードが完了したか確認
        if(GetASyncLoadNum() == 0) // 現在進行中の非同期ロード数が 0 になったら完了
        {
            _map.Initialize();
            _player.Initialize();
            _gameWave.Initialize();
            SpawnEnemiesForCurrentWave();
            _loadState = LoadState::Ready;
        }
        else
        {
            // ロード中なのでゲームロジックの更新はスキップ
            return true;
        }
    }

    // 1フレームの経過時間（約0.016秒）
    float deltaTime = 1.0f / 60.0f;

	_player.Update(_cam, _map);
    
	// 敵の更新（ウェーブのインターバル中は敵の行動を停止）
    if(!_gameWave.IsInterval())
    {
        for(auto& enemy : _enemies)
        {
            enemy->Update(_map, _player.GetPosition(), _score);
            enemy->AttackToPlayer(_player.GetPosition(), _score);
            enemy->EnenmyCollision(_enemies); // 敵同士の衝突判定
        }
    }
    
    // デバッグ用
	// マップのコリジョン可視化設定
	//_map.SetCollisionVisible(_player.IsViewCollision());

    // ウェーブとタイマーの更新
    _gameWave.Update(deltaTime);

	// ゲームクリア判定
	if(_gameWave.IsGameCleared())   // すべてのウェーブをクリアした場合
	{
		_loadState = LoadState::Result;
	}
	else if(_gameWave.IsInterval()) // ウェーブ間のインターバル中
    {
		// インターバル時間が経過したかチェック
        if(_gameWave.IsIntervalTimeUp())
        {
            // 次のウェーブに進む
            _gameWave.StartNextWave();
            SpawnEnemiesForCurrentWave();
        }
    }
	
	return true;
}

bool ModeGame::Render()
{
    base::Render();

	if(_loadState == LoadState::Title)
	{
		_title.Render();
		return true;
	}

    if(_loadState == LoadState::Loading)
    {
        // ロード中の画面描画
		_gameLoad.Render();
        return true;
    }

	if(_loadState == LoadState::Result)
	{
		// ゲームクリア画面の描画
        _map.Render();
		_gameResult.Render(_score);
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
    SetGlobalAmbientLight(GetColorF(0.0f, 0.0f, 0.0f, 0.0f));
    ChangeLightTypePoint(VAdd(playerPos, VGet(0, 50.0f, 0)), 1000.0f, 0.0f, 0.005f, 0.0f);

    // カメラ設定適用
    _cam.ApplyCamera();

    // 描画
    _map.Render();
    _player.Render();
    // UI描画処理の例（Renderなどの後半で呼び出す）

    bool isAnyEnemyInCenter = false;
    float targetRadius = GameConfig::LOOK_CENTER_RADIUS;

    if(!_gameWave.IsInterval())
    {
        for(auto& enemy : _enemies)
        {
            enemy->Render();

            // 誰か一匹でも画面中央に入っているかチェック
            if(enemy->IsInScreenCenter(targetRadius))
            {
                isAnyEnemyInCenter = true;
            }
        }
    }

    int screenWidth = 0, screenHeight = 0;
    GetScreenState(&screenWidth, &screenHeight, NULL);
    int centerX = screenWidth / 2;
    int centerY = screenHeight / 2;

    // 中央の判定エリアを円で描画
    if(isAnyEnemyInCenter)
    {
        // 捕捉中：緑色の円とクロスヘア
        DrawCircle(centerX, centerY, (int)targetRadius, Color::Green(), FALSE);
        DrawPixel(centerX, centerY, Color::Green());
    }
    else
    {
        // 未捕捉：赤色（白）の円とクロスヘア
        DrawCircle(centerX, centerY, (int)targetRadius, Color::White(), FALSE);
        DrawPixel(centerX, centerY, Color::White());
    }

    // スコア表示
    // デバッグ用
    _score.Render();

	// ウェーブ情報とタイマー表示
    // デバッグ用
    if(_gameWave.IsInterval())
    {
        DrawFormatString(centerX - 100, centerY - 50, Color::White(), "NEXT WAVE IN: %.1f", _gameWave.GetIntervalTime());
    }
    else
    {
        DrawFormatString(10, 300, Color::White(), "WAVE : %d", _gameWave.GetCurrentWaveNumber());
        DrawFormatString(10, 330, Color::White(), "TIME : %.1f sec", _gameWave.GetRemainingTime());
    }
    return true;
}
