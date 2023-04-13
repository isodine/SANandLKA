#pragma once
class IronDoor
{
public:
	IronDoor();
	~IronDoor();
	void Initialize(bool type, VECTOR Pos);
	void Update(SanBomb& sanbomb, LkaBomb& lkabomb);
	void Initialize();
	void Update(SanBomb& sanbomb);
	void Terminate();
	void Render();
	void CollCheck(SAN& san, LKA& lka);

	// 3Dモデル描画用
	int handle;
	VECTOR vPos;	// 位置
	int handleCol;
	int handleIronDoor;
	int handleIronMeltDoor;
	bool isSan = false;
	bool melt = false;
};
