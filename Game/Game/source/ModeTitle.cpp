#include "AppFrame.h"
//#include "ApplicationMain.h"
#include "ModeTitle.h"
//#include "ModeGame.h"
#include "ModeStage0.h"

ModeTitle::ModeTitle() {
	titleHandle = LoadGraph("res/title.png");
}

bool ModeTitle::Initialize() {
	if (!base::Initialize()) { return false; }
	
}

bool ModeTitle::Terminate() {
	base::Terminate();

	return true;
}

bool ModeTitle::Process() {
	base::Process();
	int Trg;
	int keyold = Key;
	Key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	Trg = (Key ^ keyold) & Key;	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）

	int checkKey = PAD_INPUT_1 | PAD_INPUT_2 | PAD_INPUT_3 | PAD_INPUT_4 | PAD_INPUT_5 | PAD_INPUT_6 | PAD_INPUT_7 | PAD_INPUT_8 | PAD_INPUT_9 | PAD_INPUT_10;


	if (Trg & checkKey) {
		ModeServer::GetInstance()->Del(this);
		ModeServer::GetInstance()->Add(new ModeGame(), 1, "game");
	}

	return true;
}

bool ModeTitle::Render() {
	//DrawString(320, 240, "タイトル画面", GetColor(255, 255, 255));

	DrawGraph(0, 0, titleHandle, true);
	return true;
}