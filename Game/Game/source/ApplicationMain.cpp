
#include "AppFrame.h"
//#include "ApplicationMain.h"
#include "ModeTitle.h"
#include "ModeLogo.h"

// 実体
ApplicationMain				g_oApplicationMain;

bool ApplicationMain::Initialize(HINSTANCE hInstance) {
	if (!base::Initialize(hInstance)) { return false; }

	// モードの登録
	ModeServer::GetInstance()->Add(new ModeLogo(), 1, "logo");
	//ModeServer::GetInstance()->Add(new ModeStage0(), 1, "stage00");
	//ModeServer::GetInstance()->Add(new ModeGame(), 1, "stage01");
	//ModeServer::GetInstance()->Add(new ModeBoss(), 1, "boss");
	return true;
}

bool ApplicationMain::Terminate() {
	base::Terminate();
	return true;
}


bool ApplicationMain::Input() {
	base::Input();
	return true;
}

bool ApplicationMain::Process() {
	base::Process();
	return true;
}

bool ApplicationMain::Render() {
	base::Render();
	return true;
}

