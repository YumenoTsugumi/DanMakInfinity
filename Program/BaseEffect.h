#pragma once

#include "Func.h"
#include "BaseBullet.h"


class CBaseEffect;
class CEffectManager;

//---------------------------------------------------------------------------------
//	CEffectManager
//---------------------------------------------------------------------------------
//�e�̊Ǘ��}�l�[�W���Ɠ����@���O�I�Ɍp����������

class CEffectManager : public CBulletManager{
public:

	//�R���X�g���N�^
	//�e���̔��ˍő吔��ݒ肷��
	//�f�t�H���g1024
	CEffectManager(int m_num = 512);
	virtual ~CEffectManager();

};


//---------------------------------------------------------------------------------
//	CBaseEffect
//---------------------------------------------------------------------------------
//�G�t�F�N�g���e�Ǝ��Ă���
class CBaseEffect : public CBaseBullet{

public:

	//���̔Z���@0�`255	�g�p���ɂ�int�ɃL���X�g���� 
	double m_blendDepth;

	//�Z���̉����Z
	double m_plusBlend;
	void SetBlend(int BlendDepth, double PlusBlend);

	//�`��T�C�Y
	double m_size;
	//�T�C�Y�̉����Z
	double m_plusSize;
	//���̒l�𒴂�����폜����
	double m_deleteOverSize;
	void SetSize(double Size, double PlusSize);	//�폜����ۂ̋}���k���l
	void SetdeleteOverSize(double deleteOverSize);	//�폜����ۂ̋}���Z�x

	//CBaseBullet::SetBlendType
	//DX_BLENDMODE_NOBLEND�@:�@�m�[�u�����h�i�f�t�H���g�j
	//DX_BLENDMODE_ALPHA�@�@:�@���u�����h
	//DX_BLENDMODE_ADD�@�@�@:�@���Z�u�����h
	//DX_BLENDMODE_SUB�@�@�@:�@���Z�u�����h
	//DX_BLENDMODE_MULA�@�@�@:�@��Z�u�����h
	//DX_BLENDMODE_INVSRC�@�@:�@���]�u�����h
private:
	//�폜����
	bool m_removeNow;
public:
	//�폜���钆�̌��Z�l
	double m_deleteTimingPlusAlpha;
	double m_deleteTimingPlusSize;
	void SetDeleteTiming(double DeleteTimingPlusAlpha, double DeleteTimingPlusSize);

	//�A�j���[�V����������摜�ŃA�j�����I�������폜���邩�ǂ���
	//true:�폜����	false:���Ȃ�
	bool m_animeEndDelFlg;
	void SetAnimeEndDelFlg(bool AnimeEndDelFlg);

	//���Ԃō폜����
	int m_removeCount;
	void SetRemoveCount(int DelCount);

	//��͓K���ɉ���������p��
	void (*funcP)(CBaseEffect* eff);
	void SetFuncP(void (*FuncP)(CBaseEffect* eff));

	//�R���X�g���N�^	�e�쐬
	CBaseEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG);
	CBaseEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME);

	//�f�X�g���N�^
	virtual ~CBaseEffect();
	virtual void Set();
	//virtual void SetImage(int IMG);

	//�Ƃ肠�����S���p�����Ƃ�
	//���C���s��
	virtual void Action();
	//���C���`��
	virtual void Draw();

	virtual void EffectAction();

	//�ړ�����	�e�̂𗬗p
	//virtual void Move();
	//�͈͊O	�R�[�����Ȃ�
	//virtual void RectOut();
	//�폜
	virtual void Remove();
};

//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*

���ʂ̒e���N���X�Ɠ����g�������ł���


	CPos pp(CFunc::RandF(0,800), CFunc::RandF(0,400));
	double ang = CFunc::RandF(0,360);
	CBaseEffect* eff = new CBaseEffect(ABS, pp, 2.0, ang, 0,0,0, 206);
	eff->SetSize(3.0, +0.05);
	eff->SetBlend(192, +3.0);

	eff->SetAnimeEndDelFlg(false);	//�A�j���[�V�����I����폜���邩
	eff->SetRemoveCount(60);	//60fr�ō폜

	effectManeger1->Add(eff);


*/







//---------------------------------------------------------------------------------
//	CBaseEffect
//---------------------------------------------------------------------------------
const int STRINGEFFECTMAX = 80;
//�C���[�W�`�悹���ɕ�����`�悷��
class CStringEffect : public CBaseEffect{

public:

private:
	char drawString[STRINGEFFECTMAX];
public:
	CFont *font;
	int fontCr;
	int edgeCr;

	//�R���X�g���N�^	�e�쐬
	CStringEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, char* DRAWSTR, int FONT);
	CStringEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, char* DRAWSTR, const char* FONT);

	//�f�X�g���N�^
	virtual ~CStringEffect();
	//virtual void Set();
	//virtual void SetImage(int IMG);

	//�Ƃ肠�����S���p�����Ƃ�
	//���C���s��
	virtual void Action();
	//���C���`��
	virtual void Draw();

	//virtual void EffectAction();

	//�ړ�����	�e�̂𗬗p
	//virtual void Move();
	//�͈͊O	�R�[�����Ȃ�
	//virtual void RectOut();
	//�폜
	//virtual void Remove();
};

