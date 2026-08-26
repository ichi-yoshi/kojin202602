#pragma once


class Score
{
public:
	Score();
	~Score() = default;

	void Initialize();

	void Update();

	void Render();

	void AddScore(int points);

	void SubtractScore(int points);

	void ResetScore();

private:
	int gameScore = 0; // ゲームスコアを保持する変数
};

