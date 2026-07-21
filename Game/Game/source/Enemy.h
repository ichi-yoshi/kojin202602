#pragma once
#include "appframe.h"
#include <vector>
#include "AStarPathfinder.h"

class Enemy
{
public:
    Enemy();
	~Enemy() = default;
    void Initialize(const Map& map);
    void Update(const Map& map, VECTOR playerPos);
    void Render();

	void SetupAStar(const Map& map);
	
private:
	VECTOR _pos;        // 敵の現在位置
	float _speed;       // 敵の移動速度
	int _imageHandle;   // 敵の画像ハンドル

	AStarPathfinder _pathfinder;    // A*経路探索用のオブジェクト
	std::vector<VECTOR> _path;      // 計算された経路の座標リスト
	int _pathIndex;	// 現在の経路上のターゲットインデックス
};

