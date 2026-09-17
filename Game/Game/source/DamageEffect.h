#pragma once
#include "appframe.h"
#include "Timer.h"

class DamageEffect
{
public:
	DamageEffect();
	~DamageEffect() = default;

	void Trigger(); // ダメージを受けたときに呼び出す

	void Update(float deltaTime); // 毎フレーム呼び出す

	void Render(); // フラッシュ効果を描画する
private:
	Timer _flashTimer;
	float _duration; // フラッシュ効果の持続時間
};

