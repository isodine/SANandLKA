
#include "AppFrame.h"
#include "ApplicationMain.h"
#include "ModeGame.h"


bool ModeGame::Initialize() {
	if (!base::Initialize()) { return false; }

	// モデルデータのロード（テクスチャも読み込まれる）
	//MV1SetLoadModelPhysicsWorldGravity(-122.5f)読み込むモデルの物理演算に適用する重力パラメータを設定する -122.5はデフォルト値
	MV1SetLoadModelPhysicsWorldGravity(-122.5f);
	//MV1SetLoadModelUsePhysicsMode(int PhysicsMode)読み込むモデルの物理演算モードを設定する
	//DX_LOADMODEL_PHYSICS_LOADCALC　　ファイル読み込み時に物理演算を行う
	//DX_LOADMODEL_PHYSICS_REALTIME　　　リアルタイム物理演算を行う
	//DX_LOADMODEL_PHYSICS_DISABLE　　　　物理演算を使用しない
	//どちらもモデルに物理演算用の情報があることが前提
	MV1SetLoadModelUsePhysicsMode(DX_LOADMODEL_PHYSICS_REALTIME);
	_handle = MV1LoadModel("res/promodel_multimotion_fujisaki.mv1");
	_handle1 = MV1DuplicateModel(_handle);
	_attach_index = -1;		// アニメーションアタッチはされていない
	// ステータスを「無し」に設定
	_status = STATUS::NONE;
	// 再生時間の初期化
	_total_time = 0.f;
	_play_time = 0.0f;
	// 位置,向きの初期化
	_vPos = VGet(0, 25, 0);
	_vDir = VGet(0, 0, -1);		// キャラモデルはデフォルトで-Z方向を向いている

	// マップ
	_handleMap = MV1LoadModel("res/plain/plain.mv1");
	_handleSkySphere = MV1LoadModel("res/SkySphere/skysphere.mv1");

	box1handle = MV1LoadModel("res/a_block_1.mv1");
	box2handle = MV1LoadModel("res/a_block_2.mv1");
	box21handle = MV1DuplicateModel(box2handle);
	box3handle = MV1LoadModel("res/a_block_3.mv1");
	box31handle = MV1DuplicateModel(box3handle);

	// コリジョン情報の生成
	_frameMapCollision = MV1SearchFrame(_handleMap, "Mesh");
	collision1 = MV1SearchFrame(box3handle, "a_block_X_harfwall");
	collision2 = MV1SearchFrame(box31handle, "a_block_X_harfwall");
	MV1SetupCollInfo(_handleMap, _frameMapCollision, 16, 16, 16);
	// コリジョンのフレームを描画しない設定
	MV1SetFrameVisible(_handleMap, _frameMapCollision, FALSE);
	// 腰位置の設定
	_colSubY = 60.f;


	// カメラの設定（わかりやすい位置に）
	_cam._vPos = VGet(0, 120.f, -300.f);
	_cam._vTarget = VGet(0, 60, 0);
	_cam._clipNear = 2.f;
	_cam._clipFar = 10000.f;

	// その他初期化
	_bViewCollision = TRUE;

	throughtime = 0.0f;
	height = 0.0f;

	return true;
}

bool ModeGame::Terminate() {
	base::Terminate();
	return true;
}

bool ModeGame::Process() {
	base::Process();
	int key = ApplicationMain::GetInstance()->GetKey();
	int trg = ApplicationMain::GetInstance()->GetTrg();

	// 処理前のステータスを保存しておく
	STATUS oldStatus = _status;


	if ((key & (PAD_INPUT_7 | PAD_INPUT_1)) == (PAD_INPUT_7 | PAD_INPUT_1)) {	// Q & Z
		// 距離、ターゲットの高さ変更
		float sx = _cam._vPos.x - _cam._vTarget.x;
		float sz = _cam._vPos.z - _cam._vTarget.z;
		float rad = atan2(sz, sx);
		float length = sqrt(sz * sz + sx * sx);
		if (key & PAD_INPUT_LEFT) { length -= 1.f; }
		if (key & PAD_INPUT_RIGHT) { length += 1.f; }
		_cam._vPos.x = _cam._vTarget.x + cos(rad) * length;
		_cam._vPos.z = _cam._vTarget.z + sin(rad) * length;

		// Y位置
		if (key & PAD_INPUT_DOWN) { _cam._vTarget.y -= 1.f; }
		if (key & PAD_INPUT_UP) { _cam._vTarget.y += 1.f; }
	}
	else if (key & PAD_INPUT_7) {	// Q
		// 角度変更
		// Y軸回転
		float sx = _cam._vPos.x - _cam._vTarget.x;
		float sz = _cam._vPos.z - _cam._vTarget.z;
		float rad = atan2(sz, sx);
		float length = sqrt(sz * sz + sx * sx);
		if (key & PAD_INPUT_LEFT) { rad -= 0.05f; }
		if (key & PAD_INPUT_RIGHT) { rad += 0.05f; }
		_cam._vPos.x = _cam._vTarget.x + cos(rad) * length;
		_cam._vPos.z = _cam._vTarget.z + sin(rad) * length;

		// Y位置
		if (key & PAD_INPUT_DOWN) { _cam._vPos.y -= 5.f; }
		if (key & PAD_INPUT_UP) { _cam._vPos.y += 5.f; }
	}
	else if (key & PAD_INPUT_1) {	// Z
		// カメラ位置（注目位置もXZスライド）
		float sx = _cam._vPos.x - _cam._vTarget.x;
		float sz = _cam._vPos.z - _cam._vTarget.z;
		float camrad = atan2(sz, sx);

		// 移動方向を決める
		VECTOR v = { 0,0,0 };
		float mvSpeed = 2.f;
		if (key & PAD_INPUT_DOWN) { v.x = 1; }
		if (key & PAD_INPUT_UP) { v.x = -1; }
		if (key & PAD_INPUT_LEFT) { v.z = -1; }
		if (key & PAD_INPUT_RIGHT) { v.z = 1; }
		

		// vをrad分回転させる
		float length = 0.f;
		if (VSize(v) > 0.f) { length = mvSpeed; }
		float rad = atan2(v.z, v.x);
		v.x = cos(rad + camrad) * length;
		v.z = sin(rad + camrad) * length;

		// vの分移動
		_cam._vPos = _cam._vPos;
		_cam._vTarget = _cam._vTarget;

	}
	else {
		// キャラ移動(カメラ設定に合わせて)

		// カメラの向いている角度を取得
		float sx = _cam._vPos.x - _cam._vTarget.x;
		float sz = _cam._vPos.z - _cam._vTarget.z;
		float camrad = atan2(sz, sx);

		// 移動方向を決める
		VECTOR v = { 0,0,0 };
		float mvSpeed = 6.f;
		if (key & PAD_INPUT_5) { v.x = 1; }
		if (key & PAD_INPUT_8) { v.x = -1; }
		if (key & PAD_INPUT_4) { v.z = -1; }
		if (key & PAD_INPUT_6) { v.z = 1; }
		//if (CheckHitKey(KEY_INPUT_SPACE)) { _vPos.y += height; }
		if (CheckHitKey(KEY_INPUT_UP) ) 
		{ _vPos.y += 1; }

		if (CheckHitKey(KEY_INPUT_DOWN))
		{ _vPos.y -= 1; }

		if (key & PAD_INPUT_10 && !(_status == STATUS::JUMP)) { _status = STATUS::JUMP; }

		if (_status == STATUS::JUMP) { charJump(); }
		// vをrad分回転させる
		float length = 0.f;
		if (VSize(v) > 0.f) { length = mvSpeed; }
		float rad = atan2(v.z, v.x);
		v.x = cos(rad + camrad) * length;
		v.z = sin(rad + camrad) * length;

		// 移動前の位置を保存
		VECTOR oldvPos = _vPos;

		// vの分移動
		_vPos = VAdd(_vPos, v);

		MV1SetupCollInfo(box3handle, -1, 8, 8, 8);
		MV1_COLL_RESULT_POLY_DIM hitPoly;
		// 主人公の腰位置から下方向への直線
		hitPoly = MV1CollCheck_Capsule(box3handle,-1, _vPos, VGet(_vPos.x, 75, _vPos.z),30.0f);
		if (hitPoly.HitNum >= 1) {
			throughtime = 0.0f;
			_status = STATUS::WAIT;
			_vPos = oldvPos;
		}
		//当たっている
		else {
			_cam._vPos = VAdd(_cam._vPos, v);
		  _cam._vTarget = VAdd(_cam._vTarget, v);
		}

		MV1SetupCollInfo(box1handle, -1, 8, 8, 8);
		//MV1_COLL_RESULT_POLY_DIM hitPoly;
		// 主人公の腰位置から下方向への直線
		hitPoly = MV1CollCheck_Capsule(box1handle, -1, _vPos, VGet(_vPos.x, 75, _vPos.z), 30.0f);
		if (hitPoly.HitNum >= 1) {
			throughtime = 0.0f;
			_status = STATUS::WAIT;
			
		}
		//当たっている
		else {
			//_cam._vPos = VAdd(_cam._vPos, v);
			//_cam._vTarget = VAdd(_cam._vTarget, v);
			_vPos = oldvPos;
		}

		// 移動量をそのままキャラの向きにする
		if (VSize(v) > 0.f) {		// 移動していない時は無視するため
			_vDir = v;
			if (!(_status == STATUS::JUMP)) {
				_status = STATUS::WALK;
			}
		}
		else if(throughtime > 0.0f){}
		else {
			_status = STATUS::WAIT;
		}

		// デバッグ機能
		if (trg & PAD_INPUT_2) {
			_bViewCollision = !_bViewCollision;
		}
		if (_bViewCollision) {
			MV1SetFrameVisible(_handleMap, _frameMapCollision, TRUE);
		}
		else {
			MV1SetFrameVisible(_handleMap, _frameMapCollision, FALSE);
		}
	}

	// ステータスが変わっていないか？
	if (oldStatus == _status) {
		// 再生時間を進める
		_play_time += 0.5f;
	}
	else {
		// アニメーションがアタッチされていたら、デタッチする
		if (_attach_index != -1) {
			MV1DetachAnim(_handle, _attach_index);
			_attach_index = -1;
		}
		// ステータスに合わせてアニメーションのアタッチ
		switch (_status) {
		case STATUS::WAIT:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle,"wait"), -1, FALSE);
			break;
		case STATUS::WALK:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "walk"), -1, FALSE);
			break;
		case STATUS::JUMP:
			_attach_index = MV1AttachAnim(_handle, MV1GetAnimIndex(_handle, "Anim002"), -1, FALSE);
			break;
		}
		// アタッチしたアニメーションの総再生時間を取得する
		_total_time = MV1GetAttachAnimTotalTime(_handle, _attach_index);
		// 再生時間を初期化
		_play_time = 0.0f;
	}

	// 再生時間がアニメーションの総再生時間に達したら再生時間を０に戻す
	if (_play_time >= _total_time) {
		_play_time = 0.0f;
	}


	return true;
}

bool ModeGame::Render() {
	base::Render();

	// 3D基本設定
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(FALSE);
	SetUseBackCulling(FALSE);

	// ライト設定
	SetUseLighting(TRUE);
#if 1	// 平行ライト
	SetGlobalAmbientLight(GetColorF(0.5f, 0.5f, 0.5f, 0.f));
	ChangeLightTypeDir(VGet(-1, -1, 0));
#endif
#if 0	// ポイントライト
	SetGlobalAmbientLight(GetColorF(0.f, 0.f, 0.f, 0.f));
	ChangeLightTypePoint(VAdd(_vPos,VGet(0,50.f,0)), 1000.f, 0.f, 0.005f, 0.f);
#endif

	// カメラ設定更新
	SetCameraPositionAndTarget_UpVecY(_cam._vPos, _cam._vTarget);
	SetCameraNearFar(_cam._clipNear, _cam._clipFar);

	// 0,0,0を中心に線を引く
	{
		float linelength = 1000.f;
		VECTOR v = { 0, 0, 0 };
		DrawLine3D(VAdd(v, VGet(-linelength, 0, 0)), VAdd(v, VGet(linelength, 0, 0)), GetColor(255, 0, 0));
		DrawLine3D(VAdd(v, VGet(0, -linelength, 0)), VAdd(v, VGet(0, linelength, 0)), GetColor(0, 255, 0));
		DrawLine3D(VAdd(v, VGet(0, 0, -linelength)), VAdd(v, VGet(0, 0, linelength)), GetColor(0, 0, 255));
	}

	//box1は緑の足場1000×1000　厚さ50
	MV1SetPosition(box1handle, VGet(0,0,0));
	//box2,box21は茶色の足場500×500　厚さ25
	//box1の高さに合わせるため、Ｙ座標に厚さの1/2を足している
	//X座標は緑の足場の中心からの距離の500と茶色の足場の中心からの距離250を足している
	MV1SetPosition(box2handle, VGet(750, 12.5, 250));
	MV1SetPosition(box21handle, VGet(750, 12.5, -250));
	//box3,box31は茶色の壁500×500　厚さ25
	//Ｘ座標は緑の足場の中心からの距離500に壁の厚さの半分の12.5を足してめり込まないようにしている
	MV1SetPosition(box3handle, VGet(-512.5, 0, 250));
	MV1SetPosition(box31handle, VGet(-512.5, 0, -250));
	//ここでＹ軸を中心に90度回転させている
	//回転させないと手前か奥の壁になる向き
	MV1SetRotationXYZ(box3handle, VGet(0, 90.0f * DX_PI_F / 180.0f,0));
	MV1SetRotationXYZ(box31handle, VGet(0, 90.0f * DX_PI_F / 180.0f, 0));
	MV1DrawModel(box1handle);
	MV1DrawModel(box2handle);
	MV1DrawModel(box21handle);
	MV1DrawModel(box3handle);
	MV1DrawModel(box31handle);
	MV1DrawModel(_handleSkySphere);

	// モデルを描画する
	{
		// 位置
		MV1SetPosition(_handle, _vPos);
		// 向きからY軸回転を算出
		VECTOR vRot = { 0,0,0 };
		vRot.y = atan2(_vDir.x * -1, _vDir.z * -1);		// モデルが標準でどちらを向いているかで式が変わる(これは-zを向いている場合)
		MV1SetRotationXYZ(_handle, vRot);
		//MV1SetRotationXYZ(_handle1, VGet(0,25,0));
		// 描画

		MV1DrawModel(_handle1);
		DrawCapsule3D(VGet(0, 55, 0), VGet(0, 75, 0), 30.0f, 8, GetColor(0, 255, 255), GetColor(255, 255, 255), true);
		DrawSphere3D(VGet(0, 65, 0), 40.0f, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), false);

		MV1DrawModel(_handle);
		DrawCapsule3D(VGet(_vPos.x, _vPos.y + 30, _vPos.z), VGet(_vPos.x, _vPos.y + 50, _vPos.z), 30.0f, 8, GetColor(0, 255, 255), GetColor(255, 255, 255), true);
		DrawSphere3D(VGet(_vPos.x, _vPos.y + 40, _vPos.z), 40.0f, 8, GetColor(0, 255, 0), GetColor(255, 255, 255), false);
		// コリジョン判定用ラインの描画
		if (_bViewCollision) {
			DrawLine3D(VAdd(_vPos, VGet(0, _colSubY, 0)), VAdd(_vPos, VGet(0, -99999.f, 0)), GetColor(255, 0, 0));
		}

	}
	return true;
}

void ModeGame::charJump() {
	_vPos.y += 10.0f - throughtime;
	throughtime += 0.5f;
}

