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

private:
	// 敵の初期設定値
	static constexpr float INITIAL_SPEED = 8.5f;						// 敵の初期移動速度
	static constexpr float STAMINA_MAX = 100.0f;						// 敵の最大スタミナ
	static constexpr float STAMINA_RECOVERY_RATE = 0.05f;				// 敵のスタミナ回復率
	static constexpr float STAMINA_COST_RATE = 0.01f;					// 敵のスタミナ消費率
	static constexpr VECTOR INITIAL_POS = { 1000.0f, 0.0f, -300.0f };	// 敵の初期位置

	// A*経路探索の設定値
	static constexpr float CELL_SIZE = 15.0f;		// A*グリッドの1マスのサイズ
	static constexpr float CLOSE_RANGE = 50.0f;		// 近距離判定の距離
	static constexpr float STOP_RANGE = 10.0f;		// 停止判定の距離
	static constexpr int MAX_ATTEMPTS = 60;			// 安全な位置を探す最大試行回数
	static constexpr int RECALC_INTERVAL = 40;		// 経路再計算のフレーム間隔

	static constexpr int MIN_SPAWN_DIST = 200.0f;	// プレイヤーからの最小出現距離
	static constexpr int RAND_SPAWN_DIST = 150.0f;	// プレイヤーからのランダム出現距離
};

