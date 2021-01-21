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
	CCustomBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//CCustomBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName);

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
	CCustomBullet* m_transBullet;
	//�ω���̊p�x	�R���X�g���N�^��m_angle���ς�邽��(EDirType::Player�̏ꍇ)�v�Z�ł��Ȃ��̂ŁA���̕ϐ���苁�߂�
	double m_transAngle;
	void SetTransAngle(double Angle);	//EDirType::Player����NOTRANS�łȂ�(���ω��e�ł���)�ꍇ�ɃR�[������
	void SetTransBullet(CCustomBullet* Bullet, int m_transFrame);
	void Trans();
	//���W�l�͕K���ω��O�������p��
	//�e���̑Ώۂ�EDirType::Player�������ꍇ�A�ω��^�C�~���O�ōĕ]������
	//	�ĕ]���������Ȃ��ꍇ�́AEDirType::Abs�ɂ��Ď��͂Ōv�Z�����l��n������
	//�l��ω����������Ȃ��ꍇNOTRANS���w�肷��
	//�摜��ω����������Ȃ��ꍇ�͓���IDX�����Ă����Ă��i�A�j���Ƃ������p��

	//---------------
	//�ǉ��e�̕ϐ��A�@�\
	//---------------
	//�e��f������
	int m_addFrame;
	void (*m_addFuncP)(CCustomBullet* m_bullet);

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
	Init�Œe�Ǘ��N���X�̃|�C���^��ݒ肵�Ă�������
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
			m_bullet->m_manager->Add(new CBaseBullet(EDirType::Abs, m_bullet->pos, 0, 90 + -20+i*10, 0,0.01,10, "bullet00"));
		}
	}
	void addFuncB(CCustomBullet* m_bullet){
		for(int i=0;i<10;i++){
			m_bullet->m_manager->Add(new CBaseBullet(EDirType::Player, m_bullet->pos, 2.5, i*(360.0/10), 0,0,0, "bullet00"));
		}
	}


	CCustomBullet *ppp = new CCustomBullet(EDirType::Abs, CPos(320,240), 0.0, CFunc::RandF(180,360) ,0,0.02,10, 0);
	//20fr����addFuncA�����s����
	ppp->SetAddBullet(addFuncA, 20);
	//�e��ǉ�����
	bulletManager->Add(ppp);

���ω��e
	CCustomBullet *m_bullet = new CCustomBullet(EDirType::Abs, CPos(320,240), 4, aaa, 0,0,0, "bullet00");
	CCustomBullet *Trans = new CCustomBullet(EDirType::Player, CPos(0,0), 0, 0, 0,0.25,10, "bullet00");
	CCustomBullet *trans2 = new CCustomBullet(EDirType::Abs, CPos(0,0), NOTRANS, CFunc::RandF(0,360), 0,0,0, 1);

	//�l��ω����������Ȃ��Ƃ���NOTRANS��ݒ肷��
	//�p�x��
	//EDirType::Abs	NOTRANS		�ω����Ȃ�
	//EDirType::Abs	�C�ӂ̊p�x	
	//EDirType::Player	NOTRANS		�ω����Ȃ�
	//EDirType::Player	�C�ӂ̊p�x	SetTransBullet���Ȃ���΂Ȃ�Ȃ�

	//���W
	//�s�v

	//m_bullet��30fr���Trans�ɂȂ�
	m_bullet->SetTransBullet(Trans, 60);

	//EDirType::Player�_���̎������A�ω���̊p�x���Đݒ肵�Ă����Ȃ��Ƃ����Ȃ�
	Trans->SetTransAngle(0);

	//Trans��20fr���trans2�ɂȂ�
	Trans->SetTransBullet(trans2, 60);

	//��L��ݒ��m_bullet�𔭎�
	bulletManager->Add(m_bullet);



*/