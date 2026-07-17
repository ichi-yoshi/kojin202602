#pragma once
#include "appframe.h"
#include <vector>
#include "AStarPathfinder.h"

class Enemy
{
public:
    Enemy();
	~Enemy() = default;
    void Initialize(const Map& map);
    void Update(const Map& map, VECTOR playerPos);
    void Render();

private:
    VECTOR _pos;
    float _speed;
    int _imageHandle; 

    AStarPathfinder _pathfinder;
    std::vector<VECTOR> _path;
    int _pathIndex;
};

