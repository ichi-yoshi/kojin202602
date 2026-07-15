#pragma once
#include "appframe.h"
#include "MouseInput.h"

class Map;
class CameraBase;

class Player
{
public:
	enum class STATUS
	{
		NONE,
		WAIT,
		WALK,
		_EOT_
	};

	Player();
	~Player() { Terminate(); }

	bool Initialize();
	void Terminate();

	void Update(CameraBase& camera, const Map& map);
	void Render();

	VECTOR GetPosition() const { return _vPos; }
	bool IsViewCollision() const { return _bViewCollision; }

private:
	VECTOR CalculateMovementVector(CameraBase& camera, int key);
	void MoveWithCollision(const Map& map, const VECTOR& baseVelocity, float camrad);
	void UpdateAnimation(STATUS oldStatus);
	void UpdateDebugInput(int trg);
	
private:
	int _handle;
	int _attachIndex;
	float _totalTime;
	float _playTime;

	VECTOR _vPos;       // 位置
	VECTOR _vDir;       // 向き
	float _colSubY;     // コリジョン判定時のY補正(腰位置）

	VECTOR _vVelocity;  // 現在の移動速度ベクトル（X, Z成分を使用）

	const float MAX_SPEED = 6.0f;       // 最高速度
	const float ACCEL = 1.0f;           // 加速度（毎フレームどれだけ加速するか）
	const float DECEL = 0.6f;           // 減速度（キーを離したとき、毎フレームどれだけ減速するか）

	STATUS _status;
	MouseInput _mouseInput;

	// デバッグ用
	bool _bViewCollision;
};

