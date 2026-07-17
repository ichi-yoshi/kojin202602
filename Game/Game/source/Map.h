#pragma once
#include "appframe.h"

class Map
{
public:
	Map();
	~Map() { Terminate(); }

	bool Initialize();
	void Terminate();
	void Render();

	bool CheckCollision(const VECTOR& startPos, float colSubY, VECTOR& outHitPos) const;

	void SetCollisionVisible(bool visible);

	VECTOR GetMinPosition() const;
	VECTOR GetMaxPosition() const;
private:
	int _handleMap;
	int _handleSkySphere;
	int _frameMapCollision;

	VECTOR lineStart;
	VECTOR lineEnd;
};

