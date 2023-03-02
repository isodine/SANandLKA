#include "Damage.h"
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "time.h"
#include "Include.h"


Damage::Damage(){

}

Damage::~Damage() {

}

void Damage::Initialize(SAN* san, LKA* lka) {
	San = san;
	Lka = lka;
  
	SanHP = 10;
	LkaHP = 10;

	Distance = 1000;

	SanCoolTime = 0;
	LkaCoolTime = 0;

	SanHitFlag = false;
	LkaHitFlag = false;
}

void Damage::Terminate() {
	
}

void Damage::Process() {
	
	San->DamageProcess();
	Lka->DamageProcess();

	Distance = VSize(VSub(VGet(Lka->vPos.x, Lka->vPos.y + 50, Lka->vPos.z), VGet(San->vPos.x, San->vPos.y + 50, San->vPos.z)));

	if (Distance < 85 && SanHitFlag == false && LkaHitFlag == false) {
		SanHP -= 2;
		LkaHP -= 2;
		SanHitFlag = true;
		LkaHitFlag = true;
	}
	else if (Distance < 110 && Distance >= 85 && SanHitFlag == false && LkaHitFlag == false) {
		SanHP -= 1;
		LkaHP -= 1;
		SanHitFlag = true;
		LkaHitFlag = true;
	}
	if (SanHitFlag == true) {
		SanCoolTime += 1;
	}
	
	if (LkaHitFlag == true) {
		LkaCoolTime += 1;
	}

	if (SanCoolTime >= 60) {
		SanHitFlag = false;
		SanCoolTime = 0;
	}

	if (LkaCoolTime >= 60) {
		LkaHitFlag = false;
		LkaCoolTime = 0;
	}
}

void Damage::Render() {
	DrawFormatString(0, 200, GetColor(255, 255, 255), "San %d", SanHP);
	DrawFormatString(0, 220, GetColor(255, 255, 255), "Lka %d", LkaHP);
	DrawFormatString(0, 240, GetColor(255, 255, 255), "Distance %f", Distance);

	auto vec1 = VGet(Lka->vPos.x, Lka->vPos.y + 50, Lka->vPos.z);
	auto vec2 = VGet(San->vPos.x, San->vPos.y + 50, San->vPos.z);


	DrawFormatString(0, 260, GetColor(255, 255, 255), "%f, %f, %f", vec1.x, vec1.y, vec1.z);
	DrawFormatString(0, 280, GetColor(255, 255, 255), "%f, %f, %f", vec2.x, vec2.y, vec2.z);

	DrawFormatString(0, 300, GetColor(255, 255, 255), "SanCoolTime %f", SanCoolTime);
	DrawFormatString(0, 320, GetColor(255, 255, 255), "LkaCoolTime %f", LkaCoolTime);

	DrawFormatString(0, 340, GetColor(255, 255, 255), "SanHitFlag %d", SanHitFlag);
	DrawFormatString(0, 360, GetColor(255, 255, 255), "LkaHitFlag %d", LkaHitFlag);
	
}