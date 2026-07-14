#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include "Resource.h"

bool ModeGame::Initialize() 
{
	if (!base::Initialize()) { return false; }

	_cam.Initialize();

	// モデルデータのロード（テクスチャも読み込まれる）
	_handle = MV1LoadModel(mv1::Chara);
	_attach_index = -1;		// アニメーションアタッチはされていない
	// ステータスを「無し」に設定
	_status = STATUS::NONE;
	// 再生時間の初期化
	_total_time = 0.f;
	_play_time = 0.0f;
	// 位置,向きの初期化
	_vPos = VGet(100, 0, 0);
	_vDir = VGet(0, 0, -1);		// キャラモデルはデフォルトで-Z方向を向いている

	// マップ
	_handleSkySphere = MV1LoadModel(mv1::SkySphere);
	_handleMap = MV1LoadModel(mv1::Map);
	_frameMapCollision = MV1SearchFrame(_handleMap, collision::MapCollision);

	// コリジョン情報の生成
	MV1SetupCollInfo(_handleMap, _frameMapCollision, 16, 16, 16);
	// コリジョンのフレームを描画しない設定
	MV1SetFrameVisible(_handleMap, _frameMapCollision, FALSE);
	// 腰位置の設定
	_colSubY = 40.f;

	// その他初期化
	_bViewCollision = TRUE;

	return true;
}

bool ModeGame::Terminate()
{
	base::Terminate();
	return true;
}

bool ModeGame::Process() 
{
	base::Process();
	
	// キャラ移動処理
	CharaMovement();
	return true;
}

bool ModeGame::CharaMovement()
{
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// 処理前のステータスを保存しておく
	STATUS oldStatus = _status;

	_cam.Update(_vPos, key);

	// マウスホイールの回転量取得
	int wheel = GetMouseWheelRotVol();

	// キャラ移動(カメラ設定に合わせて)
	// カメラの向いている角度を取得
	VECTOR camPos = _cam.GetPosition();
	VECTOR camTarget = _cam.GetTarget();
	float sx = camPos.x - camTarget.x;
	float sz = camPos.z - camTarget.z;
	float camrad = atan2(sz, sx);

	// 移動量の初期化
	float mvSpeed = 6.f;

	_mouseInput.Update(key, camrad, mvSpeed);
	VECTOR v = _mouseInput.GetMovementVector();

	// 移動前の位置を保存
	VECTOR oldvPos = _vPos;
	VECTOR oldv = v;

	// コリジョン判定で引っかかった時に、escapeTbl[]順に角度を変えて回避を試みる
	float escapeTbl[] =
	{
		0, -10, 10, -20, 20, -30, 30, -40, 40, -50, 50, -60, 60, -70, 70, -80, 80,
	};

	// 元の入力から基準となるラジアンを逆算（コリジョン回避スライド計算用）
	float rad = atan2(oldv.z, oldv.x) - camrad;

	for(int i = 0; i < sizeof(escapeTbl) / sizeof(escapeTbl[0]); i++)
	{
		// 入力がない（移動していない）場合はコリジョン補正のループを回す必要がない
		if(!_mouseInput.IsMoving()) { break; }

		// escapeTbl[i]の分だけ移動量v回転
		float escape_rad = DEG2RAD(escapeTbl[i]);
		v.x = cos(rad + camrad + escape_rad) * mvSpeed;
		v.z = sin(rad + camrad + escape_rad) * mvSpeed;

		// vの分移動
		_vPos = VAdd(_vPos, v);

		// 移動した先でコリジョン判定
		MV1_COLL_RESULT_POLY hitPoly;

		// 主人公の腰位置から下方向への直線
		hitPoly = MV1CollCheck_Line(_handleMap, _frameMapCollision,
			VAdd(_vPos, VGet(0, _colSubY, 0)), VAdd(_vPos, VGet(0, -99999.f, 0)));
		if(hitPoly.HitFlag)
		{
			// 当たった
			_vPos.y = hitPoly.HitPosition.y;
			v.y += _vPos.y - oldvPos.y;
			break;
		}
		else
		{
			// 当たらなかった。元の座標に戻す
			_vPos = oldvPos;
			v = oldv;
		}
	}

	// 移動量をそのままキャラの向きにする
	if(VSize(v) > 0.f)
	{		// 移動していない時は無視するため
		_vDir = v;
		_status = STATUS::WALK;
	}
	else
	{
		_status = STATUS::WAIT;
	}

	// デバッグ機能
	if(trg & PAD_INPUT_1)
	{
		_bViewCollision = !_bViewCollision;
	}
	if(_bViewCollision)
	{
		MV1SetFrameVisible(_handleMap, _frameMapCollision, FALSE);
	}
	else
	{
		MV1SetFrameVisible(_handleMap, _frameMapCollision, TRUE);
	}

	// ステータスが変わっていないか？
	if(oldStatus == _status)
	{
		// 再生時間を進める
		_play_time += 0.5f;
	}
	else
	{
		// アニメーションがアタッチされていたら、デタッチする
		if(_attach_index != -1)
		{
			MV1DetachAnim(_handle, _attach_index);
			_attach_index = -1;
		}
		// ステータスに合わせてアニメーションのアタッチ
		switch(_status)
		{
		case STATUS::WAIT:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "idle"), -1, FALSE);
			break;
		case STATUS::WALK:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "run"), -1, FALSE);
			break;
		}
		// アタッチしたアニメーションの総再生時間を取得する
		_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
		// 再生時間を初期化
		_play_time = 0.0f;
	}

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if(_play_time >= _total_time)
	{
		_play_time = 0.0f;
	}
	return true;
}

bool ModeGame::Render()
{
	base::Render();

	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);
	SetUseBackCulling(TRUE);

	// ライト設定
	SetUseLighting(TRUE);
#if 0	// 平行ライト
	SetGlobalAmbientLight(GetColorF(0.5f, 0.f, 0.f, 0.f));
	ChangeLightTypeDir(VGet(-1, -1, 0));
#endif
#if 1	// ポイントライト
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(_vPos,VGet(0,50.f,0)), 1000.f, 0.f, 0.005f, 0.f);
#endif

	// カメラ設定更新
	_cam.ApplyCamera();

	// 再生時間をセットする
	MV1SetAttachAnimTime(_handle, _attach_index, _play_time);

	// モデルを描画する
	{
		// 位置
		MV1SetPosition(_handle, _vPos);
		// 向きからY軸回転を算出
		VECTOR vRot = { 0,0,0 };
		vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);		// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
		MV1SetRotationXYZ(_handle, vRot);
		// 描画
		MV1DrawModel(_handle);

		// コリジョン判定用ラインの描画
		if (_bViewCollision)
		{
			DrawLine3D(VAdd(_vPos, VGet(0, _colSubY, 0)), VAdd(_vPos, VGet(0, -99999.f, 0)), GetColor(255, 0, 0));
		}
	}

	// マップモデルを描画する
	{
		MV1DrawModel(_handleMap);
		//MV1DrawModel(_handleSkySphere);
	}

	return true;
}

