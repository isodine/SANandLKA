#include "AppFrame.h"
//#include "ApplicationMain.h"
#include "ModeBoss.h"
#include "ModeEnding.h"
#include "ModeGameOver.h"


bool ModeBoss::Initialize() {
	if (!base::Initialize()) { return false; }
	boss.Initialize();
	StageHandle = MV1LoadModel("res/07_stage_map/Boss_Stage/04_Stage_Boss.mv1");
	
	return true;
}

bool ModeBoss::Terminate() {
	base::Terminate();

	return true;
}

bool ModeBoss::Process() {
	base::Process();
	boss.Process();

	
	//san.Update();
	//lka.Update();

	/*if (CheckHitKey(KEY_INPUT_SPACE)) {
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(new ModeEnding(), 1, "ending");
	}

	if (CheckHitKey(KEY_INPUT_A)) {
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(new ModeGameOver(), 1, "GameOver");
	}*/

	return true;
}

bool ModeBoss::Render() {

	SetUseLighting(TRUE);
	SetGlobalAmbientLight(GetColorF(0.1f, 0.1f, 0.1f, 0.f));
	ChangeLightTypeDir(VGet(0, -1, 1));

	DrawString(0, 0, "�{�X", GetColor(255, 255, 255));
	MV1SetPosition(StageHandle, VGet(1000,0,1000));
	MV1DrawModel(StageHandle);

	SetCameraPositionAndTarget_UpVecY(VGet(1000, 1000, 100), VGet(1000, 0, 1000));
	SetCameraNearFar(10, 2000);
 
	boss.Render();
	//san.Render();
	//lka.Render();
	return true;
}