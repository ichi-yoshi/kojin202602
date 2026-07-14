#pragma once
#include "DxLib.h" 

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

private:
	VECTOR _vMove;      // 計算された移動ベクトル
	bool   _isMoving;   // 移動入力中かどうかのフラグ
};

