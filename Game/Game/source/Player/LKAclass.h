/**
 * @file LKAclass.h
 * @brief ���J���L�̓�������������v���O����
 * @author �铇����
 * @date December 2022
 *
 * @note HP�Q�[�W�����	@author �c���@�l ,	�m�b�N�o�b�N��	@author �y�����T
 */

#pragma once
#include"Player.h"
#include "Gimmick.h"
#include "Boss.h"

class Boss;
class SAN;

class LKA:public Player, public Gimmick
{
public:
	LKA();
	~LKA();
	//����������
	void Initialize() override;
	//�X�V����
	void Update(Damage& damage, std::vector<std::unique_ptr<IronDoor>>* irondoors);
	//�`�揈��
	void Render(Damage& damage);
	//����擾
	void Input() override;
	//�폜����
	void Terminate() override;
	//�m�b�N�o�b�N����
	void KnockBack() override;
	void SetSan(SAN* _san);
	bool lkaBackFlag;


private:
	int hpgaugehandle[6];
	int hphandle;
	float oldcount;

	SAN* san;
};