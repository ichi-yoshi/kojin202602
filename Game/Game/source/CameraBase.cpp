#include "CameraBase.h"
#include "ApplicationMain.h"

CameraBase::CameraBase()
{
	_vPos = VGet(0.0f, 90.0f, -300.0f);
	_vTarget = VGet(0.0f, 60.0f, 0.0f);
	_clipFar = 10000.0f;
	_clipNear = 2.0f;
	_camYaw = 0.0f;
	_camPitch = 0.0f;
}

bool CameraBase::Initialize()
{
    // カメラの初期設定
    _vPos = VGet(0.0f, 90.0f, -300.0f);
    _vTarget = VGet(0.0f, 60.0f, 0.0f);
    _clipNear = 2.0f;
    _clipFar = 10000.0f;
    _camYaw = 0.0f;
    _camPitch = 0.0f; 

	// マウスカーソルを非表示にする
    SetMouseDispFlag(FALSE);

    return true;
}

bool CameraBase::Terminate()
{
    return true;
}

void CameraBase::Update(VECTOR playerPos, int& key)
{
    HandleMouseInput();
    UpdateFPSCamera(playerPos);
    _mouse.ResetMousePointCenter();
}

void CameraBase::HandleMouseInput()
{
	// マウスの移動量を取得してYawとPitchを更新
	_mouse.MousePointMovement();

    // カメラ回転(マウス移動で制御)FPS視点用
    _camYaw += _mouse.GetYaw();
    _camPitch -= _mouse.GetPitch();

    // ピッチ制限
    if (_camPitch < -1.2f) _camPitch = -1.2f;
    if (_camPitch > 1.2f) _camPitch = 1.2f;
}

void CameraBase::UpdateFPSCamera(VECTOR playerPos)
{
    // FPS視点のカメラ位置と向き
    _vPos = VAdd(playerPos, VGet(0, FPS_CAMERA_HEIGHT, 0)); // プレイヤーの頭部

    VECTOR forward = VGet(
        cos(_camPitch) * sin(_camYaw),
        sin(_camPitch),
        cos(_camPitch) * cos(_camYaw)
    );

    _vTarget = VAdd(_vPos, forward); // 視線方向
}

void CameraBase::ApplyCamera()
{
    // カメラ設定更新
    SetCameraPositionAndTarget_UpVecY(_vPos, _vTarget);
    SetCameraNearFar(_clipNear, _clipFar);
}

VECTOR CameraBase::GetDirection() const
{
    return VGet(sin(_camYaw), 0, cos(_camYaw));
}
