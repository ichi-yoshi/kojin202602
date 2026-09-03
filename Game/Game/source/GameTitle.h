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

	bool IsFinished() const { return _isFinished; }
private:
	bool _isFinished;
	int _titleGraph;
};

