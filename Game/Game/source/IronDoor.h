#pragma once
class IronDoor
{
public:
	IronDoor();
	~IronDoor();
	void Initialize();
	void Process(SanBomb& sanbomb);
	void Render();

	// 3Dモデル描画用
	int handle;
	VECTOR vPos;	// 位置
	int handleCol;
	int handleIronDoor;
	int handleIronMeltDoor;
	bool melt = false;
};
