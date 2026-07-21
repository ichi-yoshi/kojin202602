#pragma once
#include "appframe.h"
#include <vector>
#include <queue>
#include <cmath>

class Map;

struct Node
{
	int x=0, z=0;			//グリッド上のインデックス
	VECTOR worldPos=VGet(0, 0, 0);	//実際の3D空間上の位置(Y座標は床の高さ)
	bool isWalkable=false;	//歩行可能かどうか
	bool isValid = false;

	float gScore=999999.0f;		//スタートからの実コスト
	float hScore=0.0f;		//ゴールまでの推定コスト
	float fScore() const { return gScore + hScore; }

	Node* parent=nullptr;		//経路復元用の親ノードへのポインタ

	
};

class AStarPathfinder
{
public:
	AStarPathfinder();
	~AStarPathfinder()=default;

	// マップの床情報からグリッドを構築する
	void BuildGridFromMap(const Map& map,VECTOR origin, float spacing, int width, int length);

	// A*アルゴリズムによる経路探索
	std::vector<VECTOR> FindPath(VECTOR startPos, VECTOR goalPos);	

	// デバッグ用描画
	void DebugRender();
private:
	//グリッド上の座標(x,z)から1次元配列のインデックスを取得する
	int GetIndex(int x, int z) const;	
	
	//ノード間の距離を計算する（ユークリッド距離）
	float CalculateDistance(const Node& a, const Node& b) const;

	//ワールド座標からグリッド上のノードを取得する
	Node* GetNodeAtWorld(VECTOR worldPos);

private:
	int _width;		//グリッドの幅（X方向のマス数）
	int _length;	//グリッドの長さ（Z方向のマス数）
	float _spacing;	//グリッドの1マスのサイズ（ワールド座標上の距離）
	VECTOR _origin;	//グリッドの原点（ワールド座標上の左上隅）

	std::vector<Node> _grid;	//グリッドのノード配列
	std::vector<VECTOR> _lastCalculatePath;	//デバッグ表示用
};

