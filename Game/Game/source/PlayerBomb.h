#pragma once

class PlayerBomb
{
public:
	PlayerBomb();
	~PlayerBomb();
	void BombReset();

	enum BombType
	{
		San,
		Lka
	};
	BombType mypH;

	// 3D���f���`��p
	//int Mhandle;
	//int Mattach_index;
	//float Mtotal_time;
	//float Mplay_time;

	VECTOR vPos;	// �ʒu
	VECTOR vDir;	// ����

	float count = 0.f;
	float decrement = 10.f;

	float sphereSize = 0.f;
	float sphereMax = 50.f;

	bool bomblive = false;
	bool firstdir = false;

	enum Situation
	{
		None,
		Pop,
		Keep,
		Throw
	};
	Situation situation;

};