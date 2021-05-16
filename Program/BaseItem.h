#pragma once

#include "Func.h"
#include "BaseBullet.h"


class CBaseItem;
class CItemManager;

//---------------------------------------------------------------------------------
//	CItemManager
//---------------------------------------------------------------------------------
//�e�̊Ǘ��}�l�[�W���Ɠ����@���O�I�Ɍp����������

class CItemManager : public CBulletManager{
public:

	//�R���X�g���N�^
	//���ˍő吔��ݒ肷��
	//�f�t�H���g1024
	CItemManager(int m_num = 1024);
	virtual ~CItemManager();

	//���C���`��
	virtual void Draw();
};


//---------------------------------------------------------------------------------
//	CBaseItem
//---------------------------------------------------------------------------------
//�G�t�F�N�g���e�Ǝ��Ă���
class CBaseItem : public CBaseBullet{

public:
	// �҂���
	int m_waitTime;
	int m_waitCount;
	void SetWaitTime(int waitTime);

private:
	//�폜����
	bool m_removeNow;

	//�P���@�摜�f�[�^�ւ̃|�C���^�@�����摜�f�[�^����ۑ����Ă���
	static CBulletImage* m_shineImage;
public:

	//��͓K���ɉ���������p��
	void (*funcP)(CBaseItem* eff);
	void SetFuncP(void (*FuncP)(CBaseItem* eff));

	//�R���X�g���N�^	�e�쐬
	CBaseItem(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//CBaseItem(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName);

	//�f�X�g���N�^
	virtual ~CBaseItem();
	virtual void Set();
	//virtual void SetImage(int image);

	//�Ƃ肠�����S���p�����Ƃ�
	//���C���s��
	virtual void Action();
	//���C���`��
	virtual void Draw();

	virtual void RectOut();

	double m_gravity; // �d�́@�����鑬�x

	//�ړ�����	�e�̂𗬗p
	//virtual void Move();
	//�폜
	virtual void Remove();

	// �v���C���[�͈̔͂ɓ���܂ł̎���
	int m_survivalTime;
	int m_itemRank; // 1�i���܂łɎ��Ԃ��|�������j�`3�i���Ȃ葁��������j

	double m_size;
	double m_plusSize;
	double m_maxSize;
	void SetSize(double size, double plusSize, double maxSize);

	int GetSurvivalTimeRank();
	void SetRemove();
	void GetItemAddScore();
	void SetRecovery();
	void SetPreRecovery(); // 45fr�o�߂���܂łɃA�C�e���̎擾�͈͂ɂ͂���ƁA45fr��Ɏ�������ɂ���
	bool m_recoveryFlag; // �A�C�e�������ԃt���O
	bool m_preRecoveryFlag; // �A�C�e�����O�����ԃt���O
	bool GetRecoveryFlag();

	// �o�Ă���A�C�e������莞�Ԃ͎��Ȃ��悤�ɂ��邽�߂̔���
	bool IsTakeTime();


};

//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*

���ʂ̒e���N���X�Ɠ����g�������ł���


	CPos pp(CFunc::RandF(0,WindowX), CFunc::RandF(0,400));
	double ang = CFunc::RandF(0,360);
	CBaseItem* eff = new CBaseItem(EDirType::Abs, pp, 2.0, ang, 0,0,0, 206);
	eff->SetSize(3.0, +0.05);
	eff->SetBlend(192, +3.0);

	eff->SetAnimeEndDelFlg(false);	//�A�j���[�V�����I����폜���邩
	eff->SetRemoveCount(60);	//60fr�ō폜

	effectManager1->Add(eff);


*/

