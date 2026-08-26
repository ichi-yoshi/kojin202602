#include "AppFrame.h"
#include "ModeGame.h"
#include "MagicNumberConfig.h"

bool ModeGame::Initialize() 
{
	if (!base::Initialize()) { return false; }

    _cam.Initialize();

	SetUseASyncLoadFlag(TRUE);
	
	_map.Initialize();
	_player.Initialize();
    _gameWave.Initialize();

    SpawnEnemiesForCurrentWave();

    _enemies.clear();
    int targetCount = _gameWave.GetTargetEnemyCount();
    for(int i = 0; i < targetCount; ++i)
    {
        // とりあえず通常の敵を生成して初期化
        auto enemy = std::make_unique<EnemyInfo>(EnemyType::Enemy1);
        enemy->Initialize(_map);
        _enemies.push_back(std::move(enemy));
    }

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

// 敵の生成と配置を一括管理するヘルパー関数
void ModeGame::SpawnEnemiesForCurrentWave()
{
    _enemies.clear(); // 前のウェーブの敵を消去
    int targetCount = _gameWave.GetTargetEnemyCount(); 
        int currentWave = _gameWave.GetCurrentWaveNumber();

    for(int i = 0; i < targetCount; ++i)
    {
        auto enemy = std::make_unique<EnemyInfo>(EnemyType::Enemy1);
            enemy->Initialize(_map);

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

    // 1フレームの経過時間（約0.016秒）
    float deltaTime = 1.0f / 60.0f;

    // ウェーブとタイマーの更新
    _gameWave.Update(deltaTime);

	_player.Update(_cam, _map);

    for(auto& enemy : _enemies)
    {
        enemy->Update(_map, _player.GetPosition(), _score);
        enemy->AttackToPlayer(_player.GetPosition(), _score);
    }

	_map.SetCollisionVisible(_player.IsViewCollision());

	// ウェーブの制限時間が切れた場合の処理
    if(_gameWave.IsTimeUp())
    {
        // 時間切れ処理（ウェーブ進行、またはゲームオーバーなど）
		if(_gameWave.IsGameCleared())
		{
			// ゲームクリア処理
			// ここにゲームクリア時の処理を追加
		}
		else
		{
			// 次のウェーブに進む
			_gameWave.StartNextWave();
            SpawnEnemiesForCurrentWave();
			//// 新しい敵を生成して初期化
			//int targetCount = _gameWave.GetTargetEnemyCount();
			//_enemies.clear(); // 前の敵をクリア

			//for(int i = 0; i < targetCount; ++i)
			//{
			//	auto enemy = std::make_unique<EnemyInfo>(EnemyType::Enemy1);
			//	enemy->Initialize(_map);
			//	_enemies.push_back(std::move(enemy));
			//}
		}
    }

	return true;
}

bool ModeGame::Render()
{
    base::Render();

    if(_loadState == LoadState::Loading)
    {
        // ロード中の画面描画（「Loading...」の文字を表示するなど）
        DrawString(100, 100, "NOW LOADING...", Color::White());
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
    _player.Render();
    _map.Render();

    // UI描画処理の例（Renderなどの後半で呼び出す）

    bool isAnyEnemyInCenter = false;
    float targetRadius = 150.0f;

    for(auto& enemy : _enemies)
    {
        enemy->Render();

        // 誰か一匹でも画面中央に入っているかチェック
        if(enemy->IsInScreenCenter(targetRadius))
        {
            isAnyEnemyInCenter = true;
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
    DrawFormatString(10, 300, Color::White(), "WAVE : %d", _gameWave.GetCurrentWaveNumber());
    DrawFormatString(10, 330, Color::White(), "TIME : %.1f sec", _gameWave.GetRemainingTime());
    return true;
}
