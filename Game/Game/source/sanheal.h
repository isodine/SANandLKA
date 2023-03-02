#pragma once
class SAN;
class Sanheal
{
public:
	Sanheal();
	~Sanheal();

	void Init();
	void Update(SAN& san);
	void Render();
private:
	int		_effectResourceHandle;		// エフェクトファイルをロードするハンドル
	int		_playingEffectHandle;		// ロードしたエフェクトファイルから、エフェクトを生成したもの
	int oldcount;
	float PlayTime = 0.0f;
	int _isEffect = 0;
	int IsPlaying = -1;


};