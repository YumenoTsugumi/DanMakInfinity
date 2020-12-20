#pragma once

#include "DxLib.h"
#include "DXFunc.h"
#include "Input.h"
#include "Func.h"
//#include "CBaseEffect.h"
//#include "CCustomBullet.h"
#include "Resource.h"

class CSceneManager;
class CScene;
class CMenu;

//�t�F�[�h�X�e�[�^�X		
const int PREFEED = 0;	//�t�F�[�h�O(0)
const int NOWFEED = 1;	//�t�F�[�h��(1)
const int ENDFEED = 2;	//�t�F�[�h�I��(2)

//------------------------------
//	�V�[���̓V���O���g���ɂ��Ƃ��܂���
//------------------------------
class CSceneManager{
public:
	CSceneManager();
	~CSceneManager();

	//�V�[����ςݏグ�Ă���
	//��Ԍ��̃V�[�������݂̃V�[��
	//���̌��̃V�[���̌��ɂ��郁�j���[�͕`�悷��

	//ex)�V�[��1���V�[��2���V�[��3�����j��1�����j��2
	//���̏ꍇ�@�V�[��3�`��@���j��1�`��@���j��2�`��Ƃ�������
	//�v�f�𓮓I�Ɋm�ۂ���
	//CVoidArray m_scene;
	std::vector<CScene*> m_scene;

	//���C�����[�v
	void Main(CInputAllStatus *input);

	//�V�[���ǉ�	��Ԍ��ɒǉ������
	void AddScene(CScene* Scene);

};


//���
//�^�C�g����ʂ�������A�Q�[����ʂ�������
class CScene{
public:
	CScene(int InTime);
	virtual ~CScene();


	//�摜�f�[�^�ݒ�@�i�n�����m��new�Ŋm�ۂ��邱�Ɓ@����͂��̃N���X(���ۂɂ͏��)������
	static void SetResource(CResourceManager *ResManager);
	//�摜�����f�[�^�̃|�C���^�@�R���X�g���N�^�ŉ摜�̓ǂݍ��݂��s��
	static CResourceManager *m_resManager;

	//�V�[���}�l�[�W���[�@���ꂪ�ݒ肳��Ă��Ȃ��ƃV�[�����V�[����ǉ��ł��Ȃ�
	//static�Ȃ͕̂s�������Ǘǂ��ĂȂ��E�E�E
	static CSceneManager* m_sceneManager;
	static void SetSceneManager(CSceneManager *SceneManager);

	//�s��	����isTopScene�͈�ԏ�̕`�悩�ǂ���
	virtual void Main(CInputAllStatus *input);

	void Init();
	void Set(int InTime);

	//---------------------
	//�t�F�[�h�C���A�A�E�g
	//---------------------

	//�t�F�[�h�Ɋ|���鎞��
	int m_inTime;
	int m_outTime;

	//�t�F�[�h�̎��Ԃ��L�����Ă���
	int m_inNowTime;
	int m_outNowTime;

	//�t�F�[�h�̏�Ԃɂ���
	//�t�F�[�h�O(0)	�t�F�[�h��(1)	�t�F�[�h�I��(2)
	//const int PREFEED = 0;	//�t�F�[�h�O(0)
	//const int NOWFEED = 1;	//�t�F�[�h��(1)
	//const int ENDFEED = 2;	//�t�F�[�h�I��(2)
	char m_inStatus;
	char m_outStatus;

	//�t�F�[�h�p�̔Z�x(0.0�`1.0)
	//����ňÓ]�₽�炢�낢��\������
	double m_inBlend;
	double m_outBlend;

	virtual void FeedIn();
	virtual void FeedOut();
	//�t�F�[�h����
	bool NowFeed() const;

	//�t�F�[�h�A�E�g�����邩�ǂ���
	//�t�F�[�h�A�E�g�������Ȃ�����true�ɂ��āA���Ԃ�ݒ肷��
	bool m_outFlg;
	void SetFeedOut(int OutTime);

	//���̃V�[���Ɉȍ~����
	void SetNextScene(CScene* NextScene);
	//���݂̃V�[�����폜���ĂP��O�ɖ߂�
	void SetBackScene();

	//�ǂ��炩���ݒ肳��Ă���ꍇ�A�����Е��͐ݒ�ł��Ȃ��B
	bool m_backScene;
	CScene* m_nextScene;


	//�t�F�[�h�A�E�g���I����Ă���@���@�t�F�[�h�A�E�g���ݒ肳��Ă��炸
	//���@m_nextScene���ݒ肳��Ă���ꍇ
	//�V�[����J�ڂ�����
	void ChangeScene();

	//---------------------
	//���̑�
	//---------------------

	//�S�V�[���ɕK�v�����Ȃ���
	int m_count;

};


//----------------------------------------------------------
//	��
//----------------------------------------------------------

/*
//��j
CSceneExMain	+	CSceneExSub1	+ Menu
									+ Menu

				+	CSceneExSub2	+ Menu
				+	CSceneExSub3	+ Menu

*/
class CSceneExMain : public CScene{
public:
	//------------------
	//	���܂��Ȃ�
	//------------------
	CSceneExMain(int InTime);
	virtual ~CSceneExMain();

	//�s��	����isTopScene�͈�ԏ�̕`�悩�ǂ����ł���B�ォ��n�����̂ŋC�ɂ��Ȃ��ėǂ�
	virtual void Main(CInputAllStatus *input);

	//------------------
	//	�������牺�͎��R��
	//------------------

	int font1;
	int font2;

	//CEffectManager *effectManeger1;

	int selectPos;

};

class CSceneExSub1 : public CScene{
public:
	//------------------
	//	���܂��Ȃ�
	//------------------
	CSceneExSub1(int InTime);
	virtual ~CSceneExSub1();

	virtual void Main(CInputAllStatus *input);

	//------------------
	//	�������牺�͎��R��
	//------------------

	int m_font;

	//CBulletManager *bulletManeger1;

	int selectPos;
};



/*
How To Use

CSceneExMain
CSceneExSub1



*/