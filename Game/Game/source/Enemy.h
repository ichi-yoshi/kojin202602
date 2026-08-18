#pragma once
#include "appframe.h"
#include <vector>
#include "AStarPathfinder.h"
#include "Stamina.h"

class Enemy
{
public:
    Enemy();
	~Enemy() = default;
    void Initialize(const Map& map);
    void Update(const Map& map, VECTOR playerPos);
    void Render();

	// A*経路探索のセットアップ
	void SetupAStar(const Map& map);
	
	// 敵が画面中央の範囲内にいるかを判定する
	bool IsInScreenCenter(float targetRadiusPixels = 100.0f);
private:
	VECTOR _pos;        // 敵の現在位置
	float _speed;       // 敵の移動速度
	int _imageHandle;   // 敵の画像ハンドル

	AStarPathfinder _pathfinder;    // A*経路探索用のオブジェクト
	std::vector<VECTOR> _path;      // 計算された経路の座標リスト
	int _pathIndex;	// 現在の経路上のターゲットインデックス
	Stamina _stamina; // 敵のスタミナ
};

