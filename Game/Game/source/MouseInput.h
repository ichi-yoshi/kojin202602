#pragma once
#include "appframe.h"

class MouseInput
{
public:
	// 初期化
	MouseInput();

	// 毎フレームの入力更新処理（キー、カメラの向き、移動スピードを渡す）
	void Update(int key, float camrad, float mvSpeed);

	// 計算された移動ベクトル（回転適用済み）を取得するゲッター
	VECTOR GetMovementVector() const { return _vMove; }

	// 移動しようとしているかどうか（キー入力があったか）
	bool IsMoving() const { return _isMoving; }

	void MousePointMovement();

	float GetMouseSensitivity() const { return _mouseSensitivity; }
	float GetYaw() const { return _Yaw; }
	float GetPitch() const { return _Pitch; }

	void ResetMousePointCenter();
private:
	//キャラ移動用
	VECTOR _vMove;      // 計算された移動ベクトル
	bool   _isMoving;   // 移動入力中かどうかのフラグ

	// マウス制御用
	int _prevMouseX;	// 前回のマウス位置
	int _prevMouseY;   
	float _mouseSensitivity; // マウス感度

	float _Yaw; // マウスの水平移動量
	float _Pitch; // マウスの垂直移動量

	int SCREEN_CENTER_X = 1920 / 2;       // 画面中央X座標
	int SCREEN_CENTER_Y = 1080 / 2;       // 画面中央Y座標
};

