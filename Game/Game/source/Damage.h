#pragma once
#include "SANclass.h"
#include "LKAclass.h"


class Damage {
public:
	Damage();
	~Damage();
	virtual void Initialize(SAN* san, LKA* lka);
	virtual void Terminate();
	virtual void Process();
	virtual void Render();

	int SanHP;
	int LkaHP;
	int MaxSanHP;
	int MaxLkaHP;

	float SanCoolTime;
	float LkaCoolTime;

	bool SanHitFlag;
	bool LkaHitFlag;

	bool stageFlag;//trueの時にダメージ床の判定がONになる

	float Distance;

	int stageHandle;

protected:
	SAN* San;
	LKA* Lka;
};