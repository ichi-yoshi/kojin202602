#pragma once
#include "appframe.h"

// 2D座標やサイズをまとめた名前空間
namespace Layout
{
	struct Point { int x, y; }; // 2D座標を表す構造体
	struct Size { int w, h; };  // 幅と高さを表す構造体

	constexpr Size Screen = { 1920, 1080 }; // 画面サイズを定義
}

// 色を取得するための関数をまとめた名前空間
namespace Color
{
	inline unsigned int Yellow()	{ return GetColor(255, 255, 0); }    // 黄色
	inline unsigned int Red()		{ return GetColor(255, 0, 0); }      // 赤色
	inline unsigned int LightRed()	{ return GetColor(255, 50, 50); }    // 明るい赤色
	inline unsigned int WeakRed()	{ return GetColor(255, 120, 120); }  // 弱い赤色
	inline unsigned int Gold()		{ return GetColor(255, 215, 0); }    // 金色
	inline unsigned int White()		{ return GetColor(255, 255, 255); }  // 白色
	inline unsigned int Gray()		{ return GetColor(150, 150, 150); }  // 灰色
	inline unsigned int DarkGray()	{ return GetColor(100, 100, 100); }  // 濃い灰色
	inline unsigned int TextGray()	{ return GetColor(200, 200, 200); }  // テキスト用の灰色
	inline unsigned int DimGray()	{ return GetColor(128, 128, 128); }  // 薄い灰色
	inline unsigned int Green()		{ return GetColor(0, 255, 0); }      // 緑色
	inline unsigned int Blue()		{ return GetColor(0, 0, 255); }      // 青色
	inline unsigned int LightBlue() { return GetColor(120, 200, 255); }  // 明るい青色		
	inline unsigned int Black()		{ return GetColor(0, 0, 0); }        // 黒色
	inline unsigned int Dim()		{ return GetColor(20, 20, 20); }     // 半透明暗色背景
}

// フォントサイズの定数を定義する名前空間
namespace Font
{
	constexpr int Small = 16;       // 小
	constexpr int Normal = 18;      // 標準
	constexpr int Medium = 20;      // 中
	constexpr int Large = 24;       // 大
	constexpr int Title = 32;       // タイトル用
	constexpr int ExtraLarge = 36;  // さらに大きい
}

// 透明度をまとめた名前空間
namespace Alpha
{
	constexpr int Max = 255;	// 最大透明度
	constexpr int Min = 0;		// 最小透明度
	constexpr int Dim = 200;	// 薄暗い透明度
	constexpr int Semi = 128;	// 半透明
	constexpr int Light = 100;	// 明るい透明度
	constexpr int Dark = 50;	// 暗い透明度
}