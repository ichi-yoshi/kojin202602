#pragma once
#include "appframe.h"
#include <algorithm>

class Gauge
{	
public:
	Gauge();
	~Gauge() = default;

	void SetPosition(int x, int y, bool isCenterMode = false);
	void SetSize(int width, int height);
	void SetColor(int fillColor, int borderColor, int bgColor);
	void SetHasBorder(bool hasBorder);
	void Render(float rate) const;

private:
	int _x;
	int _y;
	int _width;
	int _height;
	int _fillColor;
	int _borderColor;
	int _bgColor;
	bool _isCenterMode;
	bool _hasBorder;
};

