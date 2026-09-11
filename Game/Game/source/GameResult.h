#pragma once
#include "Score.h"
#include "Timer.h"

class GameResult
{
public:
	void Render(const Score& score);

	// 画面の中央に配置するボックスを描画する関数
	void DrawResultCenterBox(int resultX, int resultY, int resultWidth, int resultHeight);

	// リザルトの内容を描画する関数
	void DrawResultContent(int resultX, int resultY, int resultWidth, int resultHeight, const Score& score);

	void StartNextGame();
	bool IsNextGame() { return _isNextGame; };

private:
	int _isNextGame = false;
};

