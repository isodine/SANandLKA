#pragma once
#include"Player.h"

class SAN:public Player
{
public:
	SAN();
	~SAN();
	void Initialize()override;
	void Update(Camera& cam);
	void Render()override;
	void Input();
private:

};