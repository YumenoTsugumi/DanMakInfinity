#pragma once

#include "Func.h"
#include "BaseBullet.h"

class CBeamImage;
class CBeamImageManager;
class CBeamManager;
class CBaseBeam;
class CBaseBeamChild;

//---------------------------------------------------------------------------------
//	CBeamManager
//---------------------------------------------------------------------------------
class CBeamManager {
public:
	//�R���X�g���N�^
	//�e���̔��ˍő吔��ݒ肷��
	//�f�t�H���g64
	CBeamManager(int num = 64);
	~CBeamManager();

	//���ˍő吔	64
	//�R���X�g���N�^�Őݒ��͕ύX�s��
	int m_beamTotalNum;

	//�e�̑}���ʒu	���X�ɂ��炵�e���d�Ȃ�Ȃ��悤�ɂ��邽��
	int m_order;

	//�e�f�[�^	�e�N���X��������
	CBaseBeam** m_beam;

	virtual void Action();
	virtual void Draw();

	int Add(CBaseBeam* beam);

};

//---------------------------------------------------------------------------------
//	CBaseBeam
//---------------------------------------------------------------------------------

//���[�U�[��n�̒e�ō\�������
//Draw���@�����`���ς���
class CBaseBeam : public CBulletManager {
	//�\���p�x(rad)
	double m_drawAngle;
	double m_drawSize;

public:
	//�摜�ւ̃|�C���^
	//CBeamImage *m_image;
	//CImageSet* m_image;
	CImage* m_imageStand;
	CImage* m_imageLaser;


	////�摜�f�[�^�ݒ�@�i�n�����m��new�Ŋm�ۂ��邱�Ɓ@����͂��̃N���X(���ۂɂ͏��)������
	//static void SetResource(CResourceManager* ImageManager);
	////�摜�����f�[�^�̃|�C���^�@�R���X�g���N�^�ŉ摜�̓ǂݍ��݂��s��
	//static CResourceManager* m_imageManager;

	//������ƍL�߂͈̔�
	static CRect m_bigRect;
	static void SetBigRect(CRect& bigrect);

	//�R���X�g���N�^
	//���[�U�[�P�{�̔��ˍő吔��ݒ肷��
	//�f�t�H���g128�ō\�������
	//800*600�Œ[����[�܂Ł@���x10�Ԋu1�Ȃ�128�ŏ\��
	CBaseBeam(CPos P, double angle, int imageIndex1, int imageIndex2, int num = 128);
	CBaseBeam(CPos P, double angle, const char* imageName1, const char* imageName2, int num = 128);
	virtual ~CBaseBeam();
	virtual void Set(CPos pos, double m_speed, double m_angle, int length, double m_thick, int m_shotSpan);

	virtual void SetImage(int imageIndex1, int imageIndex2);
	virtual void SetImage(const char* imageName1, const char* imageName2);

	//�ʒu
	CPos m_pos;
	//���ˊp�x(deg)
	double m_angle;
	//����(�����ł͖����A�J�E���g
	int m_length;
	void SetLength(int Length);
	//�摜���
	int m_imgNo;

	//�r�[���Ǝ˂܂ł̑ҋ@����
	int m_wait;
	void SetWait(int Wait);

	//�폜���ԁ@�������Ă���
	int m_removeTime;
	void SetRemoveTime(int DelTime);
	void SetRemove();
	//�폜	Yes�ō폜��
	bool m_removeNow;
	//�폜���ɏ��������Ă������߁@��{��1.0�{�@��0.0�{�܂�
	double m_removeSize;

	//���x	���邩�ȁc
	double m_speed;
	//����	1.0�{���f�t�H���g
	double m_thick;
	//���ˊԊu	���邩�ȁc
	int m_shotSpan;

	//�r�[���ړ���]	�ړ���̍��W	��]�ʁ@5.0�x����
	void Move(CPos& pos, double plusAngle);

	//�폜�t���O	�Ǘ��N���X������
	bool m_removeFlg;
	//��������
	int m_count;

	virtual void Action();
	virtual void Draw();

	//�v�f�폜�̑O�@�G�t�F�N�g�o������
	void Remove();
	//�v�f�폜
	void Clear();
};

//---------------------------------------------------------------------------------
//	CBaseBeamChild
//---------------------------------------------------------------------------------
//�r�[���̔j�Ђ�e�ŕ\������
class CBaseBeamChild : public CBaseBullet {
public:

	//�R���X�g���N�^	�e�쐬
	CBaseBeamChild(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//�f�X�g���N�^
	virtual~CBaseBeamChild();

	//�͈͊O
	virtual void RectOut();

	//�G�t�F�N�g�o�������Ȃ�����
	virtual void Remove();

	//�����̈ʒu�i�ˏo����邽�уC���N�������g�����
	//�q���S�̂ő����Ă���
	int idx;
};



//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*

	//--------------
	//�r�[���֌W������
	//--------------
	//�폜�͈͂͏����L�߂Ɏ��
	CBaseBeamChild::SetBigRect(CRect(	0 - 100,
										0 - 100,
										800 + 100,
										600 + 100));
	//�摜
	//���[�U�[�p�̉摜�ǂݍ���
	{	const char fileA[][RESOURCENAMEMAX] = {	"Image/bulletstand_b.png","Image/childLaser_b.png"	};
		resManager.Add(new CImageSet(2, fileA), "laser01", 100);	}
	{	const char fileA[][RESOURCENAMEMAX] = {	"Image/bulletstand_r.png","Image/childLaser_r.png"	};
		resManager.Add(new CImageSet(2, fileA), "laser02", 101);	}

	//�r�[���Ǘ��N���X
	CBeamManager *beamManager = new CBeamManager(64);


	//���ʂɔ���
	CPos ppp(300,300);
	CPos vel(1.0,1.0);
	double ang = 0.0;
	CBaseBeam *beam1 = new CBaseBeam(ppp, ang, 0);
	beamManager->Add(beam1);

	//���[�v����
	while(ProcessMessage() == 0){
		ClearDrawScreen();

		//�ړ���]
		ppp += vel;
		beam1->Move(ppp, 1.0);



		CPos p(400, 100);
		CBaseBeam *m_beam = new CBaseBeam(p, m_angle, 0);

		//�����ݒ� 1�`64�܂�	�s�N�Z���Ō�����*10�܂ŐL�т�
		m_beam->SetLength(32);
		//�Ǝ˂܂ł̑ҋ@���ԁ@fr�P��
		m_beam->SetWait(24);
		//�폜����
		m_beam->SetRemoveTime(120);

		beamManager->Add(m_beam);
	}


//��


	//��]�Ƃ��Ŗ��t���[���Q�Ƃ����Ɨ�����̂�
	//�r�[���͊Ǘ�����SetRemove���Ȃ��ƍ폜���Ȃ�

	//�폜���Ԃ𒴂�����폜
	//�����ɂ́Adeltime��ݒ肷�邩�Ǘ�����SetRemove����

*/











