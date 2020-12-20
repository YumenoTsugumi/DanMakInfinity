#pragma once

#include "Func.h"

class CBaseBullet;
class CBulletImage;
class CBulletImageManager;
class CBulletManager;
class CBulletImageInfo;

class CResourceManager;
#include "Resource.h"

const bool PLAYER = false;
const bool ABS = true;

const int BLEND_NO = DX_BLENDMODE_NOBLEND;
const int BLEND_ALPHA = DX_BLENDMODE_ALPHA;
const int BLEND_ADD = DX_BLENDMODE_ADD;
const int BLEND_SUB = DX_BLENDMODE_SUB;
const int BLEND_MULA = DX_BLENDMODE_MULA;
const int BLEND_INVSRC = DX_BLENDMODE_INVSRC;


//---------------------------------------------------------------------------------
//	CBulletImageInfo
//---------------------------------------------------------------------------------

//�摜�֘A���
//�e�e�����̏���K���P����
//�C���[�W�������̍\���̂����@���̍ێg��Ȃ���񂠂�@*���g��Ȃ����
class CBulletImageInfo{
public:
	int m_imageNo;//*�摜No
	int sizeX, sizeY;	//1��������̉摜�T�C�Y	(�ǂݍ��ݎ��ɐݒ肷��

	bool animeFlg;	//�A�j���[�V�������邩�@false:���Ȃ�	true:����
	char animeNum;	//�A�j���[�V��������	2�`n
	char animePos;	//*���݂̃A�j���[�V�����̈ʒu
	char animeSpeed;	//�A�j�����x	�A�j���̉摜��؂�ւ���fr

	bool rotationFlg;	//��]���邩�@false:���Ȃ�	true:����
	double rotationSpeed;	//��]���x	fr���̉��Z�p�x(rad)
	double rotationAngle;	//*���݂̊p�x(rad)

	//�R���X�g���N�^
	CBulletImageInfo();
	//�S��������
	void init();
	//���͈�����S���Z�b�g
	void Set(	bool animeFlg,char animeNum,char animePos,char animeSpeed,
				bool rotationFlg,double rotationSpeed,double rotationAngle);

	//���ׂđ��
	void operator=(const CBulletImageInfo& info){
		m_imageNo	= info.m_imageNo;
		sizeX	= info.sizeX;
		sizeY	= info.sizeY;
		animeFlg	= info.animeFlg;
		animeNum	= info.animeNum;
		animePos	= info.animePos;
		animeSpeed	= info.animeSpeed;
		rotationFlg	= info.rotationFlg;
		rotationSpeed	= info.rotationSpeed;
		rotationAngle	= info.rotationAngle;
	}
};

//---------------------------------------------------------------------------------
//	CBulletManager
//---------------------------------------------------------------------------------

//�e���Ǘ�����N���X
class CBulletManager{
public:

	//�R���X�g���N�^
	//�e���̔��ˍő吔��ݒ肷��
	//�f�t�H���g1024
	CBulletManager(int m_num = 1024);
	virtual ~CBulletManager();

	//���ˍő吔	1024���炢����Βe���ł���񂶂�Ȃ���
	//�R���X�g���N�^�Őݒ��͕ύX�s��
	int m_bulletTotalNum;

	//�e�̑}���ʒu	���X�ɂ��炵�e���d�Ȃ�Ȃ��悤�ɂ��邽��
	int m_order;

	//�e�f�[�^	�e�N���X��������
	//�|�C���^�����锠
	CBaseBullet** m_bullet;

	virtual void Action();
	virtual void Draw();

	int Add(CBaseBullet *bullet);
};

//---------------------------------------------------------------------------------
//	CBaseBullet
//---------------------------------------------------------------------------------
class CBaseBullet{
public:
	//-----------------------------------
	//	�ÓI�����o�[�@�N���X�𗘗p����
	//	�O�ɐݒ肵�Ă�������
	//-----------------------------------
	//�\���̈�
	static CRect m_rect;
	static void SetRect(CRect &m_rect);

	//���@�_���p���W
	static CPos m_target;
	static void SetTarget(CPos pos);	//��fr�X�V���Ȃ��Ƃ����Ȃ�

	//�摜�f�[�^�ݒ�@�i�n�����m��new�Ŋm�ۂ��邱�Ɓ@����͂��̃N���X(���ۂɂ͏��)������
	static void SetResource(CResourceManager *ImageManager);

protected:
	//-----------------------------------
	//	�摜�f�[�^�֘A
	//-----------------------------------
	//�摜�����f�[�^�̃|�C���^�@�R���X�g���N�^�ŉ摜�̓ǂݍ��݂��s��
	static CResourceManager *m_imageManager;


	//�摜�f�[�^�ւ̃|�C���^�@�����摜�f�[�^����ۑ����Ă���
	int *m_image;
	//�摜���
	CBulletImageInfo m_imageInfo;

public:
	//DX_BLENDMODE_NOBLEND�@:�@�m�[�u�����h�i�f�t�H���g�j
	//DX_BLENDMODE_ALPHA�@�@:�@���u�����h
	//DX_BLENDMODE_ADD�@�@�@:�@���Z�u�����h
	//DX_BLENDMODE_SUB�@�@�@:�@���Z�u�����h
	//DX_BLENDMODE_MULA�@�@�@:�@��Z�u�����h
	//DX_BLENDMODE_INVSRC�@�@:�@���]�u�����h
	//�`����@�@ALPHA��ADD
	int m_blendType;
	void SetBlendType(int BlendType);

	int m_count;	//�J�E���g�@���t���[�����ށ@���낢��g��

	bool m_removeFlg;	//�e����������true�ɂ���ƁA�Ǘ��N���X��������Ă����

	//-----------------------------------
	//	�e���
	//-----------------------------------
	CPos m_pos;
	CPos m_vel;
	bool m_type;	//���@�_�����ǂ���
	double m_angle, m_speed;
	double m_corner, m_accelerate, m_maxsp;

public:
	//-----------------------------------
	//	���C�����[�v
	//-----------------------------------
	//���C���s��
	virtual void Action();
	//���C���`��
	virtual void Draw();

	//�ړ�����
	virtual void Move();
	//�͈͊O
	virtual void RectOut();
	//�폜
	virtual void Remove();

	//�l�ݒ�@��ɃR���X�g���N�^����Ă�
	virtual void Set(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP);
	//�摜�֘A
	virtual void SetImage(int IMG);
	virtual void SetImage(const char* IMAGENAME);
	virtual void SetImageInfo(CBulletImage* bulletImage);

	//�R���X�g���N�^	�e�쐬
	CBaseBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG);
	CBaseBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME);
	//�f�X�g���N�^
	virtual ~CBaseBullet();
};



//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*
//���K�v�w�b�_�[
	#include "\MyFunc\MyInclude.h"

//�����C�����[�v�O
	//----------------
	//	���\�[�X���C��
	//----------------
	CResourceManager resManager;

	//�e���摜
	//�ӂ�����A�����AX�AY�A�T�C�YX�A�T�C�YY�A�A�j�����x�A��]���x
	resManager.Add(new CBulletImage("Image/b0r.png",	1, 1, 1, 32, 32, 0, 0.0), "bullet00", 0);
	resManager.Add(new CBulletImage("Image/b7b.png",	1, 1, 1, 32, 32, 0, 4.0), "bullet01", 1);
	resManager.Add(new CBulletImage("Image/b5b.png",	2, 2, 1, 16, 64, 6, 0.0), "bullet02", 2);
	resManager.Add(new CBulletImage("Image/bullet5.png",4, 4, 1, 32, 32, 8, 0.0), "bullet03", 3);

	//--------------
	//	�e���֌W
	//--------------
	CBulletManager *bulletManager = new CBulletManager(4096);
	CBaseBullet::SetRect(CRect(0,0,800,600));
	CBaseBullet::SetResource(&resManager);
	CNormalBullet::SetBulletManagerPointer(bulletManager);

//�����t���[���X�V���Ȃ���΂Ȃ�Ȃ�
	CBaseBullet::SetTarget(CPos(p,0));

//�����C���g�p���@
	bulletManager->Add( new CBaseBullet(ABS, CPos(100,100), 3, abg, 0,0,0, "bullet00"));
	bulletManager->Add( new CBaseBullet(ABS, CPos(200,100), 3, abg, 0,0,0, 1));

	bulletManager->Action();
	bulletManager->Draw();


//�����
	delete bulletManager;

*/
