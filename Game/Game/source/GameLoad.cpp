#include "GameLoad.h"
#include "MagicNumberConfig.h"
#include "ResourceManager.h"
#include "Resource.h"

void GameLoad::Update()
{
	// ロード画面の更新処理（必要に応じて）
	// ここでリソースのロードを開始する
	ResourceManager::GetInstance().LoadResource();
}

void GameLoad::Render()
{
	// デバッグ用
	// ロード中の画面描画
	DrawString(100, 100, "NOW LOADING...", Color::White());
	DrawFormatString(20, 20, Color::White(), "AsyncLoad Left: %d", GetASyncLoadNum());

	DrawBox(200, 580, 200 + 800, 620, Color::Gray(), FALSE);

	// 非同期ロードの進捗を取得
	int loadNum = GetASyncLoadNum();
	int maxLoadNum = 6; // 最大ロード数（仮の値）

	// ロードの進捗に応じて緑色のバーを描画
	double progress = static_cast<double>(maxLoadNum - loadNum) / static_cast<double>(maxLoadNum);

	// 進捗率を0.0から1.0の範囲に制限
	if(progress < 0.0) progress = 0.0;
	if(progress > 1.0) progress = 1.0;
	int barWidth = static_cast<int>(800.0 * progress);

	DrawBox(200, 580, 200 + barWidth, 620, Color::Green(), TRUE);
}
