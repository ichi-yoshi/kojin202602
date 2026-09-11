#pragma once
#include "appframe.h"

class GameTitle
{
public:
	GameTitle();
	~GameTitle();

	bool Initialize();
	void Terminate();
	void Update();
	void Render();

	// タイトル画面が終了したかどうかを返す関数
	bool IsFinished() const { return _isFinished; }

private:
	bool _isFinished;	// タイトル画面が終了したかどうかのフラグ
	int _titleGraph;	// タイトル画面の画像ハンドル
};

