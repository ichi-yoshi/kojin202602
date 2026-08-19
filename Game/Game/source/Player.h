#pragma once
#include "appframe.h"
#include "MouseInput.h"
#include "Stamina.h"

class Map;
class CameraBase;

class Player
{
public:
	Player();
	~Player() { Terminate(); }

	void Initialize();
	void Terminate();

	void Update(CameraBase& camera, const Map& map);
	void Render();

	VECTOR GetPosition() const { return _vPos; }
	bool IsViewCollision() const { return _bViewCollision; }

	enum class STATUS
	{
		NONE,
		WAIT,
		WALK,
		_EOT_
	};
private:
	// 内部処理
	VECTOR CalculateMovementVector(CameraBase& camera, int key);

	// コリジョン判定を回して実際に移動させる
	void MoveWithCollision(const Map& map, const VECTOR& baseVelocity, float camrad);

	// アニメーションの更新
	void UpdateAnimation(STATUS oldStatus);

	// デバッグ機能の更新
	void UpdateDebugInput(int trg);
	
private:
	int _handle;		// モデルハンドル
	int _attachIndex;	// アニメーションのアタッチインデックス
	float _totalTime;	// アニメーションの総再生時間
	float _playTime;	// アニメーションの再生時間

	VECTOR _vPos;       // 位置
	VECTOR _vDir;       // 向き
	float _colSubY;     // コリジョン判定時のY補正(腰位置）

	VECTOR _vVelocity;  // 現在の移動速度ベクトル（X, Z成分を使用）

	STATUS _status;
	MouseInput _mouseInput;
	Stamina _stamina;

	// デバッグ用
	bool _bViewCollision;

private:
	static constexpr float MAX_SPEED = 6.0f;       // 最高速度
	static constexpr float DASH_SPEED = 10.0f;     // ダッシュ時の最高速度
	static constexpr float ACCEL = 1.0f;           // 加速度（毎フレームどれだけ加速するか）
	static constexpr float DECEL = 0.6f;           // 減速度（キーを離したとき、毎フレームどれだけ減速するか）

	static constexpr float STAMINA_MAX = 30.0f;				// 最大スタミナ
	static constexpr float STAMINA_RECOVERY_RATE = 0.1f;	// スタミナ回復率
	static constexpr float STAMINA_COST_RATE = 0.3f;		// スタミナ消費率

	static constexpr float ANIMATION_SPEED = 0.5f;	// アニメーションの再生速度
	static constexpr VECTOR INITIAL_POS = { 100.0f, 0.0f, 0.0f };	// プレイヤーの初期位置
};

