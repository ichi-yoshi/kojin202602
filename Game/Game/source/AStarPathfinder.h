#pragma once
#include "appframe.h"
#include <vector>
#include <queue>
#include <cmath>
#include <unordered_set>

class Map;

struct Node
{
	int x, z;			//グリッド上のインデックス
	VECTOR worldPos;	//実際の3D空間上の位置(Y座標は床の高さ)
	bool isWalkable;	//歩行可能かどうか
	bool isValid;		//グリッドとして有効かどうか（床がないマスは無効）
	float moveCost;		//追加コスト（崖際などの危険なマスにペナルティを付与する場合に使用）

	float gScore;		//スタートからの実コスト
	float hScore;		//ゴールまでの推定コスト
	float fScore() const { return gScore + hScore; }	//総コスト（f = g + h）

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

	// 指定したワールド座標が歩行可能なエリア（ノード）か判定する
	bool IsWalkableWorldPos(VECTOR& worldPos);

	// デバッグ用描画
	void DebugRender();
private:
	//グリッド上の座標(x,z)から1次元配列のインデックスを取得する
	int GetIndex(int x, int z) const;	
	
	//ノード間の距離を計算する（ユークリッド距離）
	float CalculateDistance(const Node& a, const Node& b) const;

	//ワールド座標からグリッド上のノードを取得する
	Node* GetNodeAtWorld(VECTOR worldPos);

	//ゴールノードが通行不可の場合、最寄りの通行可能なノードを探索する
	Node* FindNearestWalkableNode(Node* targetNode);
private:
	int _width;		//グリッドの幅（X方向のマス数）
	int _length;	//グリッドの長さ（Z方向のマス数）
	float _spacing;	//グリッドの1マスのサイズ（ワールド座標上の距離）
	VECTOR _origin;	//グリッドの原点（ワールド座標上の左上隅）

	std::vector<Node> _grid;	//グリッドのノード配列
	std::vector<VECTOR> _lastCalculatePath;	//デバッグ表示用

	static constexpr float MAX_COST = 999999.0f;	//初期化時の最大コスト
	static constexpr float EDGE_PENALTY = 50.0f;	//崖際などのマスに追加するペナルティコスト
};

