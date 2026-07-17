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

	void BuildGridFromMap(const Map& map,VECTOR origin, float spacing, int width, int length);

	std::vector<VECTOR> FindPath(VECTOR startPos, VECTOR goalPos);

	void DebugRender();
private:
	int GetIndex(int x, int z) const;
	float CalculateDistance(const Node& a, const Node& b) const;
	Node* GetNodeAtWorld(VECTOR worldPos);

private:
	std::vector<Node> _grid;
	int _width;
	int _length;
	float _spacing;
	VECTOR _origin;		
	std::vector<VECTOR> _lastCalculatePath;	//デバッグ表示用
};

