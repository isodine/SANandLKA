#pragma once

extern bool timeup;
class Timer
{
public:
	Timer();
	~Timer();
	void Update();            //�X�V
	void Render();            //�`��

	int number[10];
	int count10 = 3;
	int count1 = 0;
	int secondcnt = 0;
};
