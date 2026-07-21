#include "AStarPathfinder.h"
#include "Map.h"
#include <algorithm>

AStarPathfinder::AStarPathfinder() 
{
	_width = 0;
	_length = 0;
	_spacing = 0.0f;
	_origin = VGet(0, 0, 0);
}

int AStarPathfinder::GetIndex(int x, int z) const
{
	// グリッドの範囲外の場合は -1 を返す
	if(x < 0 || x >= _width || z < 0 || z >= _length) { return -1; }
	return z * _width + x;
}

// マップの床情報からグリッドを構築する
void AStarPathfinder::BuildGridFromMap(const Map& map, VECTOR origin, float spacing, int width, int length) 
{
	_origin = origin;
	_spacing = spacing;
	_width = width;
	_length = length;
	_grid.clear();
	_grid.resize(width * length);
	
	// パス1：床があるマスを判定してグリッドに登録
	for(int z = 0; z < _length; ++z)
	{
		for(int x = 0; x < _width; ++x) 
		{
			int index = GetIndex(x, z);
			_grid[index].x = x;
			_grid[index].z = z;

			float worldX = origin.x + x * spacing;
			float worldZ = origin.z + z * spacing;
			VECTOR worldPos = VGet(worldX, origin.y, worldZ);

			VECTOR hitPos;

			// 床の高さを取得するために、上方向にレイを飛ばして床との衝突判定を行う
			if(map.CheckCollision(VAdd(worldPos, VGet(0.0f, 100.0f, 0.0f)), 10.0f, hitPos)) 
			{
				_grid[index].worldPos = hitPos;
				_grid[index].isWalkable = true;
				_grid[index].isValid = true;
			}
			else
			{
				_grid[index].worldPos = worldPos;
				_grid[index].isWalkable = false;
				_grid[index].isValid = false;
			}
		}
	}

	// パス2：床がないマスのうち、周囲8マスに床があるマスだけを復活させる
	int dx[] = { 0, 0, -1, 1, -1, 1, -1, 1 };
	int dz[] = { -1, 1, 0, 0, -1, -1, 1, 1 };

	// 処理中に自分自身で書き換えたデータを参照しないよう、一時的にパス1の状態をコピー
	std::vector<Node> tempGrid = _grid;

	// パス2の処理
	for(int z = 0; z < _length; ++z)
	{
		for(int x = 0; x < _width; ++x)
		{
			int index = GetIndex(x, z);
			if(tempGrid[index].isValid) continue; // 既に床がある場所はスルー

			// 周囲8マスに床があるかチェック
			bool nearFloor = false;
			for(int i = 0; i < 8; ++i)
			{
				int nx = x + dx[i];
				int nz = z + dz[i];
				int nIdx = GetIndex(nx, nz);
				if(nIdx != -1 && tempGrid[nIdx].isWalkable)
				{
					nearFloor = true;
					break;
				}
			}

			// 床がないマスのうち、周囲に床がある場合は壁や落下判定としてグリッドに含める
			if(nearFloor)
			{
				// 床の隣なので、落下判定や壁判定としてグリッドに「含める」
				_grid[index].isValid = true;
				//_grid[index].isWalkable = false; // ただし床はないので歩けない
			}
			// 周囲に何もない「完全な虚無」は _grid[index].isValid が false のまま残り、
			// 以降の A* や描画から完全に無視されます。
		}
	}
}

// ワールド座標からグリッド上のノードを取得する
Node* AStarPathfinder::GetNodeAtWorld(VECTOR worldPos)
{
	int x = static_cast<int>((worldPos.x - _origin.x) / _spacing);
	int z = static_cast<int>((worldPos.z - _origin.z) / _spacing);
	int index = GetIndex(x, z);
	if(index == -1) { return nullptr; }
	return &_grid[index];
}

// ノード間の距離を計算する（ユークリッド距離）
float AStarPathfinder::CalculateDistance(const Node& a, const Node& b) const 
{
	return VSize(VSub(b.worldPos, a.worldPos));
}

// A*アルゴリズムによる経路探索
std::vector<VECTOR> AStarPathfinder::FindPath(VECTOR starWorld, VECTOR goalWorld) 
{
	std::vector<VECTOR> path;
	_lastCalculatePath.clear();

	// ワールド座標からスタートノードとゴールノードを取得
	Node* startNode = GetNodeAtWorld(starWorld);
	Node* goalNode = GetNodeAtWorld(goalWorld);

	// スタートノードまたはゴールノードが無効な場合は経路探索を中止
	if(!startNode||!goalNode||!startNode->isWalkable||!goalNode->isWalkable||!startNode->isValid||!goalNode->isValid)
	{
		return path;
	}

	// A*アルゴリズムの初期化
	for(auto& node : _grid) 
	{
		node.gScore = 999999.0f;
		node.hScore = 0.0f;
		node.parent = nullptr;
	}

	// オープンリストとクローズドリストを使用して探索
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	// スタートノードをオープンリストに追加
	startNode->gScore = 0.0f;
	startNode->hScore=CalculateDistance(*startNode, *goalNode);
	openList.push_back(startNode);

	// A*探索ループ
	while(!openList.empty()) 
	{
		// オープンリストからfScoreが最小のノードを取得
		auto currentIt = std::min_element(openList.begin(), openList.end(), [](const Node* a, const Node* b)
			{
				return a->fScore() < b->fScore(); 
			});

		Node* currentNode = *currentIt;

		// ゴールノードに到達した場合、経路を復元して返す
		if(currentNode == goalNode) 
		{
			Node* current = currentNode;
			while(current != nullptr) 
			{
				path.push_back(current->worldPos);
				current = current->parent;
			}
			std::reverse(path.begin(), path.end());	// 経路を逆順にしてスタートからゴールへ
			_lastCalculatePath = path;
			return path;
		}

		// 現在のノードをオープンリストから削除し、クローズドリストに追加
		openList.erase(currentIt);
		closedList.push_back(currentNode);

		// 隣接ノードの探索
		int dx[] = { 0,0,-1,1,-1,1,-1,1 };
		int dz[] = { -1,1,0,0,-1,-1,1,1 };

		for(int i = 0; i < 8; ++i)
		{
			int nx = currentNode->x + dx[i];
			int nz = currentNode->z + dz[i];
			int nIdx = GetIndex(nx, nz);

			if(nIdx == -1) continue;
			Node* neighbor = &_grid[nIdx];

			// 隣接ノードが無効、通行不可、またはクローズドリストにある場合はスキップ
			if(!neighbor->isValid || !neighbor->isWalkable ||
				std::find(closedList.begin(), closedList.end(), neighbor) != closedList.end())
			{
				continue;
			}

			// 隣接ノードのgScoreを計算し、オープンリストに追加または更新
			float tentativeGScore = currentNode->gScore + CalculateDistance(*currentNode, *neighbor);

			// すでにオープンリストにあるかどうかを確認
			auto openIt = std::find(openList.begin(), openList.end(), neighbor);

			// オープンリストにない場合、または新しいgScoreが小さい場合は更新
			if(openIt == openList.end() || tentativeGScore < neighbor->gScore)
			{
				neighbor->parent = currentNode;
				neighbor->gScore = tentativeGScore;
				neighbor->hScore = CalculateDistance(*neighbor, *goalNode);

				if(openIt == openList.end())
				{
					openList.push_back(neighbor);
				}
			}
		}
	}
	return path;
}

void AStarPathfinder::DebugRender()
{
	// 全グリッドマスの床位置を可視化（緑＝通行可能、赤＝通行不可）
	for(const auto& node : _grid)
	{
		if(!node.isValid) continue; // 無効なマスは描画しない
		unsigned int color = node.isWalkable ? GetColor(0, 150, 0) : GetColor(150, 0, 0);
		DrawSphere3D(node.worldPos, 3.0f, 3, color, color, TRUE);
	}

	// 計算された最終ルートを太い青線で描画
	if(_lastCalculatePath.size() > 1)
	{
		for(size_t i = 0; i < _lastCalculatePath.size() - 1; ++i)
		{
			// 床の少し上に浮かせて線を描画
			VECTOR p1 = VAdd(_lastCalculatePath[i], VGet(0, 5.0f, 0));
			VECTOR p2 = VAdd(_lastCalculatePath[i + 1], VGet(0, 5.0f, 0));
			DrawLine3D(p1, p2, GetColor(0, 100, 255));
		}
	}
}