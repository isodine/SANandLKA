#pragma once
#include "AppFrame.h"
#include "SANclass.h"
#include "LKAclass.h"
#include <string>

class SAN;
class LKA;

class Gimmick {
public:
	Gimmick();
	void Initialize();
	void Terminate();
	void Process();
	void Render();

	void SetSanLka(SAN* San, LKA* Lka) {
		san = San;
		lka = Lka;
	}
	float GetPolyMaxY(MV1_COLL_RESULT_POLY* Dim, int num);
	void Balance(VECTOR SanPos, VECTOR LkaPos);

	bool GetSanHitFlag() { return SanHitFlag; }
	bool GetLkaHitFlag() { return LkaHitFlag; }

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
	bool BalanceFlag;



protected:
	enum class BALANCE {
		EQUAL,
		SAN,
		LKA,
		NONE
	};

	BALANCE balance;

private:
	SAN* san;
	LKA* lka;
};