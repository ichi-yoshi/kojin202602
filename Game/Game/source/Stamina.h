#pragma once

class Stamina
{
public:
	Stamina();
	~Stamina() = default;

	// スタミナの初期化
	void Initialize(float maxStamina, float recoveryRate, float costRate);

	// スタミナの消費
	void Consume(float amount);

	// スタミナの回復
	void Recover();

	// 毎フレームの更新処理（スタミナの回復や疲労状態の管理）
	void Update();

	// スタミナの状態を取得するゲッター
	bool IsExhausted() const { return _isExhausted; };	// 疲労状態（スタミナ切れ）かどうか
	float GetCurrent() const { return _current; };		// 現在のスタミナ
	float GetMax() const { return _max; };				// 最大スタミナ	
	float GetRate() const { return (_max > 0.0f) ? (_current / _max) : 0.0f; }	// スタミナの割合（0.0〜1.0）

private:
	float _current;     // 現在のスタミナ
	float _max;         // 最大スタミナ
	float _recoveryRate;  // 1フレームあたりの回復量
	float _costRate;      // 消費倍率
	bool _isExhausted;   // 疲労状態（スタミナ切れ）フラグ
};

