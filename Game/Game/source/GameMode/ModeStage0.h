#pragma once

#include "appframe.h"
#include "Player/Player.h"
#include "Player/SANclass.h"
#include "Player/LKAclass.h"
#include "Damage.h"
#include "timer.h"
#include "gimmick.h"
#include "Slime.h"
#include "Player/sancircle.h"
#include "Player/lkacircle.h"
#include "Player/sanheal.h"
#include "Player/lkacircle.h"
#include "IronDoor.h"
#include "Electrode.h"
#include "Elevator.h"
#include "Tube.h"
#include "timer.h"
#include "BossSwamp.h"
#include "ModeGameOver.h"

#include <string>
#include <memory>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>

// �v�Z�p�}�N��
#define	PI	(3.1415926535897932386f)
#define	DEG2RAD(x)			( ((x) / 180.0f ) * PI )
#define	RAD2DEG(x)			( ((x) * 180.0f ) / PI )





// ���[�h
class ModeStage0 : public ModeBase
{
	typedef ModeBase base;
public:
	ModeStage0();
	virtual bool Initialize();
	virtual bool Terminate();
	virtual bool Process();
	virtual bool Render();
	void Respawn();

	// �J����
	Camera _cam;


	// 3D���f���`��p
	int _handle;
	int _attach_index;
	float _total_time;
	float _play_time;
	VECTOR _vPos;	// �ʒu
	VECTOR _vDir;	// ����
	float _colSubY;	// �R���W�������莞��Y�␳(���ʒu�j

	// �}�b�v�p
	int _handleMap;
	int _handleSkySphere;
	int frameMapCollisionfloor;
	int frameMapCollisionwall;
	int frameMapCollisiongoalSAN;
	int frameMapCollisiongoalLKA;

	VECTOR respawnstartSan;
	VECTOR respawnstartLka;
	bool respawn1st = false;
	VECTOR respawn1stPosSan;
	VECTOR respawn1stPosLka;
	bool respawn2nd = false;
	VECTOR respawn2ndPosSan;
	VECTOR respawn2ndPosLka;

	int LightHandle;
	int MaskHandle;

	// �f�o�b�O�p
	bool	_bViewCollision;

	//�W�����v�����p
	float throughtime;
	float height;


	//UV�X�N���[��
	float _UVScroll_U, _UVScroll_V;

	//�V���h�E�}�b�v�p

	int ShadowMapHandle;

	VECTOR ShadowMapUpVec;
	VECTOR ShadowMapDownVec;

	//VOICE�p
	bool modeStart = false;
	int VOICEstartSANLKA[6] = { LoadSoundMem("res/06_Sound/02_Voice/01_In_Game/06_Start/Lka_GO_Voice_01.wav"),
															LoadSoundMem("res/06_Sound/02_Voice/01_In_Game/06_Start/Lka_GO_Voice_02.wav"),
															LoadSoundMem("res/06_Sound/02_Voice/01_In_Game/06_Start/Lka_GO_Voice_03.wav"),
															LoadSoundMem("res/06_Sound/02_Voice/01_In_Game/06_Start/San_GO_Voice_01.wav"),
															LoadSoundMem("res/06_Sound/02_Voice/01_In_Game/06_Start/San_GO_Voice_02.wav"),
															LoadSoundMem("res/06_Sound/02_Voice/01_In_Game/06_Start/San_GO_Voice_03.wav") };

public:
	Player player;
	SAN san;
	LKA lka;
	SanBomb sanbomb;
	LkaBomb lkabomb;
	Sancircle sancircle;
	Lkacircle lkacircle;
	std::vector<std::unique_ptr<IronDoor>> irondoors;
	Damage damage;
	Timer timer;
	ModeGameOver gameover;
};