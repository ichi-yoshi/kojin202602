#include "Enemy.h"
#include "Map.h"
#include "Resource.h"
#include "MagicNumberConfig.h"

Enemy::Enemy() 
{
	_pos = VGet(0, 0, 0);
	_speed = 8.5f;
	_imageHandle = -1;
	_pathIndex = 0;

	_stamina.Initialize(100.0f, 0.01f, 0.01f);
}

void Enemy::Initialize(const Map& map) 
{
	_imageHandle = LoadGraph(image::Enemy1);

	SetupAStar(map);

	// 敵を配置
	_pos = VGet(1000.0f, 0.0f, -300.0f);
}

void Enemy::SetupAStar(const Map& map) 
{
	// 正しく取得した最小・最大座標
	VECTOR mapMin = map.GetMinPosition();
	VECTOR mapMax = map.GetMaxPosition();

	// 中心座標を計算
	VECTOR mapCenter;
	mapCenter.x = (mapMin.x + mapMax.x) / 2.0f;
	mapCenter.y = (mapMin.y + mapMax.y) / 2.0f;
	mapCenter.z = (mapMin.z + mapMax.z) / 2.0f;

	// マップの大きさを計算
	float mapWidthX = mapMax.x - mapMin.x;
	float mapLengthZ = mapMax.z - mapMin.z;

	// 1マスのサイズ
	float cellSize = 15.0f;

	// マップを覆うのに必要なマス数を計算
	int gridWidth = static_cast<int>(ceilf(mapWidthX / cellSize));
	int gridHeight = static_cast<int>(ceilf(mapLengthZ / cellSize));

	// A*グリッドの再構築
	_pathfinder.BuildGridFromMap(map, mapMin, cellSize, gridWidth, gridHeight);
}

void Enemy::Update(const Map& map, VECTOR playerPos)
{
	static int recalcTimer = 0;

	if(_stamina.IsExhausted())
	{
		_stamina.Recover(); // 回復処理

		if(_stamina.IsExhausted())
		{
			_stamina.Recover(); // 姿を消した状態で回復

			// 全回復した瞬間に「プレイヤーから少し離れた場所」へ再出現
			if(!_stamina.IsExhausted())
			{
				bool spawnSuccess = false;
				const int MAX_ATTEMPTS = 60; // 安全な場所を探す最大試行回数

				for(int i = 0; i < MAX_ATTEMPTS; ++i)
				{
					// プレイヤーの周囲360度からランダムな方向・距離を決定
					float angle = (float)(rand() % 360) * 3.1415926f / 180.0f;
					float spawnDistance = 200.0f + (float)(rand() % 150); // 指定範囲でランダム

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
				recalcTimer = 40;
			}

			// 消滅中はこれ以降の移動・コリジョン処理を行わずに抜ける
			return;
		}
	}

	// 敵とプレイヤーの直線距離を計算
	VECTOR toPlayer = VSub(playerPos, _pos);
	float distToPlayer = VSize(toPlayer);

	// 近距離にプレイヤーがいる場合の処理
	const float CLOSE_RANGE = 50.0f;    // 近距離とみなす距離（A*を無視して直接追従する）
	const float STOP_RANGE = 10.0f;     // これ以上近づいたら完全に止まる

	if(distToPlayer < CLOSE_RANGE)
	{
		// 近距離ならA*のルートはクリアする
		_path.clear();

		if(distToPlayer > STOP_RANGE)
		{
			// 障害物がない前提で、プレイヤーの方向に直接スムーズに移動する
			VECTOR dir = VNorm(toPlayer);
			_pos = VAdd(_pos, VScale(dir, _speed));
			_stamina.Consume(VSize(VScale(dir, _speed)));
		}
		else
		{
			_stamina.Recover(); // 移動停止中は回復
		}

		// 足元の床高さに吸着
		VECTOR hitPos;
		if(map.CheckCollision(_pos, 40.0f, hitPos)) { _pos.y = hitPos.y; }
		return; // 近距離処理が終わったらここでUpdateを抜ける
	}

	// 遠距離時の従来のA*処理 ---
	recalcTimer++;

	// 一定時間ごとに経路を再計算する
	if(recalcTimer >= 40 || _path.empty())
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
			_stamina.Consume(VSize(VScale(dir, _speed)));
		}
	}
	else
	{
		_stamina.Recover(); // 移動していない場合は回復
	}

	// 足元の床高さに吸着
	VECTOR hitPos;
	if(map.CheckCollision(_pos, 40.0f, hitPos))
	{
		_pos.y = hitPos.y;
	}
}

bool Enemy::IsInScreenCenter(float targetRadiusPixels)
{
	// スタミナ切れ中は画面中央判定を無効化
	if(_stamina.IsExhausted()) return false; 

	VECTOR checkPos = _pos;
	checkPos.y += 70.0f; // 敵の頭上付近を判定対象にする

	// ワールド座標をスクリーン座標に変換
	VECTOR screenPos = ConvWorldPosToScreenPos(checkPos);

	if(screenPos.z < 0.0f || screenPos.z>1.0f) 
	{
		return false;
	}

	// 画面中央の範囲を計算
	int screenWidth = Layout::Screen.w;
	int screenHeight = Layout::Screen.h;
	GetScreenState(&screenWidth, &screenHeight,NULL);

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

void Enemy::Render()
{
	if(_imageHandle == -1) return;

	//デバッグ用
	// A*の床グリッドやルート線画を表示
	if(CheckHitKey(KEY_INPUT_SPACE)) 
	{
		_pathfinder.DebugRender();
	}
	
	// 敵キャラの位置に画像（ビルボード）を描画
	if(!_stamina.IsExhausted())
	{
		if(_imageHandle != -1)
		{
			VECTOR renderPos = _pos;
			renderPos.y += 70.0f;

			// 3D空間上の敵の座標に、カメラを常に向く画像（ビルボード）を描画する
			DrawBillboard3D(renderPos, 0.5f, 0.5f, 200.0f, 0.0f, _imageHandle, TRUE);
		}
	}
	
	// デバッグ用
	DrawFormatString(0, 0, Color::White(), "Enemy Pos: (%.2f, %.2f, %.2f)", _pos.x, _pos.y, _pos.z);
	DrawFormatString(0, 20, Color::White(), "Enemy Stamina: %.1f / %.1f (%s)",
		_stamina.GetCurrent(), _stamina.GetMax(), _stamina.IsExhausted() ? "EXHAUSTED" : "OK");
}