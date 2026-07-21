#pragma once
#include "appframe.h"
#include "MouseInput.h"

// 計算用マクロ
#define	PI	(3.1415926535897932386f)
#define	DEG2RAD(x)			( ((x) / 180.0f ) * PI )
#define	RAD2DEG(x)			( ((x) * 180.0f ) / PI )

class CameraBase
{
public:
    CameraBase();
    ~CameraBase() = default;

    // 初期化・終了処理
    bool Initialize();
    bool Terminate();

    // 更新処理
    void Update(VECTOR playerPos, int& key);

    // カメラ設定の適用
    void ApplyCamera();

    // アクセサ
    VECTOR GetPosition() const { return _vPos; }
    VECTOR GetTarget() const { return _vTarget; }
    VECTOR GetDirection() const;
    float GetYaw() const { return _camYaw; }
    float GetPitch() const { return _camPitch; }

    // 設定
    void SetPosition(VECTOR pos) { _vPos = pos; }
    void SetTarget(VECTOR target) { _vTarget = target; }
    void SetClip(float nearClip, float farClip) { _clipNear = nearClip; _clipFar = farClip; }

private:
    // カメラパラメータ
    VECTOR _vPos;                   // 位置
    VECTOR _vTarget;                // 注視点
    float _clipNear, _clipFar;      // クリップ距離
    float _camYaw, _camPitch;       // カメラの角度
   
    // FPS視点用パラメータ
    float FPS_CAMERA_HEIGHT = 100.0f;  // カメラの高さ

    // 内部処理
    void HandleMouseInput();
    void UpdateFPSCamera(VECTOR playerPos);

	MouseInput _mouse; // マウス入力処理用オブジェクト
};

