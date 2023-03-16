#include"Bossdamage.h"
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeBoss.h"

Bossdamage::Bossdamage()
{
	_effectResourceHandle = LoadEffekseerEffect("res/Beaker/Beaker_damage01/Beaker_damage01_effect.efkmat.efkefc");
	IsPlaying = -1;
	damageEfsize = 100.0f;
}

Bossdamage::~Bossdamage()
{
}

void Bossdamage::Update(Boss& boss)
{
	Effekseer_Sync3DSetting();
	if (boss.bosshitEf == true&& IsPlaying == -1)
	{
		_playingEffectHandle = PlayEffekseer3DEffect(_effectResourceHandle);
		SetScalePlayingEffekseer3DEffect(_playingEffectHandle, damageEfsize, damageEfsize, damageEfsize);
	}
	IsPlaying = IsEffekseer3DEffectPlaying(_playingEffectHandle);
	SetPosPlayingEffekseer3DEffect(_playingEffectHandle, boss.model.pos.x, boss.model.pos.y, boss.model.pos.z);
}

void Bossdamage::Render(Boss& boss)
{
	DrawEffekseer3D();
	boss.bosshitEf = false;
}
