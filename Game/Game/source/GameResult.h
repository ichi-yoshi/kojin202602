#pragma once
#include "Score.h"
#include "Timer.h"

class GameResult
{
public:
	void Render(const Score& score);

	void DrawResultCenterBox(int resultX, int resultY, int resultWidth, int resultHeight);

	void DrawResultContent(int resultX, int resultY, int resultWidth, int resultHeight, const Score& score);
};

