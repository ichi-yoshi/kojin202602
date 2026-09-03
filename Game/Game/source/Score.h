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

	int GetScore() const { return gameScore; }
private:
	int gameScore = 0; // ゲームスコアを保持する変数
};

