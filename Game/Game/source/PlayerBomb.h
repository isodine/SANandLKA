#pragma once

class PlayerBomb
{
public:
	PlayerBomb();
	~PlayerBomb();
	//virtual void Initialize();
	//virtual void Update();
	//virtual void Render();
	void BombReset();
	void SetSan(SAN& San);

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

	SAN san;
};
