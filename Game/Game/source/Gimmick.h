#pragma once
#include "AppFrame.h"
#include "SANclass.h"
#include "LKAclass.h"
#include <string>

class Gimmick {
public:
	Gimmick();
	void Initialize();
	void Terminate();
	void Process();
	void Render();

	void Balance(VECTOR SanPos, VECTOR LkaPos);

	int BalanceHandle;

	int AttachAnim1;
	int AttachAnimSAN;
	int AttachAnimLKA;

	VECTOR SANDisk;
	VECTOR LKADisk;

	float PlayBalance;
	float TotalBalance;
	float BlendRate;
	float nowBlendRate;

	bool SanHitFlag;
	bool LkaHitFlag;

protected:
	enum class BALANCE {
		EQUAL,
		SAN,
		LKA,
		NONE
	};

	BALANCE balance;

private:
	Player player;
	SAN san;
	LKA lka;
};