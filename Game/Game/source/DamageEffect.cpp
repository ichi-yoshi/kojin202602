#include "DamageEffect.h"
#include "MagicNumberConfig.h"

DamageEffect::DamageEffect() 
{
	_duration = 0.5f; // フラッシュ効果の持続時間を設定
}


void DamageEffect::Trigger()
{
	_flashTimer.Start(_duration); // フラッシュ効果を開始
}

void DamageEffect::Update(float deltaTime)
{
	_flashTimer.Update(deltaTime); // タイマーを更新
}

void DamageEffect::Render()
{
	if(_flashTimer.IsRunning())
	{
		// フラッシュ効果のアルファ値を計算（0.0〜1.0の範囲）
		float alphaRatio = 1.0f - _flashTimer.GetProgress();
		int alpha = static_cast<int>(Alpha::Semi * alphaRatio);

		// 赤色のフラッシュを描画
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawBox(0, 0, Layout::Screen.w, Layout::Screen.h, Color::Red(), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
}