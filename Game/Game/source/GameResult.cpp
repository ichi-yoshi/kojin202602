#include "GameResult.h"
#include "MagicNumberConfig.h"
#include <string>

void GameResult::DrawResultCenterBox(int resultX, int resultY, int resultWidth, int resultHeight) 
{
    // リザルト背景（黒）
    DrawBox(resultX, resultY, resultX + resultWidth, resultY + resultHeight, Color::Black(), TRUE);

    // 枠線
    DrawBox(resultX, resultY, resultX + resultWidth, resultY + resultHeight, Color::White(), FALSE);
}

void GameResult::DrawResultContent(int resultX, int resultY, int resultWidth, int resultHeight, const Score& score)
{
	int finalScore = score.GetScore();
	const char* resultTextMsg = "Your Score:";
    int resultText = Color::Red();

    //テキスト描画
	SetFontSize(Font::Large);
	int textWidth = GetDrawStringWidth(resultTextMsg, strlen(resultTextMsg));
	int textX = resultX + (resultWidth - textWidth) / 2;
	DrawString(textX, resultY + 20, resultTextMsg, resultText);

	// スコア描画
	SetFontSize(Font::ExtraLarge);
	int scoreWidth = GetDrawStringWidth(std::to_string(finalScore).c_str(), std::to_string(finalScore).length());
	int scoreX = resultX + (resultWidth - scoreWidth) / 2;
	DrawString(scoreX, resultY + 60, std::to_string(finalScore).c_str(), resultText);
}

void GameResult::Render(const Score& score)
{
    int screenWidth, screenHeight;
    GetScreenState(&screenWidth, &screenHeight, NULL);

    // 画面中央に配置するためのサイズと座標設定
    int boxWidth = 600;
    int boxHeight = 200;
    int boxX = (screenWidth - boxWidth) / 2;
    int boxY = (screenHeight - boxHeight) / 2;

    DrawResultCenterBox(boxX, boxY, boxWidth, boxHeight);
    DrawResultContent(boxX, boxY, boxWidth, boxHeight, score); 
}