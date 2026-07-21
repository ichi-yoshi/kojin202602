#include "Map.h"
#include "Resource.h"

Map::Map() 
{
	_handleMap = -1;
	_handleSkySphere = -1;
	_frameMapCollision = -1;
	lineStart = VGet(0, 0, 0);
	lineEnd = VGet(0, 0, 0);
}

void Map::Initialize()
{
	// モデルロード
	_handleSkySphere = MV1LoadModel(mv1::SkySphere);
	_handleMap = MV1LoadModel(mv1::Map);
	
	_frameMapCollision = MV1SearchFrame(_handleMap, collision::MapCollision);

	// コリジョン情報の生成
	MV1SetupCollInfo(_handleMap, _frameMapCollision, 16, 16, 16);

	// コリジョンのフレームを描画しない設定
	MV1SetFrameVisible(_handleMap, _frameMapCollision, FALSE);
   
}

void Map::Terminate()
{
	if(_handleMap != -1)
	{
		MV1DeleteModel(_handleMap);
		_handleMap = -1;
	}

	if(_handleSkySphere != -1)
	{
		MV1DeleteModel(_handleSkySphere);
		_handleSkySphere = -1;
	}
}

void Map::Render() 
{
	// マップ描画
	if(_handleMap != -1) 
	{
		MV1DrawModel(_handleMap);
	}

	// スカイスフィア描画
	/*if(_handleSkySphere != -1)
	{
		MV1DrawModel(_handleSkySphere);
	}*/
}

bool Map::CheckCollision(const VECTOR& startPos, float colSubY, VECTOR& outHitPos) const
{
	if(_handleMap == -1 || _frameMapCollision == -1) { return false; }

	// コリジョン判定用のラインを設定
	VECTOR lineStart = VAdd(startPos, VGet(0.0f, colSubY, 0.0f));
	VECTOR lineEnd = VAdd(startPos, VGet(0.0f, -9999.0f, 0.0f));

	// ラインとコリジョンの交差判定
	MV1_COLL_RESULT_POLY hitPoly = MV1CollCheck_Line(_handleMap, _frameMapCollision, lineStart, lineEnd);

	if(hitPoly.HitFlag)
	{
		outHitPos = hitPoly.HitPosition;
		return true;
	}
	return false;
}

// コリジョン判定用のフレームを可視化するかどうか
void Map::SetCollisionVisible(bool visible)
{
	if(_handleMap != -1 && _frameMapCollision != -1)
	{
		// コリジョン判定用のフレームの可視化設定
		MV1SetFrameVisible(_handleMap, _frameMapCollision, visible ? FALSE : TRUE);
	}
}

// マップを取得する
// 最小座標
VECTOR Map::GetMinPosition() const 
{
	MV1_REF_POLYGONLIST refPoly = MV1GetReferenceMesh(_handleMap, -1, TRUE);
	return refPoly.MinPosition;
}

// 最大座標
VECTOR Map::GetMaxPosition() const 
{
	MV1_REF_POLYGONLIST refPoly = MV1GetReferenceMesh(_handleMap, -1, TRUE);
	return refPoly.MaxPosition;
}