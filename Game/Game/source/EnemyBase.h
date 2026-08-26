#pragma once
#include <vector>
#include "appframe.h"
#include "AStarPathfinder.h"
#include "Stamina.h"
#include "Score.h"

struct EnemyData 
{
	float speed; // 敵の移動速度
	float staminaMax; // 敵の最大スタミナ
	float staminaRecoveryRate; // 敵のスタミナ回復率
	float staminaCostRate; // 敵のスタミナ消費率
	VECTOR initialPos; // 敵の初期位置
	const char* imagePath; // 敵の画像パス
};

class EnemyBase
{
public:
	EnemyBase(const EnemyData&data);
	virtual ~EnemyBase() = default;

	virtual void Initialize(const Map& map);
	virtual void Update(const Map& map, VECTOR playerPos, Score& score);
	virtual void Render();

	void SetupAStar(const Map& map);
	bool IsInScreenCenter(float targetRadiusPixels=100.0f);
	void AttackToPlayer(VECTOR playerPos, Score& score);

	bool IsExhausted() const { return _stamina.IsExhausted(); }
	VECTOR GetPos() const { return _pos; }
protected:
	VECTOR _pos;
	float _speed;
	int _imageHandle;

	AStarPathfinder _pathfinder;
	std::vector<VECTOR> _path;
	int _pathIndex;
	Stamina _stamina;
	EnemyData _param;

protected:
	static constexpr float CELL_SIZE = 15.0f;
	static constexpr float CLOSE_RANGE = 50.0f;		// 近距離判定の距離
	static constexpr float STOP_RANGE = 10.0f;		// 停止判定の距離
	static constexpr int MAX_ATTEMPTS = 60;			// 安全な位置を探す最大試行回数
	static constexpr int RECALC_INTERVAL = 40;		// 経路再計算のフレーム間隔

	static constexpr int MIN_SPAWN_DIST = 200.0f;	// プレイヤーからの最小出現距離
	static constexpr int RAND_SPAWN_DIST = 150.0f;	// プレイヤーからのランダム出現距離

};

