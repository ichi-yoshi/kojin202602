#include "Enemy.h"
#include "Map.h"
#include "Resource.h"

Enemy::Enemy() 
{
	_pos = VGet(0, 0, 0);
	_speed = 8.5f;
	_imageHandle = -1;
	_pathIndex = 0;
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
		}

		// 足元の床高さに吸着
		VECTOR hitPos;
		if(map.CheckCollision(_pos, 40.f, hitPos)) { _pos.y = hitPos.y; }
		return; // 近距離処理が終わったらここでUpdateを抜ける
	}

	// 遠距離時の従来のA*処理 ---

	static int recalcTimer = 0;
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
		}
	}

	// 足元の床高さに吸着
	VECTOR hitPos;
	if(map.CheckCollision(_pos, 40.f, hitPos))
	{
		_pos.y = hitPos.y;
	}
}

void Enemy::Render()
{
	//デバッグ用
	// A*の床グリッドやルート線画を表示
	if(CheckHitKey(KEY_INPUT_SPACE)) 
	{
		_pathfinder.DebugRender();
	}
	
	// 敵キャラの位置に画像（ビルボード）を描画
	if(_imageHandle != -1)
	{
		VECTOR renderPos = _pos;
		renderPos.y += 70.0f;	

		// 3D空間上の敵の座標に、カメラを常に向く画像（ビルボード）を描画する
		DrawBillboard3D(renderPos, 0.5f, 0.5f, 200.0f, 0.0f, _imageHandle, TRUE);
	}

	// デバッグ用
	DrawFormatString(0, 0, GetColor(255, 255, 255), "Enemy Pos: (%.2f, %.2f, %.2f)", _pos.x, _pos.y, _pos.z);
}