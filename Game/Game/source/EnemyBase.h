#pragma once
#include <vector>
#include <memory>
#include "appframe.h"
#include "AStarPathfinder.h"
#include "Stamina.h"
#include "Score.h"
#include "Timer.h"

struct EnemyData 
{
	float speed;				// 移動速度
	float staminaMax; 			// 最大スタミナ
	float staminaRecoveryRate;	// スタミナ回復率
	float staminaCostRate;		// スタミナ消費率
	VECTOR initialPos;			// 初期位置
	const char* imagePath;		// 画像パス
};

class EnemyBase
{
public:
	EnemyBase(const EnemyData& data);
	virtual ~EnemyBase() = default;

	virtual void Initialize(const Map& map);
	virtual void Update(const Map& map, VECTOR playerPos, Score& score);
	virtual void Render();

	// A*経路探索のセットアップ
	void SetupAStar(const Map& map);

	// プレイヤーの周囲に安全な位置を探して再出現させる
	bool IsInScreenCenter(float targetRadiusPixels = 100.0f);

	// プレイヤーに攻撃する処理
	void AttackToPlayer(VECTOR playerPos, Score& score);

	// 攻撃中かどうかを返す
	bool IsAttacking() const { return _isAttacking; }

	// 攻撃中フラグをリセットする
	bool ResetAttacking() { _isAttacking = false; return _isAttacking; }

	// スタミナが尽きているかどうかを返す
	bool IsExhausted() const { return _stamina.IsExhausted(); }

	// 敵の現在位置を返す
	VECTOR GetPos() const { return _pos; }

	// プレイヤーの周囲に安全な位置を探して再出現させる
	virtual bool SetRandomSpawnPos(const Map& map, VECTOR playerPos);

	// 敵同士の衝突判定を行い、衝突している場合は位置を調整する
	void EnenmyCollision(const std::vector<std::unique_ptr<EnemyBase>>& otherEnemies);

protected:
	VECTOR _pos;		// 敵の位置
	float _speed;		// 敵の移動速度
	int _imageHandle;	// 敵の画像ハンドル
	int _recalcTimer;	// 経路再計算のタイマー
	bool _isAttacking;

	AStarPathfinder _pathfinder;
	std::vector<VECTOR> _path;
	int _pathIndex;
	Stamina _stamina;
	EnemyData _param;
	Timer FlashTimer;

protected:
	static constexpr float CELL_SIZE = 15.0f;		// A*探索のグリッド1マスのサイズ
	static constexpr float CLOSE_RANGE = 50.0f;		// 近距離判定の距離
	static constexpr float STOP_RANGE = 10.0f;		// 停止判定の距離
	static constexpr int MAX_ATTEMPTS = 60;			// 安全な位置を探す最大試行回数
	static constexpr int RECALC_INTERVAL = 40;		// 経路再計算のフレーム間隔
	static constexpr float REACH_DISTANCE = 10.0f;	// 経路上の到達判定距離
	static constexpr int MIN_SPAWN_DIST = 400.0f;	// プレイヤーからの最小出現距離
	static constexpr int RAND_SPAWN_DIST = 250.0f;	// プレイヤーからのランダム出現距離
	static constexpr float ENEMY_RADIUS = 20.0f;	// 敵の半径（コリジョン判定用）
};

