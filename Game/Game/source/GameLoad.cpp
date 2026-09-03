#include "GameLoad.h"
#include "MagicNumberConfig.h"
#include "ResourceManager.h"

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
}