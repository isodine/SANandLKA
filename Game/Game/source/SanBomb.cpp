SanBomb::SanBomb() :PlayerBomb()
{

}

SanBomb::~SanBomb() 
{

}

void SanBomb::Initialize(SAN& san)
{
	_effectResourceHandle[0] = LoadEffekseerEffect("res/san_bomb_1.6_2/san_bomb_loop_01.efkefc");
	_effectResourceHandle[1] = LoadEffekseerEffect("res/san_bomb_explode/sam_bomb_explode_effect.efkefc");
	vPos = VGet(san.vPos.x, san.vPos.y + 150, san.vPos.z);
	
	mypH = San;
	situation = PlayerBomb::None;
	Isbombdead = false;
}

void SanBomb::Update(SAN& san)
{
	int key = san.Key1P;
	int trg = san.Trg1P;

	MV1_COLL_RESULT_POLY_DIM hitfloor;
	hitfloor = MV1CollCheck_Sphere(san.stageHandle, san.floorCol, vPos, 10.0f);

	if (san.attack == Pop)
	{
		situation = PlayerBomb::Pop;
		//bomblive = true;
	}

	if (san.attack == Keep)
	{
		situation = PlayerBomb::Keep;
	}

	if (san.attack == PlayerBomb::Throw)
	{
		situation = PlayerBomb::Throw;
	}
	if (0 < hitfloor.HitNum)
	{
		BombReset();
		StopEffekseer3DEffect(_playingEffectHandle[0]);
		vPos = VGet(vPos.x, vPos.y, vPos.z);
		situation = PlayerBomb::Dead;
	}
	switch (situation)
	{
	case PlayerBomb::None:
		break;
	case PlayerBomb::Pop:
		vPos = VGet(san.vPos.x, san.vPos.y + 150, san.vPos.z);
		if(bomblive == false)
		{
			_playingEffectHandle[0] = PlayEffekseer3DEffect(_effectResourceHandle[0]);
			// 再生中のエフェクトを移動する。
			SetPosPlayingEffekseer3DEffect(_playingEffectHandle[0], vPos.x, vPos.y, vPos.z);
	/*		SetScalePlayingEffekseer3DEffect(_playingEffectHandle[0], 10.0f, 10.0f, 10.0f);*/
		}
		bomblive = true;
		if (sphereSize <= sphereMax)
		{
			sphereSize += 2;
		}
		else
		{
			situation = PlayerBomb::Keep;
		}
		break;
	case PlayerBomb::Keep:
		vPos = VGet(san.vPos.x, san.vPos.y + 150, san.vPos.z);
		break;
	case PlayerBomb::Throw:
		Throw(san);
		break;
	case PlayerBomb::Dead:
		if (!Isbombdead)
		{
			_playingEffectHandle[1] = PlayEffekseer3DEffect(_effectResourceHandle[1]);
			// 再生中のエフェクトを移動する。
			SetPosPlayingEffekseer3DEffect(_playingEffectHandle[1], vPos.x, vPos.y, vPos.z);
			SetScalePlayingEffekseer3DEffect(_playingEffectHandle[1], 10.0f, 10.0f, 10.0f);
		}
		Isbombdead = true;
		IsPlaying = IsEffekseer3DEffectPlaying(_playingEffectHandle[1]);
		Bombdead();
		break;
	}
	SetPosPlayingEffekseer3DEffect(_playingEffectHandle[0], vPos.x, vPos.y, vPos.z);
}

void SanBomb::Render()
{
	//DrawSphere3D(vPos, sphereSize, 8, GetColor(255, 0, 0), GetColor(255, 255, 255), FALSE);
	DrawEffekseer3D();
}

void SanBomb::Throw(SAN& san)
{
	if (!firstdir)
	{
		vDir = VGet(0.f, 0.f, 0.f);
		vDir = VAdd(vDir, san.vDir);
		vDir.x = vDir.x ; vDir.y = vDir.y ; vDir.z = vDir.z ;
		firstdir = true;
	}
	vPos = VAdd(vPos, vDir);
	vPos.y -= count;
	vPos.y += decrement;
	count += 0.5f;
	if (vPos.y < 0) {
		bomblive = false;
		firstdir = false;
		sphereSize = 0.f;
		count = 0.f;
		StopEffekseer3DEffect(_playingEffectHandle[0]);
		situation = PlayerBomb::None;
	}
}

void SanBomb::Bombdead()
{
	DrawEffekseer3D();
	if (IsPlaying == -1)
	{
		situation = PlayerBomb::None;
		Isbombdead = false;
		vPos = VGet(san.vPos.x, san.vPos.y + 150, san.vPos.z);
	}
}

void PlayerBomb::BombReset()
{
	bomblive = false;
	firstdir = false;
	sphereSize = 0.f;
	situation = PlayerBomb::Dead;
	count = 0.f;
}