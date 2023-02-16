//#include "SANclass.h"
#include "AppFrame.h"
//#include "ApplicationMain.h"
//#include "ModeGame.h"

SAN::SAN()
	:Player()
{
	SetType(true);
}

SAN::~SAN()
{
}

void SAN::Initialize()
{
	Player::Initialize();
	sanfrask = LoadGraph("res/pH_gauge/pHgauge_SL_flask.png");
	sangauge = LoadGraph("res/pH_gauge/ゲージ（中）/酸/pHgauge_strongacid.png");
	sanicon = LoadGraph("res/pH_gauge/アイコン表情差分/サン/pHgauge_Sun_Emotions_Normal.png");
	sanframememori = LoadGraph("res/pH_gauge/フレーム/サン/pHgauge_Sun_background_memori.png");
	// モデルデータのロード（テクスチャも読み込まれる)
	Mhandle = MV1LoadModel("res/San_2023_0130/San_Fullmotion_2023_0203.mv1");

	// 位置,向きの初期化
	vPos = VGet(-60, 20, 0);

	// 腰位置の設定
	_colSubY = 45.f;

}

void SAN::Input()
{
	int keyold1P = Key1P;
	Key1P = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	Trg1P = (Key1P ^ keyold1P) & Key1P;	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）
}

void SAN::Update()
{
	Input();
	int key = Key1P;
	int trg = Trg1P;

	Player::Update();

	if (key & PAD_INPUT_5) {	//多分L1ボタン
		// 角度変更
		// Y軸回転
		float sx = _camera->_vPos.x - _camera->_vTarget.x;
		float sz = _camera->_vPos.z - _camera->_vTarget.z;
		float rad = atan2(sz, sx);
		float length = sqrt(sz * sz + sx * sx);
		if (key & PAD_INPUT_LEFT) { rad -= 0.05f; }
		if (key & PAD_INPUT_RIGHT) { rad += 0.05f; }
		_camera->_vPos.x = _camera->_vTarget.x + cos(rad) * length;
		_camera->_vPos.z = _camera->_vTarget.z + sin(rad) * length;

		// Y位置
		if (key & PAD_INPUT_DOWN) { _camera->_vPos.y -= 5.f; }
		if (key & PAD_INPUT_UP) { _camera->_vPos.y += 5.f; }
	}
	else {

		// 処理前のステータスを保存しておく
		STATUS oldStatus = _status;
		// カメラの向いている角度を取得
		float sx = _camera->_vPos.x - _camera->_vTarget.x;
		float sz = _camera->_vPos.z - _camera->_vTarget.z;
		float camrad = atan2(sz, sx);

		// 移動方向を決める
		VECTOR v = { 0,0,0 };
		float mvSpeed = 6.f;
		if (key & PAD_INPUT_DOWN) { v.x = 1; }
		if (key & PAD_INPUT_UP) { v.x = -1; }
		if (key & PAD_INPUT_LEFT) { v.z = -1; }
		if (key & PAD_INPUT_RIGHT) { v.z = 1; }
		if (key & PAD_INPUT_1 && !(_status == STATUS::JUMP)) { _status = STATUS::JUMP; PlaySoundMem(SEjump, DX_PLAYTYPE_BACK, true); }
		//if (key & PAD_INPUT_2 && !(_status == STATUS::CHARGE)) { _status = STATUS::CHARGE; }
		//if (key & PAD_INPUT_3 && !(_status == STATUS::ATTACK)) { _status = STATUS::ATTACK; }
		//if (key & PAD_INPUT_4 && !(_status == STATUS::DAMAGE)) { _status = STATUS::DAMAGE; }
		if (key & PAD_INPUT_10) { _status = STATUS::DOWN; }

		if (_bomb->situation == None) { attack = Attack::None; }
		if (trg & PAD_INPUT_6 && (attack == Attack::None)) {
			attack = Attack::Pop;
		}
		if (_bomb->situation == Keep) { attack = Attack::Keep; }
		if (trg & PAD_INPUT_6 && (attack == Attack::Keep)) { attack = Attack::Throw; }
		if (_status == STATUS::JUMP) { Jump(); }
		// vをrad分回転させる
		float length = 0.f;
		if (VSize(v) > 0.f) { length = mvSpeed; }
		float rad = atan2(v.z, v.x);
		v.x = cos(rad + camrad) * length;
		v.z = sin(rad + camrad) * length;

		// 移動前の位置を保存
		VECTOR oldvPos = vPos;



		// 画面内にキャラクターが入っていないかどうかを描画する
		//TRUEは入ってない、FALSEは入ってる
		if (CheckCameraViewClip(vPos) == TRUE)
		{
			// 画面外に出た。元の座標に戻す
			vPos = oldvPos;
			v = { 0,0,0 };
		}


		// vの分移動
		vPos = VAdd(vPos, v);


		// 移動した先でコリジョン判定
		MV1_COLL_RESULT_POLY_DIM hitPolyDim;
		MV1_COLL_RESULT_POLY hitPolyfloor;
		MV1_COLL_RESULT_POLY hitPolywall;

		hitPolywall = MV1CollCheck_Line(_handleMap, frameMapCollisionwall,
			VAdd(vPos, VGet(0, _colSubY, -50)), VAdd(vPos, VGet(0, _colSubY, 500.f)));
		if (hitPolywall.HitFlag && (vPos.z + 30 >= hitPolywall.HitPosition.z))
		{
			float backwidth = hitPolywall.HitPosition.z - vPos.z + 30;
			float subX = vPos.x - oldvPos.x;
			float subZ = vPos.z - oldvPos.z;
			vPos.x = oldvPos.x;
			vPos.z = oldvPos.z;

			v = { 0,0,0 };
		}

		// 主人公の腰位置から下方向への直線
		hitPolyfloor = MV1CollCheck_Line(_handleMap, frameMapCollisionfloor,
			VAdd(vPos, VGet(0, _colSubY, 0)), VAdd(vPos, VGet(0, -99999.f, 0)));

		hitPolyDim = MV1CollCheck_Capsule(_handleMap, frameMapCollisionfloor,
			VGet(vPos.x, vPos.y + 30, vPos.z), VGet(vPos.x, vPos.y + 75, vPos.z), 30.0f);
		if (hitPolyDim.HitNum >= 1)
		{
			// 当たった
			if (vPos.y < hitPolyfloor.HitPosition.y )
			{
				Landing(hitPolyfloor.HitPosition.y);
			}
		}
		else {
			freeFall();
		}



		// 移動量をそのままキャラの向きにする
		if (VSize(v) > 0.f) {		// 移動していない時は無視するため
			vDir = v;
			if (!(_status == STATUS::JUMP)) {
				_status = STATUS::WALK;
			}
		}
		else if (throughtime > 0.0f) {}
		else
		{
			_status = STATUS::WAIT;
		}



		//sanB.Update(this);         //ボムの更新



		// デバッグ機能
		//if (trg & PAD_INPUT_2) {
		//	_bViewCollision = !_bViewCollision;
		//}
		//if (_bViewCollision) {
		//	MV1SetFrameVisible(_handleMap, _frameMapCollision, TRUE);
		//}
		//else {
		//	MV1SetFrameVisible(_handleMap, _frameMapCollision, FALSE);
		//}


		// ステータスが変わっていないか？
		if (oldStatus == _status) {
			// 再生時間を進める
			Mplay_time += 0.5f;
		}
		else {
			// アニメーションがアタッチされていたら、デタッチする
			if (Mattach_index != -1) {
				MV1DetachAnim(Mhandle, Mattach_index);
				Mattach_index = -1;
			}
			// ステータスに合わせてアニメーションのアタッチ
			switch (_status) {
			case STATUS::WAIT:
				Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "idle"), -1, FALSE);
				break;
			case STATUS::WALK:
				Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "walk"), -1, FALSE);
				break;
			case STATUS::JUMP:
				Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "jamp1"), -1, FALSE);
				break;
			case STATUS::DAMAGE:
				Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "damage"), -1, FALSE);
				break;
			case STATUS::CHARGE:
				Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "attack1"), -1, FALSE);
				break;
			case STATUS::ATTACK:
				Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "attack2"), -1, FALSE);
				break;
			case STATUS::DOWN:
				Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "down"), -1, FALSE);
				break;
			}

			// アタッチしたアニメーションの総再生時間を取得する
			Mtotal_time = MV1GetAttachAnimTotalTime(Mhandle, Mattach_index);
			// 再生時間を初期化
			Mplay_time = 0.0f;
			//if(!(oldStatus== STATUS::JUMP)&& _status== STATUS::JUMP)
			//{
			//	Mplay_time = 20.0f;
			//}
		}

		// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
		if (Mplay_time >= Mtotal_time)
		{
			Mplay_time = 0.0f;
		}
	}
}

void SAN::Render()//(SanBomb& sanB,Damage& damage)
{
	Player::Render();
	DrawGraph(0, 470, sanfrask, TRUE);
	DrawGraph(0, 470, sanicon, TRUE);
	HPgauge = 610 - int((610 / _damage->MaxSanHP) * _damage->SanHP);
	DrawRectGraph(0, 470, 0, 0, 220, 610 - HPgauge, sangauge, true, false);
	/*DrawGraph(0,470,sangauge,TRUE);*/
	DrawGraph(0, 470, sanframememori, TRUE);
	// 再生時間をセットする
	MV1SetAttachAnimTime(Mhandle, Mattach_index, Mplay_time);

	// モデルを描画する
	{
		// 位置
		MV1SetPosition(Mhandle, vPos);

		// 向きからY軸回転を算出
		VECTOR vRot = { 0,0,0 };
		vRot.y = atan2(vDir.x * -1, vDir.z * -1);		// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
		MV1SetRotationXYZ(Mhandle, vRot);

		// 描画
		//MV1SetScale(Mhandle, VGet(10.0f, 10.0f, 10.0f));
		//MV1SetFrameOpacityRate(Mhandle, 0, 0.0f);
		//MV1SetOpacityRate(Mhandle, 0.3f);
		//MV1SetMaterialDrawBlendMode(Mhandle, 0, DX_BLENDMODE_ALPHA);
		//MV1SetMaterialDrawBlendParam(Mhandle, 0, 100);
		MV1DrawModel(Mhandle);

		//ダメージ判定の描画
		DrawCapsule3D(VGet(vPos.x, vPos.y + 30, vPos.z), VGet(vPos.x, vPos.y + 75, vPos.z), 30.0f, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), FALSE);
		DrawSphere3D(VGet(vPos.x, vPos.y + 50, vPos.z), 55, 8, GetColor(0, 0, 255), GetColor(255, 255, 255), FALSE);

		// コリジョン判定用ラインの描画
		DrawLine3D(VAdd(vPos, VGet(0, _colSubY, -50)), VAdd(vPos, VGet(0, _colSubY, 500.f)), GetColor(255, 0, 0));

	}
	//sanB.Render();
	//DrawFormatString(0, 260, GetColor(255, 255, 255), "%f, %f, %f", vPos.x, vPos.y, vPos.z);
}
void SAN::Jump()
{
	if (throughtime == 0.f) { height = 10.f; }
	vPos.y += height;

}

void SAN::freeFall()
{
	vPos.y -= throughtime;
	throughtime += 0.5f;
}