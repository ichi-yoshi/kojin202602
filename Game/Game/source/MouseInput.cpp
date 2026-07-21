#include "MouseInput.h"
#include <cmath>

MouseInput::MouseInput()
{
	_vMove = VGet(0, 0, 0);
	_isMoving = false;
	_prevMouseX = -1;
	_prevMouseY = -1;
	_mouseSensitivity = 0.002f;
	_Yaw = 0.0f;
	_Pitch = 0.0f;
}

void MouseInput::Update(int key, float camrad, float mvSpeed)
{
	// 入力方向の決定
	VECTOR v = VGet(0, 0, 0);
	_isMoving = false;

	//WASDキーの入力に応じて移動方向を設定
	if(key & PAD_INPUT_5) { v.x = 1; }
	if(key & PAD_INPUT_8) { v.x = -1; }
	if(key & PAD_INPUT_4) { v.z = -1; }
	if(key & PAD_INPUT_6) { v.z = 1; }

	// 入力があった場合のみ、カメラの向きに合わせて回転・移動ベクトルを計算する
	float length = 0.0f;
	if(VSize(v) > 0.f)
	{
		length = mvSpeed;
		_isMoving = true;
	}

	float rad = atan2(v.z, v.x);
	_vMove.x = cos(rad + camrad) * length;
	_vMove.y = 0.0f;
	_vMove.z = sin(rad + camrad) * length;
}

void MouseInput::MousePointMovement()
{
	// マウス移動量取得
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// 初回時の前回位置設定
	if(_prevMouseX == -1)
	{
		_prevMouseX = mouseX;
		_prevMouseY = mouseY;
	}

	// マウスの移動量を計算
	int deltaX = mouseX - _prevMouseX;
	int deltaY = mouseY - _prevMouseY;
	_prevMouseX = mouseX;
	_prevMouseY = mouseY;

	_Yaw = deltaX * _mouseSensitivity;
	_Pitch = deltaY * _mouseSensitivity;
}

void MouseInput::ResetMousePointCenter() 
{
	// 毎フレーム中央に戻す
	SetMousePoint(SCREEN_CENTER_X, SCREEN_CENTER_Y);
	_prevMouseX = SCREEN_CENTER_X;
	_prevMouseY = SCREEN_CENTER_Y;
}