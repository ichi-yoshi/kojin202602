#include "Stamina.h"

Stamina::Stamina()
{
	_current = 0.0f;
	_max = 0.0f;
	_recoveryRate = 0.0f;
	_costRate = 0.0f;
	_isExhausted = false;
}

void Stamina::Initialize(float maxStamina, float recoveryRate, float costRate)
{
	_max = maxStamina;
	_recoveryRate = recoveryRate;
	_costRate = costRate;
	_current = _max; // 初期化時にスタミナを最大値に設定
	_isExhausted = false;
}

void Stamina::Consume(float amount)
{
	if(_isExhausted) return; // 疲労状態なら消費しない

	_current -= amount * _costRate;	// 消費量に倍率をかけてスタミナを減少させる
	if(_current < 0.0f)
	{
		_current = 0.0f;
		_isExhausted = true; // スタミナが0になったら疲労状態にする
	}
}

void Stamina::Recover() 
{
	if(_current < _max)
	{
		_current += _recoveryRate;	// 回復量を加算
		if(_current > _max)
		{
			_current = _max;
			_isExhausted = false; // スタミナが回復したら疲労状態を解除
		}
	}
}