#include "EnemyBase.h"
#include "Map.h"
#include "MagicNumberConfig.h"

EnemyBase::EnemyBase(const EnemyData& data) 
{
	_param = data;
	_pos = data.initialPos;
	_speed = data.speed;
	_imageHandle = -1;
	_pathIndex = 0;

	// スタミナの初期化（最大値、回復率、消費率）
	_stamina.Initialize(_param.staminaMax, _param.staminaRecoveryRate, _param.staminaCostRate);
}

void EnemyBase::Initialize(const Map& map) 
{
	if(_param.imagePath != nullptr) 
	{
		_imageHandle = LoadGraph(_param.imagePath);
	}

	SetupAStar(map);
	_pos = _param.initialPos;
}

void EnemyBase::SetupAStar(const Map& map) 
{	
	// 正しく取得した最小・最大座標
	VECTOR mapMin = map.GetMinPosition();
	VECTOR mapMax = map.GetMaxPosition();

	// マップの大きさを計算
	float mapWidthX = mapMax.x - mapMin.x;
	float mapLengthZ = mapMax.z - mapMin.z;

	// マップを覆うのに必要なマス数を計算
	int gridWidth = static_cast<int>(ceilf(mapWidthX / CELL_SIZE));
	int gridHeight = static_cast<int>(ceilf(mapLengthZ / CELL_SIZE));

	// A*グリッドの再構築
	_pathfinder.BuildGridFromMap(map, mapMin, CELL_SIZE, gridWidth, gridHeight);
}

void EnemyBase::Update(const Map& map, VECTOR playerPos, Score& score) 
{
	// 経路再計算のフレーム間隔を管理するための静的変数
	static int recalcTimer = 0;

	if(IsInScreenCenter(GameConfig::LOOK_CENTER_RADIUS))
	{
		VECTOR enemyPos = VAdd(_pos, VGet(0.0f, GameConfig::ENEMY_HEIGHT, 0.0f));
		VECTOR hitPos;
		if(map.CheckCollision(enemyPos, 40.0f, hitPos))
		{
			_stamina.Consume(_speed);	// 画面中央にいる場合はスタミナを消費
			score.AddScore(1);			// スコアを加算
		}
	}

	// スタミナが尽きている場合の処理
	if(_stamina.IsExhausted())
	{
		_stamina.Recover(); // 回復処理

		if(_stamina.IsExhausted())
		{
			_stamina.Recover(); // 姿を消した状態で回復

			// 全回復した瞬間に「プレイヤーから少し離れた場所」へ再出現
			if(!_stamina.IsExhausted())
			{
				// プレイヤーの周囲に安全な位置を探す
				bool spawnSuccess = false;

				for(int i = 0; i < MAX_ATTEMPTS; ++i)
				{
					// プレイヤーの周囲360度からランダムな方向・距離を決定
					float angle = (float)(rand() % 360) * DEG2RAD(1);
					float spawnDistance = MIN_SPAWN_DIST + (float)(rand() % static_cast<int>(RAND_SPAWN_DIST)); // プレイヤーからの距離をランダムに設定

					VECTOR candidatePos;
					candidatePos.x = playerPos.x + cosf(angle) * spawnDistance;
					candidatePos.z = playerPos.z + sinf(angle) * spawnDistance;
					candidatePos.y = playerPos.y;

					// 地面との当たり判定チェック
					VECTOR hitPos;
					if(map.CheckCollision(candidatePos, 100.0f, hitPos))
					{
						candidatePos.y = hitPos.y;
					}

					// 生成した位置がA*上で歩行可能なエリアかチェック
					if(_pathfinder.IsWalkableWorldPos(candidatePos))
					{
						_pos = candidatePos;
						spawnSuccess = true;
						break; // 移動可能な安全な場所が見つかったのでループを抜ける
					}
				}

				//// 万が一探しても見つからなかった場合はプレイヤーの直近（安全保証位置）に出現
				//if(!spawnSuccess)
				//{
				//	_pos = playerPos; // 必要に応じて調整
				//}

				// 追跡状態をリセットし、次のフレームで即座にA*探索を走らせる
				_path.clear();
				_pathIndex = 0;
				recalcTimer = RECALC_INTERVAL;
			}

			// 消滅中はこれ以降の移動・コリジョン処理を行わずに抜ける
			return;
		}
	}

	// 敵とプレイヤーの直線距離を計算
	VECTOR toPlayer = VSub(playerPos, _pos);
	float distToPlayer = VSize(toPlayer);

	if(distToPlayer < CLOSE_RANGE)
	{
		// 近距離ならA*のルートはクリアする
		_path.clear();

		if(distToPlayer > STOP_RANGE)
		{
			// 障害物がない前提で、プレイヤーの方向に直接スムーズに移動する
			VECTOR dir = VNorm(toPlayer);
			_pos = VAdd(_pos, VScale(dir, _speed));
			//_stamina.Consume(VSize(VScale(dir, _speed)));
		}

		// 足元の床高さに吸着
		VECTOR hitPos;
		if(map.CheckCollision(_pos, 40.0f, hitPos)) { _pos.y = hitPos.y; }
		return; // 近距離処理が終わったらここでUpdateを抜ける
	}

	// 遠距離時の従来のA*処理 ---
	recalcTimer++;

	// 一定時間ごとに経路を再計算する
	if(recalcTimer >= RECALC_INTERVAL || _path.empty())
	{
		_path = _pathfinder.FindPath(_pos, playerPos);
		_pathIndex = 0;
		recalcTimer = 0;
	}

	// 経路に沿って移動する
	if(!_path.empty() && _pathIndex < _path.size())
	{
		VECTOR targetPos = _path[_pathIndex];
		VECTOR toTarget = VSub(targetPos, _pos);
		float dist = VSize(toTarget);

		// 到達判定を少し広め（8.0f〜12.0f程度）にしておくことで往復を防ぐ
		if(dist < 10.0f)
		{
			_pathIndex++;
		}
		else
		{
			VECTOR dir = VNorm(toTarget);
			_pos = VAdd(_pos, VScale(dir, _speed));
			//_stamina.Consume(VSize(VScale(dir, _speed)));
		}
	}

	// 足元の床高さに吸着
	VECTOR hitPos;
	if(map.CheckCollision(_pos, 40.0f, hitPos))
	{
		_pos.y = hitPos.y;
	}
}

bool EnemyBase::IsInScreenCenter(float targetRadiusPixels)
{
	// スタミナ切れ中は画面中央判定を無効化
	if(_stamina.IsExhausted()) return false;

	VECTOR checkPos = _pos;
	checkPos.y += GameConfig::ENEMY_HEIGHT; // 敵の頭上付近を判定対象にする

	// ワールド座標をスクリーン座標に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(checkPos);

	if(screenPos.z < 0.0f || screenPos.z > 1.0f)
	{
		return false;
	}

	// 画面中央の範囲を計算
	int screenWidth = Layout::Screen.w;
	int screenHeight = Layout::Screen.h;
	GetScreenState(&screenWidth, &screenHeight, NULL);

	// 画面中央の座標を計算
	float centerX = screenWidth / 2.0f;
	float centerY = screenHeight / 2.0f;

	// 画面中央からの距離を計算
	float dx = screenPos.x - centerX;
	float dy = screenPos.y - centerY;
	float distFromCenter = sqrtf(dx * dx + dy * dy);

	// 画面中央からの距離が指定された半径以内かどうかを判定
	return distFromCenter <= targetRadiusPixels;
}

void EnemyBase::AttackToPlayer(VECTOR playerPos, Score& score)
{
	// 攻撃処理
	VECTOR toPlayer = VSub(playerPos, _pos);
	float distToPlayer = VSize(toPlayer);

	// 攻撃判定はスタミナが尽きていない場合のみ有効
	if(!_stamina.IsExhausted())
	{
		if(distToPlayer <= 10.0f)
		{
			_stamina.Consume(9999);		// スタミナを強制的に消費して消滅させる
			score.SubtractScore(100);	// プレイヤーのスコアを減らす
		}
	}
}

void EnemyBase::Render()
{
	if(_imageHandle == -1) return;

	// 敵キャラの位置に画像（ビルボード）を描画
	if(!_stamina.IsExhausted())
	{
		if(_imageHandle != -1)
		{
			VECTOR renderPos = _pos;
			renderPos.y += GameConfig::ENEMY_HEIGHT;

			// 3D空間上の敵の座標に、カメラを常に向く画像（ビルボード）を描画する
			DrawBillboard3D(renderPos, 0.5f, 0.5f, 200.0f, 0.0f, _imageHandle, TRUE);
		}
	}

	//デバッグ用
	// A*の床グリッドやルート線画を表示
	if(CheckHitKey(KEY_INPUT_SPACE))
	{
		_pathfinder.DebugRender();
	}

	// デバッグ用
	DrawFormatString(0, 0, Color::White(), "Enemy Pos: (%.2f, %.2f, %.2f)", _pos.x, _pos.y, _pos.z);
	DrawFormatString(0, 20, Color::White(), "Enemy Stamina: %.1f / %.1f (%s)",
		_stamina.GetCurrent(), _stamina.GetMax(), _stamina.IsExhausted() ? "EXHAUSTED" : "OK");
}