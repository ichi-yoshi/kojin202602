#include "MouseInput.h"
#include <cmath>

MouseInput::MouseInput()
{
	_vMove = VGet(0, 0, 0);
	_isMoving = false;
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
