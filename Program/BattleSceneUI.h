#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"
#include "BaseBeam.h"

#include "Launcher.h"
#include "BaseEnemy.h"
#include "BaseItem.h"
#include "BackGround.h"
#include "GameDefine.h"

class CBattleSceneUI{
public:
	CBattleSceneUI();
	virtual ~CBattleSceneUI();

	void Init();
	void Draw();

protected:
	// ����
	CImage* m_textScore;
	CImage* m_textHiScore;
	std::vector<CImage*> m_textNumber;
	std::vector<CImage*> m_textBigNumber;
	std::vector<CImage*> m_textItemTakeNumber;
	std::vector<CImage*> m_textItemTakeOther; // %,.
	std::vector<CImage*> m_RankBigNumber;

	CImage* m_text0g;
	CImage* m_textComma;
	CImage* m_textCommag;

	CImage* m_rankWaku;
	CImage* m_rankWaku2;

	// ���݂̃X�e�[�W
	std::vector<CImage*> m_stageNumber;
	CImage* m_stageLogo;

	// �O��
	CImage* m_UIFoundation;

	// �{��
	double m_bombIconSize;
	double m_bombIconSize2;
	CImage* m_bombIcon;
	CImage* m_bombText;
	// �c�@
	CImage* m_playerIcon;

	// �A�C�e��
	CImages* m_textScoreItem;
	double m_rotationAngle; // �A�C�e���̉�]
	double m_itemDrawSize1;
	double m_itemDrawSize2;
	CBulletImage* m_itemImage; // �A�C�e��
	CBulletImage* m_shineImage; // �A�C�e���̋P��
	void DrawItemGetCounter(double x, double xGap, double y, double size, int value);

	void DrawGameAreaUI(); // ��ʓ���UI�`��
	void DrawOutArea(); // ��ʊO�̕`��
	void DrawOutArea_Rank(); // ��ʊO�̕`��rank
	void DrawOutArea_Stage(); // ��ʊO�@���݂̃X�e�[�W

	// �OUI�̃����N�A�j���[�V����
	int m_rankAnime_PreRank = 1000; // �����N�A�j���j�A�j�����n�܂������̃����N�̒l
	int m_rankAnime_TargetRank = 1000; // �����N�A�j���j�ŏI�I�ȃ����N�̒l
	int m_rankAnime_UpRank = 0; // �����N�A�j���j�A�j�����ɏオ���Ă��郉���N���L�^����
	int m_rankAnime_Once = false; // �����N�A�j���jm_rankAnime_PreRank�̐ݒ���ŏ��̈�x
	int m_rankAnime_Count = 0; // �����N�A�j���j�A�j���[�V�����𐧌䂷�邽�߂̃J�E���^�[
	double m_rankAnime_WaveAnimeSize = 1.0; // �����N�A�j���j���_�̔g��p�̃A�j��
	std::vector<int> m_rankAnime_KeepRank; // �����N�@���ɂǂ�ǂ�ǉ�����
	int m_rankAnime_UpdateCount; // ��ʍX�V�^�C�~���O
	int m_rankAnime_RankUpDown; // �����N���オ��̂�������̂��i���̗ʁj
	CImage* m_imageRankRatio; // �����N�㏸���摜
};