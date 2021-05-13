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

constexpr int draw1Count = 7;





class CBattleResultUI{
public:
	enum BattleResultUIStatus {
		Now = 0, // �`�撆
		FeedOut = 1, // �I����ăt�F�[�h�A�E�g��
		Finish = 2 // ���S�ɏI��
	};

	CBattleResultUI();
	virtual ~CBattleResultUI();

	void Init();
	void Draw();

	BattleResultUIStatus GetStatus() {
		return status;
	}
	BattleResultUIStatus status;
protected:
	int m_count;

protected:
	CImage* m_resultUI_A;
	CImage* m_resultUI_AA;
	CImage* m_resultUI_AB;
	CImage* m_resultUI_AC;
	CImage* m_resultUI_B;
	CImage* m_resultUI_C;
	CImage* m_resultUI_D;
	CImage* m_resultUI_E;
	CImage* m_resultUI_F;
	CImage* m_resultUI_N0;
	CImage* m_resultUI_N1;
	CImage* m_resultUI_N2;
	CImage* m_resultUI_N3;
	CImage* m_resultUI_N4;
	CImage* m_resultUI_N5;
	CImage* m_resultUI_N6;
	CImage* m_resultUI_N7;
	CImage* m_resultUI_N8;
	CImage* m_resultUI_N9;
	CImage* m_resultUI_Np;
	CImage* m_resultUI_Nc;
	CImage* m_resultUI_Nd;

	CImage* m_resultUI_rankS;
	CImage* m_resultUI_rankA;
	CImage* m_resultUI_rankB;
	CImage* m_resultUI_rankC;
	CImage* m_resultUI_rankD;
	CImage* m_resultUI_rankE;
	CImage* m_resultUI_uiaura;

public:
	void Set(	int destoryLargeEnemyRatio,
				int destoryMediumEnemyRatio,
				int destorySmallEnemyRatio,
				int usedBomb,
				int missCount,
				int getItemRatio,
				int getScore,
				int getRank
	);

	// �G�̌��j��
	// 	   ��
	// 	   ��
	// 	   ��
	int m_destoryLargeEnemyRatio; // 100.00 => 10000
	int m_destoryMediumEnemyRatio;
	int m_destorySmallEnemyRatio;
	int m_drawDestoryLargeEnemyRatio;
	int m_drawDdestoryMediumEnemyRatio;
	int m_drawDdestorySmallEnemyRatio;
	void DrawDestoryRatio(int destoryEnemyRatio, int posX, int posY);
	void DrawDigitNumber2(int number, int posX, int posY);

	// �g�p�{��
	int m_usedBomb;
	double m_usedBombSize;
	double m_usedBombAlpha;
	// ��e��
	int m_missCount;
	double m_missCountSize;
	double m_missCountAlpha;
	// �`�����l�����Ȃ��đ����I��������Ɏ��Ԃ𑁂߂�t���O
	bool m_countupFinishEndFlag[4];

	// �A�C�e���̎擾��
	int m_getItemRatio; // 100.0 => 10000
	int m_drawGetItemRatio;

	double m_draw1Size[draw1Count];
	int m_draw1Alpha[draw1Count];
	int m_draw1ImageSizeX[draw1Count];

	// �����N�̌��ɂ���s�J�s�J
	double m_rankDrawSize;
	double m_rankDrawAlpha;
	double m_rankAuraAngle[4];

	// �擾�X�R�A
	int m_getScore;
	int m_drawGetScore;
	int m_getScoreImageSizeX;
	double m_getScoreDrawSize;
	double m_getScoreDrawAlpha;

	// �㏸�����N
	int m_getRank;


	// �S�̂̃�lpha
	double m_allAlphaRatio;
};

