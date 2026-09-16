#include "Gauge.h"
#include "MagicNumberConfig.h"

Gauge::Gauge() 
{
	_x = 0;
	_y = 0;
	_width = 0;
	_height = 0;
	_fillColor = 0;
	_borderColor = 0;
	_bgColor = 0;
	_isCenterMode = false;
	_hasBorder = true;
}

void Gauge::SetPosition(int x, int y, bool isCenterMode)
{
	_x = x;
	_y = y;
	_isCenterMode = isCenterMode;
}

void Gauge::SetSize(int width, int height)
{
	_width = width;
	_height = height;
}

void Gauge::SetColor(int fillColor, int borderColor, int bgColor)
{
	_fillColor = fillColor;
	_borderColor = borderColor;
	_bgColor = bgColor;
}

void Gauge::SetHasBorder(bool hasBorder)
{
	_hasBorder = hasBorder;
}

void Gauge::Render(float rate) const 
{
	// rateを0.0〜1.0の範囲に制限
	float clampRate = std::clamp(rate, 0.0f, 1.0f);

	int drawX = _isCenterMode ? (_x - _width / 2) : _x;

	DrawBox(drawX, _y, drawX + _width, _y + _height, _bgColor, TRUE);

	// ゲージの幅を計算して描画
	int currentWidth = static_cast<int>(_width * clampRate);
	if(currentWidth > 0)
	{
		DrawBox(drawX, _y, drawX + currentWidth, _y + _height, _fillColor, TRUE);
	}

	if(_hasBorder)
	{
		DrawBox(drawX, _y, drawX + _width, _y + _height, _borderColor, FALSE);
	}
}