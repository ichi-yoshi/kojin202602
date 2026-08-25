#include "Score.h"
#include "AppFrame.h"
#include "MagicNumberConfig.h"

Score::Score()
{
	gameScore = 0;
}

void Score::Initialize()
{
	// スコアの初期化処理
	gameScore = 0;
}

void Score::Update()
{
	// スコアの更新処理（必要に応じて）
}

void Score::Render()
{
	// スコアの描画処理
	DrawFormatString(1120, 20, Color::White(), "SCORE: %d", gameScore);
}

void Score::AddScore(int points)
{
	// スコアを加算
	gameScore += points;
}

void Score::ResetScore()
{
	// スコアをリセット
	gameScore = 0;
}