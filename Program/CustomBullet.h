#pragma once

#include "Func.h"
#include "BaseBullet.h"

//�ω��e�Œl�������p���ꍇ�̒l
const double NOTRANS = 99999;

//�ėp�e��
//�ʏ�̋@�\�ɉ���
//	�ω��e�A�ǉ��e�𓋍�
//	�e�̎��ԉ�m_wait�A���̕`����ǉ�
class CCustomBullet : public CBaseBullet{
public:

	//�l�ݒ�@��ɃR���X�g���N�^����Ă�
	virtual void Set();
	//�R���X�g���N�^�@���������镨�������Ă�
	CCustomBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG);
	CCustomBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME);

	//�f�X�g���N�^
	virtual~CCustomBullet();

	//�������g���Ǘ����Ă���}�l�[�W���̃|�C���^(�֐��|�C���^�ŎQ�Ƃ��邽��
	static CBulletManager *m_manager;
	static void SetBulletManagerPointer(CBulletManager *Manager);

	//���C���s��
	virtual void Action();
	virtual void Draw();

	//---------------
	//�ω��e�̕ϐ��A�@�\
	//---------------
	//�ω����鎞��
	int m_transFrame;
	//�ω���̒e
	CCustomBullet* transBullet;
	//�ω���̊p�x	�R���X�g���N�^��m_angle���ς�邽��(PLAYER�̏ꍇ)�v�Z�ł��Ȃ��̂ŁA���̕ϐ���苁�߂�
	double transAngle;
	void SetTransAngle(double Angle);	//PLAYER����NOTRANS�łȂ�(���ω��e�ł���)�ꍇ�ɃR�[������
	void SetTransBullet(CCustomBullet* Bullet, int m_transFrame);
	void Trans();
	//���W�l�͕K���ω��O�������p��
	//�e���̑Ώۂ�PLAYER�������ꍇ�A�ω��^�C�~���O�ōĕ]������
	//	�ĕ]���������Ȃ��ꍇ�́AABS�ɂ��Ď��͂Ōv�Z�����l��n������
	//�l��ω����������Ȃ��ꍇNOTRANS���w�肷��
	//�摜��ω����������Ȃ��ꍇ�͓���IDX�����Ă����Ă��i�A�j���Ƃ������p��

	//---------------
	//�ǉ��e�̕ϐ��A�@�\
	//---------------
	//�e��f������
	int m_addFrame;
	void (*addFuncP)(CCustomBullet* m_bullet);

	void SetAddBullet(void (*funcP)(CCustomBullet* m_bullet), int m_addFrame);
	//�ǉ��̒e
	void Add();


	//---------------
	//���
	//---------------
	//���ԉ����鎞��
	int m_standByTime;
	//���̉�����܂ł̃J�E���g	0�`
	int m_standByCount;
	void SetStandByTime(int Time);
};



//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*

����{�I�ȑ���
	CbaseBullet�̏�������
	init�Œe�Ǘ��N���X�̃|�C���^��ݒ肵�Ă�������
	CCustomBullet::SetBulletManagerPointer(bulletManager);


	���L�̂Q�̐ݒ肪�K�v
	//
	//	CBaseBullet�N���X�̐ݒ肵�Ȃ���΂Ȃ�Ȃ�����
	//
	CBaseBullet::SetRect(CRect(0,0,640,640));
	CBaseBullet::setImageData(m_imageManager);


���ǉ��e
	//�K���Ȋ֐�
	void addFuncA(CCustomBullet* m_bullet){
		for(int i=0;i<5;i++){
			m_bullet->m_manager->Add(new CBaseBullet(ABS, m_bullet->pos, 0, 90 + -20+i*10, 0,0.01,10, "bullet00"));
		}
	}
	void addFuncB(CCustomBullet* m_bullet){
		for(int i=0;i<10;i++){
			m_bullet->m_manager->Add(new CBaseBullet(PLAYER, m_bullet->pos, 2.5, i*(360.0/10), 0,0,0, "bullet00"));
		}
	}


	CCustomBullet *ppp = new CCustomBullet(ABS, CPos(320,240), 0.0, CFunc::RandF(180,360) ,0,0.02,10, 0);
	//20fr����addFuncA�����s����
	ppp->SetAddBullet(addFuncA, 20);
	//�e��ǉ�����
	bulletManager->Add(ppp);

���ω��e
	CCustomBullet *m_bullet = new CCustomBullet(ABS, CPos(320,240), 4, aaa, 0,0,0, "bullet00");
	CCustomBullet *Trans = new CCustomBullet(PLAYER, CPos(0,0), 0, 0, 0,0.25,10, "bullet00");
	CCustomBullet *trans2 = new CCustomBullet(ABS, CPos(0,0), NOTRANS, CFunc::RandF(0,360), 0,0,0, 1);

	//�l��ω����������Ȃ��Ƃ���NOTRANS��ݒ肷��
	//�p�x��
	//ABS	NOTRANS		�ω����Ȃ�
	//ABS	�C�ӂ̊p�x	
	//PLAYER	NOTRANS		�ω����Ȃ�
	//PLAYER	�C�ӂ̊p�x	SetTransBullet���Ȃ���΂Ȃ�Ȃ�

	//���W
	//�s�v

	//m_bullet��30fr���Trans�ɂȂ�
	m_bullet->SetTransBullet(Trans, 60);

	//PLAYER�_���̎������A�ω���̊p�x���Đݒ肵�Ă����Ȃ��Ƃ����Ȃ�
	Trans->SetTransAngle(0);

	//Trans��20fr���trans2�ɂȂ�
	Trans->SetTransBullet(trans2, 60);

	//��L��ݒ��m_bullet�𔭎�
	bulletManager->Add(m_bullet);



*/