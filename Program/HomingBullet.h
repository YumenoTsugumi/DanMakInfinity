#pragma once

#include "Func.h"
#include "BaseBullet.h"

//�z�[�~���O���
class CLimitInfo{
public:
	int m_count;
	double m_limitAngle;	//�����p�x
	//m_count�t���[���܂�m_limitAngle�ł���

	//m_count�͏��������ɕ���ł��邱��
	//CVoidArray��CHomingLaser�ɓn������

	//ex)
	//m_count:10 8.0
	//m_count:30 3.0
	//m_count:MAXINT 0.5
};

class CHomingLaser : public CBaseBullet{
public:

	//�l�ݒ�@��ɃR���X�g���N�^����Ă�
	virtual void Set();
	//�R���X�g���N�^�@���������镨�������Ă�
	CHomingLaser(EDirType type, CPos P, int NUM, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//CHomingLaser(EDirType type, CPos P, int NUM, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName);
	~CHomingLaser();

	int m_num;
	CPos *m_posA;
	double *m_angleA;

	std::vector<CLimitInfo*> m_limit;

	void AddLimitInfo(int Count, double Limit);

	//���C���s��
	virtual void Action();
	virtual void Draw();

	virtual void Move();
	virtual void RectOut();
	virtual void Remove();
};

/*

CPos pp(CFunc::RandF(400,400), CFunc::RandF(100,100));
for(int i=0;i<12;i++){
	CHomingLaser *hl = new CHomingLaser(EDirType::Player,		//�ˏo����
										pp,			//���W
										20,			//���[�U�[����
										12,			//���x
										i*30.0,		//�ˏo�p�x
										0,0,0,		//�����͗L��
										"hLaser01");//�摜
	hl->AddLimitInfo(30, 5.0);						//30fr�܂ł͊p�x����5.0
	hl->AddLimitInfo(60, 3.0);						//60fr�܂ł͊p�x����3.0
	hl->AddLimitInfo(MAXINT, 0.5);					//�ȍ~�͊p�x����0.5
	bulletManager->Add( hl );
}

*/















