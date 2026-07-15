#include "CameraBase.h"
#include "ApplicationMain.h"

CameraBase::CameraBase()
{
	_vPos = VGet(0.0f, 90.0f, -300.0f);
	_vTarget = VGet(0.0f, 60.0f, 0.0f);
	_clipFar = 10000.0f;
	_clipNear = 2.0f;
	_prevMouseX = -1;
	_prevMouseY = -1;
	_camYaw = 0.0f;
	_camPitch = 0.0f;
	_mouseSensitivity = 0.002f;
}

CameraBase::~CameraBase()
{

}

bool CameraBase::Initialize()
{
    // カメラの初期設定
    _vPos = VGet(0.0f, 90.0f, -300.0f);
    _vTarget = VGet(0.0f, 60.0f, 0.0f);
    _clipNear = 2.0f;
    _clipFar = 10000.0f;

    // マウス関連の初期化
    _prevMouseX = -1;
    _prevMouseY = -1;
    _camYaw = 0.0f;
    _camPitch = 0.0f;

	// マウス感度の設定
	_mouseSensitivity = 0.002f;

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
    ResetMouseToCenter();
}

void CameraBase::HandleMouseInput()
{
    // マウス移動量取得
    int mouseX, mouseY;
    GetMousePoint(&mouseX, &mouseY);

    // 初回時の前回位置設定
    if (_prevMouseX == -1)
    {
        _prevMouseX = mouseX;
        _prevMouseY = mouseY;
    }

    int deltaX = mouseX - _prevMouseX;
    int deltaY = mouseY - _prevMouseY;
    _prevMouseX = mouseX;
    _prevMouseY = mouseY;

    // カメラ回転(マウス移動で制御)FPS視点用
    _camYaw += deltaX * _mouseSensitivity;
    _camPitch -= deltaY * _mouseSensitivity;

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

void CameraBase::ResetMouseToCenter()
{
    // 毎フレーム中央に戻す
    SetMousePoint(SCREEN_CENTER_X, SCREEN_CENTER_Y);
    _prevMouseX = SCREEN_CENTER_X;
    _prevMouseY = SCREEN_CENTER_Y;
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
