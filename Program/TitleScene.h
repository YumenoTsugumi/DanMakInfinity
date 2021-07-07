#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"

#include <functional>

constexpr int SelectItemNum = 5;
constexpr int SelectRankItemNum = 6;
constexpr int SelectRankItems[SelectRankItemNum] = {1,20,40,60,80,100};

constexpr int SelectWeaponMenuNum = 5;
constexpr int SelectWeaponNum = 4; // ���펩�̂�4��

class CTitleScene : public CScene{
public:
	//------------------
	//	���܂��Ȃ�
	//------------------
	CTitleScene(int InTime);
	virtual ~CTitleScene();

	//�s��	����isTopScene�͈�ԏ�̕`�悩�ǂ����ł���B�ォ��n�����̂ŋC�ɂ��Ȃ��ėǂ�
	virtual void Main(CInputAllStatus *input);

	//------------------
	//	�������牺�͎��R��
	//------------------
	void Init(CGame* gameP);



	// �֐��|�C���^
	std::function<void(CTitleScene*)> m_selectItemFunc[SelectItemNum];

	static void Play(CTitleScene* thisScene);
	static void Extra(CTitleScene* thisScene);
	static void Result(CTitleScene* thisScene);
	static void Config(CTitleScene* thisScene);
	static void Exit(CTitleScene* thisScene);

	CGame *m_game; // ��ʂ̃Q�[���N���X


	//------------------------------------------------------
	void Draw();
	void Action(CInputAllStatus* input);
	// ���S�ް�
	CPos m_titleLog1Pos;
	double m_titleLog1Size;
	double m_titleLog1Alpha;

	CImage* m_titleLog1;

	CPos m_titleLog2Pos;
	double m_titleLog2Size;
	double m_titleLog2Alpha;
	double m_titleLog2Angle;
	CPos m_titleLog3Pos;
	double m_titleLog3Size;
	double m_titleLog3Alpha;
	CImage* m_titleLog2;
	CImage* m_titleLog3;

	// �w�i
	CImage* m_background;
	CImage* m_planet1;
	CImage* m_planet2;
	CImage* m_planet3;

	// �I����
	int m_selectIndex;
	CImage* m_selectItem[SelectItemNum][2]; // ��A�N�e�B�u�A�A�N�e�B�u
	int m_selectItemSizeWidth[SelectItemNum];
	int m_selectItemSizeMaxWidth;

	int m_topTitleAnimeCount;
	bool m_top_animetionFinishFlag;
	int m_top_textBlinkAnimeCount;
	//------------------------------------------------------
	void Draw_PlayStandby();
	void Action_PlayStandby(CInputAllStatus* input);
	CImage* m_WeaponSelect_StartRank;
	int m_playStandby_selectIndex;
	CImage* m_playStandby_selectItem[SelectRankItemNum][2]; // ��A�N�e�B�u�A�A�N�e�B�u
	int m_playStandby_selectItemSizeWidth[SelectItemNum];
	int m_playStandby_selectItemSizeMaxWidth;

	// ����I�����
	void Draw_WeaponSelect();
	void Action_WeaponSelect(CInputAllStatus* input);
	CImage* m_WeaponSelect_SelectWeapon;
	CImage* m_WeaponSelect_Left;
	CImage* m_WeaponSelect_Right;
	CImage* m_WeaponSelect_Rapid;
	CImage* m_WeaponSelect_Slow;
	CImage* m_WeaponSelect_Speed[2]; // �ړ����x
	CImage* m_WeaponSelect_ShotType[2]; // �V���b�g�^�C�v
	CImage* m_WeaponSelect_Sortie[2]; // �o��
	CImage* m_WeaponSelect_Weapon[4][2]; // ����
	CImage* m_WeaponSelect_Number[10][2]; // ���l
	int m_WeaponSelect_selectIndex;
	int m_SelectedSpeed[2]; // ���s�b�h�@�X���[
	int m_SelectedWeapon[2]; // ���s�b�h�@�X���[

	// ���U���g���
	void Draw_Result();
	void Draw_ResultSub(int posY, int number, bool nulldata,SaveDatus& s);
	void Action_Result(CInputAllStatus* input);
	CImage* m_ResultImage;
	int m_resultIndex;
	CImage* m_strScoreImage[10];
	CImage* m_strRankingGold;
	CImage* m_strRankingSliver;
	CImage* m_strRankingBronzeImage[10];
	CImage* m_strImage[37];
	CImage* m_strNumber[10];
	CImage* m_colonImage;
	CImage* m_slashImage;


	// �V�[���ɖ߂��Ă������̏��
	enum SceneReturnStatus {
		Nothing = 0, // �{���ɍŏ�
		Retry,
		GoTitle,
		GameOver,
		GoResult
	};
	SceneReturnStatus m_sceneReturnStatus;
	void SetReturnStatus_Retry();
	void SetReturnStatus_GoTitle();
	// �J�n�����N,�I�������N,��������,���s�b�h�V���b�g,�X���[�V���b�g,�A�C�e����,�X�R�A
	void SetReturnStatus_GameOver(
		int startRank, int endRank, 
		int stage, int item, int liveTime, 
		int rapidShot, int rapidspeed, int slowShot, int slowspeed, long long score);
	void SetReturnStatus_GoResult();

	// ��ʂ̏�
	enum Status{
		Top = 0, // TOP���
		PlayStandby, // Play�����āA�����N�⎩�@��I��
		PlayWeaponSelect, // �����N��I�񂾌�Ɏ��@��I��
		ExtraStandby,
		ShowResult, // ���U���g���
	};
	Status m_status;
};