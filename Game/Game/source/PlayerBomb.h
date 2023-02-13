#pragma once
class PlayerBomb
{
public:
	PlayerBomb();
	~PlayerBomb();

	enum BombType
	{
		San,
		Lka
	};
	BombType mypH;

	// 3Dモデル描画用
	//int Mhandle;
	//int Mattach_index;
	//float Mtotal_time;
	//float Mplay_time;

	VECTOR vPos;	// 位置
	VECTOR vDir;	// 向き

	enum Situation
	{
		Pop,
		Keep,
		Throw
	};
	Situation situation;

};

PlayerBomb::PlayerBomb()
{
}

PlayerBomb::~PlayerBomb()
{
}