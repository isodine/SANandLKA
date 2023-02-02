
#include "ApplicationBase.h"

ApplicationBase	*ApplicationBase::_lpInstance = NULL;


ApplicationBase::ApplicationBase() {
	_lpInstance = this;
}

ApplicationBase::~ApplicationBase() {
}

bool ApplicationBase::Initialize(HINSTANCE hInstance) {

	// DXライブラリの初期化
	if(AppWindowed()){
		ChangeWindowMode(true);							// ウィンドウモードに指定する
	}
	SetGraphMode(DispSizeW(), DispSizeH(), 32);

	if ((DxLib_Init() == -1)|| CheckHitKey(KEY_INPUT_ESCAPE))
	{	// エラーが起きたら直ちに終了
		return false;
	}
	SetDrawScreen(DX_SCREEN_BACK);		// 描画先画面を裏画面にセット

	// 乱数初期化
	srand((unsigned int)time(NULL));

	// モードサーバの初期化
	_serverMode = new ModeServer();

	return true;
}

bool ApplicationBase::Terminate() {
	// DXライブラリ開放
	DxLib_End();

	return true;
}


bool ApplicationBase::Input() {
	// キーの入力、トリガ入力を得る
	int keyold1P = _gKey1P;
	_gKey1P = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	_gTrg1P = (_gKey1P ^ keyold1P) & _gKey1P;	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）
	int keyold2P = _gKey2P;
	_gKey2P = GetJoypadInputState(DX_INPUT_PAD2);
	_gTrg2P = (_gKey2P ^ keyold2P) & _gKey2P;	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）

	return true;
}

bool ApplicationBase::Process() {
	_serverMode->ProcessInit();
	_serverMode->Process();
	_serverMode->ProcessFinish();
	return true;
}

bool ApplicationBase::Render() {
	_serverMode->RenderInit();
	_serverMode->Render();
	_serverMode->RenderFinish();
	return true;
}
