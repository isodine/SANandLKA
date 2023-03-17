#pragma once
#include "AppFrame.h"
#include <string>

class SANclass;
class LKAclass;

class Slime {
public:
	void Initialize(float x, float y, float z, int pH);
	void Terminate();
	void Process(VECTOR SanPos, VECTOR LkaPos, int HandleMap, float speed);
	void Render(VECTOR Pos);
	void Walk(float speed);
	void Targeting(VECTOR SanPos, VECTOR LkaPos, float speed);
	void SlimeJump();

	float sanDistance;//スライムとサンの距離
	float lkaDistance;//スライムとルカの距離
	float slimecount;//モーションのフレーム数
	bool slimeSerch;//
	bool slimeAttackFlag;//TRUEなら攻撃に入る
	bool slimeHitFlag;//TRUEならスライムがサンに当たった


	int AttachAnim;
	float TotalTime;
	float PlayTime;

	int slimeHandle;//スライムのモデル
	int acidHandle;//酸性スライムのテクスチャ
	int alkaliHandle;//アルカリ性スライムのテクスチャ

	MATRIX _rotationMatrix;

	VECTOR slimeDir;//スライムの向き
	VECTOR oldPos;//スライムの前の場所

	VECTOR sanPos;//スライムから見たサンの向き
	VECTOR lkaPos;//スライムから見たルカの向き
	VECTOR slimePos;//スライムの場所

	VECTOR forward;//スライムの向き
	VECTOR slimeTargetPos;//スライムが目指すポイント
	VECTOR acidPos[4];
	VECTOR acidPos1;//酸性スライムが巡回するときに通過するポイント
	VECTOR acidPos2;
	VECTOR acidPos3;
	VECTOR acidPos4;

	VECTOR alkaliPos[4];
	VECTOR alkaliPos1;//アルカリ性スライムが巡回するときに通過するポイント
	VECTOR alkaliPos2;
	VECTOR alkaliPos3;
	VECTOR alkaliPos4;

	int SearchPosMinX;//サンかルカがこの範囲に入ったらスライムが追いかける
	int SearchPosMaxX;
	int SearchPosMinZ;
	int SearchPosMaxZ;

	int mypH = 0;     //１＝酸性、２＝アルカリ性

	//std::vector<std::unique_ptr<Slime>> slime;

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