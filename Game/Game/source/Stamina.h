#pragma once

class Stamina
{
public:
	Stamina();
	~Stamina() = default;

	void Initialize(float maxStamina, float recoveryRate, float costRate);

	void Consume(float amount);

	void Recover();

	void Update();

	bool IsExhausted() const { return _isExhausted; };
	float GetCurrent() const { return _current; };
	float GetMax() const { return _max; };
	float GetRate() const { return (_max>0.0f)?(_current/_max):0.0f; }

	void SetCurrent(float val);

private:
	float _current = 100.0f;     // 現在のスタミナ
	float _max = 100.0f;         // 最大スタミナ
	float _recoveryRate = 0.3f;  // 1フレームあたりの回復量
	float _costRate = 1.0f;      // 消費倍率
	bool _isExhausted = false;   // 疲労状態（スタミナ切れ）フラグ
};

