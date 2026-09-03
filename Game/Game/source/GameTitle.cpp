#include "GameTitle.h"

GameTitle::GameTitle()
{
	_titleGraph = -1;
	_isFinished = false;
}

GameTitle::~GameTitle()
{
	Terminate();
}

bool GameTitle::Initialize()
{
	// タイトル画面の画像をロード
	//_titleGraph = LoadGraph("assets/title.png");
	_isFinished = false;
	return true;
}

void GameTitle::Terminate()
{
	// タイトル画面の画像を解放
	if(_titleGraph != -1)
	{
		DeleteGraph(_titleGraph);
		_titleGraph = -1;
	}
}

void GameTitle::Update()
{
	// キー入力をチェックして、タイトル画面を終了する条件を満たしたら_isFinishedをtrueにする
	if(CheckHitKey(KEY_INPUT_SPACE))
	{
		_isFinished = true;
	}
}

void GameTitle::Render()
{
	// タイトル画面の描画
	if(_titleGraph != -1)
	{
		DrawGraph(0, 0, _titleGraph, TRUE);
	}

	//デバッグ用
	DrawString(320, 400, "PRESS SPACE KEY TO START", GetColor(255, 255, 255));
}