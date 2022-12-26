#include"LKAclass.h"
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"
#include <string>
#include <memory>
#include <vector>

LKA::LKA()
{
}

LKA::~LKA()
{
}

void LKA::Initialize()
{
	// モデルデータのロード（テクスチャも読み込まれる）
	Mhandle = MV1LoadModel("res/mecha-shiteyanyo/Model/mecha.mv1");
	Mattach_index = -1;		// アニメーションアタッチはされていない
	// ステータスを「無し」に設定
	_status = STATUS::NONE;
	// 再生時間の初期化
	Mtotal_time = 0.f;
	Mplay_time = 0.0f;
	// 位置,向きの初期化
	vPos = VGet(0, 0, 0);
	vDir = VGet(0, 0, -1);		// キャラモデルはデフォルトで-Z方向を向いている

	// 腰位置の設定
	_colSubY = 60.f;

}

void LKA::Input()
{
	int keyold1P = Key1P;
	Key1P = GetJoypadInputState(DX_INPUT_KEY_PAD1);
	Trg1P = (Key1P ^ keyold1P) & Key1P;	// キーのトリガ情報生成（押した瞬間しか反応しないキー情報）
}

void LKA::Update(Camera& cam)
{
	int keyold1P = Key1P;
	Input();
	//int key = ApplicationMain::GetInstance()->GetKey1P();
	//int trg = ApplicationMain::GetInstance()->GetTrg1P();
	int key = Key1P;
	int trg = Trg1P;
	//std::unique_ptr<Camera> cam = std::make_unique<Camera>();

	// 処理前のステータスを保存しておく
	STATUS oldStatus = _status;
	// カメラの向いている角度を取得
	//float sx = _cam._vPos.x - _cam._vTarget.x;
	//float sz = _cam._vPos.z - _cam._vTarget.z;
	//float camrad = atan2(sz, sx);

	// 移動方向を決める
	VECTOR v = { 0,0,0 };
	float mvSpeed = 6.f;
	if (key & PAD_INPUT_RIGHT) { v.x = 1; }
	if (key & PAD_INPUT_LEFT) { v.x = -1; }
	if (key & PAD_INPUT_DOWN) { v.z = -1; }
	if (key & PAD_INPUT_UP) { v.z = 1; }

	//if (key & PAD_INPUT_DOWN) { v.x = 1; }
	//if (key & PAD_INPUT_UP) { v.x = -1; }
	//if (key & PAD_INPUT_LEFT) { v.z = -1; }
	//if (key & PAD_INPUT_RIGHT) { v.z = 1; }
	if (key & PAD_INPUT_10 && !(_status == STATUS::JUMP)) { _status = STATUS::JUMP; }

	if (_status == STATUS::JUMP) { charJump(); }
	// vをrad分回転させる
	//float length = 0.f;
	//if (VSize(v) > 0.f) { length = mvSpeed; }
	//float rad = atan2(v.z, v.x);
	//v.x = cos(rad + camrad) * length;
	//v.z = sin(rad + camrad) * length;

	// 移動前の位置を保存
	VECTOR oldvPos = vPos;

	// vの分移動
	vPos = VAdd(vPos, v);

	// 移動した先でコリジョン判定
	MV1_COLL_RESULT_POLY hitPoly;
	// 主人公の腰位置から下方向への直線
	hitPoly = MV1CollCheck_Line(_handleMap, _frameMapCollision,
		VAdd(vPos, VGet(0, _colSubY, 0)), VAdd(vPos, VGet(0, -99999.f, 0)));
	if (hitPoly.HitFlag) {
		// 当たった
		if (vPos.y < hitPoly.HitPosition.y) {
			throughtime = 0.0f;
			height = 0.0f;
			vPos.y = 0.f;
			_status = STATUS::WAIT;
		}
		// 当たったY位置をキャラ座標にする
		vPos.y = hitPoly.HitPosition.y + height;

		// カメラも移動する
		cam._vPos = VAdd(cam._vPos, v);
		cam._vTarget = VAdd(cam._vTarget, v);
	}
	else {
		// 当たらなかった。元の座標に戻す
		vPos = oldvPos;
	}

	// 移動量をそのままキャラの向きにする
	if (VSize(v) > 0.f) {		// 移動していない時は無視するため
		vDir = v;
		if (!(_status == STATUS::JUMP)) {
			_status = STATUS::WALK;
		}
	}
	else if (throughtime > 0.0f) {}
	else {
		_status = STATUS::WAIT;
	}


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
			Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "Anim003"), -1, FALSE);
			break;
		case STATUS::WALK:
			Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "Anim004"), -1, FALSE);
			break;
		case STATUS::JUMP:
			Mattach_index = MV1AttachAnim(Mhandle, MV1GetAnimIndex(Mhandle, "Anim002"), -1, FALSE);
			break;
		}
		// アタッチしたアニメーションの総再生時間を取得する
		Mtotal_time = MV1GetAttachAnimTotalTime(Mhandle, Mattach_index);
		// 再生時間を初期化
		Mplay_time = 0.0f;
	}

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (Mplay_time >= Mtotal_time) {
		Mplay_time = 0.0f;
	}
}

void LKA::Render() {
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
		MV1SetScale(Mhandle, VGet(3.0f, 3.0f, 3.0f));
		MV1DrawModel(Mhandle);

		// コリジョン判定用ラインの描画
		DrawLine3D(VAdd(vPos, VGet(0, _colSubY, 0)), VAdd(vPos, VGet(0, -99999.f, 0)), GetColor(255, 0, 0));

	}
}