#pragma once
#include "AppFrame.h"
#include <string>

class SAN;
class LKA;
class SanBomb;
class LkaBomb;

class Slime {
public:
	void Initialize(float x, float y, float z, int pH);
	void Terminate();
	void Process(VECTOR SanPos, VECTOR LkaPos, int HandleMap, float speed, int mypH);
	void Render(int pH);
	//void Walk(float speed);
	void AcidWalk(float speed);
	void AlkaliWalk(float speed);
	void SanTargeting(VECTOR SanPos, float speed);
	void LkaTargeting( VECTOR LkaPos, float speed);
	//void SlimeJump(VECTOR SanPos, VECTOR LkaPos);
	void AcidJump(VECTOR SanPos);
	void AlkaliJump(VECTOR LkaPos);

	void SetSan(SAN* san);
	void SetLka(LKA* lka);
	void SetSanBomb(SanBomb* sanbomb);
	void SetLkaBomb(LkaBomb* lkabomb);

	float sanDistance;//スライムとサンの距離
	float lkaDistance;//スライムとルカの距離
	float acidcount;//モーションのフレーム数
	float alkalicount;//モーションのフレーム数
	bool slimeAttackFlag;//TRUEなら攻撃に入る
	bool sanHitFlag;//TRUEならスライムがサンに当たった
	bool lkaHitFlag;//TRUEならルカに当たった

	int AttachAnim;
	float TotalTime;
	float PlayTime;

	int slimeHandle;//スライムのモデル
	int acidHandle;//酸性スライムのテクスチャ
	int alkaliHandle;//アルカリ性スライムのテクスチャ

	MV1_COLL_RESULT_POLY_DIM hitPolyDimAcid;
	MV1_COLL_RESULT_POLY_DIM hitPolyDimAlkali;
	MV1_COLL_RESULT_POLY_DIM hitPolyDim1;
	MV1_COLL_RESULT_POLY_DIM hitPolyDim2;

	MATRIX _rotationMatrix;

	VECTOR slimeDir;//スライムの向き
	VECTOR oldPos;//スライムの前の場所

	VECTOR sanPos;//スライムから見たサンの向き
	VECTOR lkaPos;//スライムから見たルカの向き
	VECTOR slimePos;//スライムの場所

	VECTOR acidSlimePos;//酸性スライムの場所
	VECTOR alkaliSlimePos;//アルカリ性スライムの場所

	VECTOR forward;//スライムの進む向き
	VECTOR slimeTargetPos;//スライムが目指すポイント
	VECTOR acidPos[4];//酸性スライムが巡回するときに通過するポイント

	VECTOR alkaliPos[4];//アルカリ性スライムが巡回するときに通過するポイント

	int SearchPosMinX;//サンかルカがこの範囲に入ったらスライムが追いかける
	int SearchPosMaxX;
	int SearchPosMinZ;
	int SearchPosMaxZ;

	float slimeError;//スライムの座標の誤差
	int acidPassedCount;//酸性スライムが通過した数
	int alkaliPassedCount;//アルカリ性スライムが通過した数

	bool acidDieFlag;//TRUEなら酸性スライムが死ぬ
	bool alkaliDieFlag;//TRUEならアルカリ性スライムが死ぬ

	int mypH = 0;     //１＝酸性、２＝アルカリ性

	//std::vector<std::unique_ptr<Slime>> slime;
	SAN* _san;
	LKA* _lka;
	SanBomb* _sanbomb;
	LkaBomb* _lkabomb;
protected:
	enum class STATUS {
		NONE,
		ATTACK,
		HIT,
		WALK_KAI,
		_EOT_
	};
	STATUS _status;

	

};