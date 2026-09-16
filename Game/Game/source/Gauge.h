#pragma once
#include "appframe.h"
#include <algorithm>

class Gauge
{	
public:
	Gauge();
	~Gauge() = default;

	// 描画位置、サイズ、色、枠線の有無を設定する関数
	void SetPosition(int x, int y, bool isCenterMode = false);

	// ゲージのサイズを設定する関数
	void SetSize(int width, int height);

	// ゲージの色を設定する関数
	void SetColor(int fillColor, int borderColor, int bgColor);

	// 枠線の有無を設定する関数
	void SetHasBorder(bool hasBorder);

	// ゲージを描画する関数
	void Render(float rate) const;

private:
	int _x;				// 描画位置X座標
	int _y;				// 描画位置Y座標
	int _width;			// 幅
	int _height;		// 高さ
	int _fillColor;		// 塗りつぶし色
	int _borderColor;	// 枠線色
	int _bgColor;		// 背景色
	bool _isCenterMode;	// 描画位置を中央基準にするかどうか
	bool _hasBorder;	// 枠線を描画するかどうか
};

