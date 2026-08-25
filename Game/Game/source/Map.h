#pragma once
#include "appframe.h"

class Map
{
public:
	Map();
	~Map() { Terminate(); }

	void Initialize();
	void Terminate();
	void Render();

	// コリジョン判定(スタート位置、コリジョン判定用のY座標補正値、衝突位置を返す)
	bool CheckCollision(const VECTOR& startPos, float colSubY, VECTOR& outHitPos) const;

	// コリジョン判定用のフレームを可視化するかどうか
	void SetCollisionVisible(bool visible);

	// マップを取得する
	VECTOR GetMinPosition() const;	// 最小座標
	VECTOR GetMaxPosition() const;	// 最大座標
private:
	int _handleMap;			// マップモデルのハンドル
	int _handleSkySphere;	// 空のモデルのハンドル
	int _frameMapCollision;	// マップのコリジョン判定用のフレームハンドル

	VECTOR lineStart;	// 線の開始位置
	VECTOR lineEnd;		// 線の終了位置
};

