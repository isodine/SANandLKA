#include "Player.h"
#include "ModeGame.h"

int _handleMap;
int _handleSkySphere;
int _frameMapCollision;

Player::Player(bool issan)
{
	isSan = issan;
}

Player::~Player()
{
}

void Player::Initialize()
{
}

void Player::DamageProcess()
{
	if (isSan)
	{
		// sanのダメージ判定
	}
	else
	{
		// lkaのダメージ判定
	}
}

void Player::Update(){}

void Player::Render(){}

void Player::charJump() {
	height += 10.0f - throughtime;
	throughtime += 0.5f;
}