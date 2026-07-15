#include "Player.h"
#include "Map.h"
#include "CameraBase.h"
#include "ApplicationMain.h"
#include "Resource.h"
#include <cmath>

Player::Player()
{
	_handle = -1;
	_attachIndex = -1;
	_totalTime = -1;
	_playTime = -1;
	_vPos = VGet(0, 0, 0);
	_vDir = VGet(0, 0, 0);
	_vVelocity = VGet(0, 0, 0);
	_colSubY = 0.0f;
	_status = STATUS::NONE;
	_bViewCollision = false;
}

bool Player::Initialize()
{
	_handle = MV1LoadModel(mv1::Chara);
	if(_handle == -1) { return false; }
	_attachIndex = -1;
	_totalTime = 0.0f;
	_playTime = 0.0f;
	_vPos = VGet(100, 0, 0);
	_vDir = VGet(0, 0, -1);
	_vVelocity = VGet(0, 0, 0);
	_colSubY = 40.0f;
	_status = STATUS::NONE;
	_bViewCollision = true;
	return true;
}

void Player::Terminate()
{
	if(_handle != -1)
	{
		MV1DeleteModel(_handle);
		_handle = -1;
	}
}

void Player::Update(CameraBase& camera, const Map& map) 
{
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// 状態の退避とカメラの追従更新
	STATUS oldStatus = _status;
	camera.Update(_vPos, key);

	// カメラの角度を取得
	VECTOR camPos = camera.GetPosition();
	VECTOR camTarget = camera.GetTarget();
	float sx = camPos.x - camTarget.x;
	float sz = camPos.z - camTarget.z;
	float camrad = atan2(sz, sx);

	// 入力から移動ベクトルを計算
	VECTOR baseVelocity = CalculateMovementVector(camera, key);

	// コリジョン判定を回して実際に移動させる
	MoveWithCollision(map, baseVelocity, camrad);

	// アニメーションの更新
	UpdateAnimation(oldStatus);

	// デバッグ機能の更新
	UpdateDebugInput(trg);
}

VECTOR Player::CalculateMovementVector(CameraBase& camera, int key) 
{
	// カメラの角度を取得
	VECTOR camPos = camera.GetPosition();
	VECTOR camTarget = camera.GetTarget();
	float camrad = atan2(camPos.z - camTarget.z, camPos.x - camTarget.x);

	_mouseInput.Update(key, camrad, MAX_SPEED);

	return _mouseInput.GetMovementVector();
}

void Player::MoveWithCollision(const Map& map, const VECTOR& baseVelocity, float camrad)
{
	VECTOR oldvPos = _vPos;

	// 【加減速ロジック】現在の速度ベクトル(_vVelocity)を目標(baseVelocity)に近づける
	if(VSize(baseVelocity) > 0.0f)
	{
		// 入力がある場合：目標の方向に向けて加速
		// ※ baseVelocity の長さはすでに最高速度(MAX_SPEED)になっている前提
		VECTOR vDiff = VSub(baseVelocity, _vVelocity);
		float diffLen = VSize(vDiff);

		if(diffLen <= ACCEL)
		{
			_vVelocity = baseVelocity; // 差がわずかなら目標値にする
		}
		else
		{
			// 目標に向けて ACCEL の分だけ速度を近づける
			_vVelocity = VAdd(_vVelocity, VScale(VNorm(vDiff), ACCEL));
		}
	}
	else
	{
		// 入力がない場合：摩擦や空気抵抗のように減速
		float currentSpeed = VSize(_vVelocity);
		if(currentSpeed <= DECEL)
		{
			_vVelocity = VGet(0, 0, 0); // ほぼ止まっていれば完全に停止
		}
		else
		{
			// 現在進んでいる方向と逆向きに力をかけて減速する
			_vVelocity = VSub(_vVelocity, VScale(VNorm(_vVelocity), DECEL));
		}
	}

	// コリジョン回避用の計算には、この「加減速計算後の現在の速度」を使う
	VECTOR v = _vVelocity;
	float currentSpeedXZ = VSize(VGet(v.x, 0.0f, v.z));

	float escapeTbl[] =
	{
		0, -10, 10, -20, 20, -30, 30, -40, 40, -50, 50, -60, 60, -70, 70, -80, 80,
	};

	// 現在の移動ベクトルの角度を求める
	float rad = atan2(v.z, v.x) - camrad;

	for(int i = 0; i < sizeof(escapeTbl) / sizeof(escapeTbl[0]); i++)
	{
		// 速度が 0 の時はコリジョン判定を回さない
		if(currentSpeedXZ <= 0.01f) { break; }

		float escape_rad = DEG2RAD(escapeTbl[i]);
		v.x = cos(rad + camrad + escape_rad) * currentSpeedXZ;
		v.z = sin(rad + camrad + escape_rad) * currentSpeedXZ;

		_vPos = VAdd(_vPos, v);

		VECTOR hitPosition;
		if(map.CheckCollision(_vPos, _colSubY, hitPosition))
		{
			_vPos.y = hitPosition.y;
			v.y += _vPos.y - oldvPos.y;
			break;
		}
		else
		{
			_vPos = oldvPos;
			v = _vVelocity; // 元の速度に戻す
		}
	}

	// 移動結果に基づいて向きとアニメーションステータスを確定
	// ※ 速度がごくわずかでもあるならWALK状態にする（閾値を少し設けると不自然にアニメーションが切り替わるのを防げます）
	if(currentSpeedXZ > 0.1f)
	{
		_vDir = _vVelocity;
		_status = STATUS::WALK;
	}
	else
	{
		_status = STATUS::WAIT;
	}
}

void Player::UpdateAnimation(STATUS oldStatus)
{
	if(oldStatus == _status)
	{
		_playTime += 0.5f;
	}
	else
	{
		if(_attachIndex != -1)
		{
			MV1DetachAnim(_handle, _attachIndex);
			_attachIndex = -1;
		}

		switch(_status)
		{
		case STATUS::WAIT:
			_attachIndex = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "idle"), -1, FALSE);
			break;
		case STATUS::WALK:
			_attachIndex = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "run"), -1, FALSE);
			break;
		}
		_totalTime = MV1GetAttachAnimTotalTime(_handle, _attachIndex);
		_playTime = 0.0f;
	}

	if(_playTime >= _totalTime)
	{
		_playTime = 0.0f;
	}
}

void Player::UpdateDebugInput(int trg)
{
	if(trg & PAD_INPUT_1)
	{
		_bViewCollision = !_bViewCollision;
	}
}

void Player::Render()
{
	if(_handle == -1) return;

	// アニメーション再生時間の設定
	MV1SetAttachAnimTime(_handle, _attachIndex, _playTime);

	// 位置と回転
	MV1SetPosition(_handle, _vPos);
	VECTOR vRot = { 0.0f, 0.0f, 0.0f };
	vRot.y = atan2(_vDir.x * -1.0f, _vDir.z * -1.0f);
	MV1SetRotationXYZ(_handle, vRot);

	MV1DrawModel(_handle);

	// デバッグ用コリジョンラインの描画
	if(_bViewCollision)
	{
		DrawLine3D(VAdd(_vPos, VGet(0.0f, _colSubY, 0.0f)), VAdd(_vPos, VGet(0.0f, -99999.0f, 0.0f)), GetColor(255, 0, 0));
	}
}