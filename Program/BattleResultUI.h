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
	CBattleResultUI();
	virtual ~CBattleResultUI();

	void Init();
	void Draw();

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

	// 敵の撃破率
	// 	   小
	// 	   中
	// 	   大
	int m_destoryLargeEnemyRatio; // 100.00 => 10000
	int m_destoryMediumEnemyRatio;
	int m_destorySmallEnemyRatio;
	int m_drawDestoryLargeEnemyRatio;
	int m_drawDdestoryMediumEnemyRatio;
	int m_drawDdestorySmallEnemyRatio;
	void DrawDestoryRatio(int destoryEnemyRatio, int posX, int posY);

	// 使用ボム
	int m_usedBomb;
	double m_usedBombSize;
	double m_usedBombAlpha;
	// 被弾数
	int m_missCount;
	double m_missCountSize;
	double m_missCountAlpha;
	// ～率が値が少なくて早く終わった時に時間を早めるフラグ
	bool m_countupFinishEndFlag[4];

	// アイテムの取得率
	int m_getItemRatio; // 100.0 => 10000
	int m_drawGetItemRatio;

	double m_draw1Size[draw1Count];
	int m_draw1Alpha[draw1Count];
	int m_draw1ImageSizeX[draw1Count];

	// 取得スコア
	int m_getScore;


	// 上昇ランク
	int m_getRank;

};

