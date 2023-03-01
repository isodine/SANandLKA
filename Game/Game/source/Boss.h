#pragma once

#include "appframe.h"
class Boss {
public:
	void Initialize();
	void Terminate();
	void Process();
	void Render();

	void Rush(VECTOR sanPos, VECTOR lkaPos, VECTOR sanDir, VECTOR lkaDir);
	void Capture();
	void Rotation();
	void Walk();
	void Crush();
	void Down();
	void Idle();

	int Target;
	VECTOR BossPos;
	VECTOR BossSetDir;
	VECTOR BossDir;
	VECTOR RightHandPos;
	VECTOR LeftHandPos;
	VECTOR HandPos;
	VECTOR AddPos;

	MATRIX rotationMatrix;
	VECTOR forward;

	enum class BOSSTYPE {
		NONE,
		RUSH,
		CAPTURE,
		ROTATION,
		WALK,
		CRUSH,
		DOWN,
		IDLE,
	};

	BOSSTYPE type;
	
	int BossHandle;
	int AttachAnim1;
	int AttachAnim2;
	int AttachAnim3;
	float TotalTime1;
	float TotalTime2;
	float TotalTime3;
	float PlayTime;
	int randomNum;
	int walkRand;
	int walkTime0;
	int walkTime1;
	int walkTime2;
	int WalkTime;
	int walkTimeCount;

	float StopDir;

	bool rotateFlag;//TRUEなら回転する
	bool walkFlag;//TRUEなら歩く
	bool rushFlag;//TRUEなら攻撃準備から攻撃に移る
	bool targetFlag;//TRUEなら攻撃のターゲットを決める
	bool target;//TRUEならサンに攻撃
	bool idleflag;//

	SAN* san;
	LKA* lka;
	modelInf model;
	modelManager* manager;
};