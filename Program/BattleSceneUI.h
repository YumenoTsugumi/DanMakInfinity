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
	// 内側
	CImage* m_textScore;
	CImage* m_textHiScore;
	std::vector<CImage*> m_textNumber;
	std::vector<CImage*> m_textBigNumber;
	std::vector<CImage*> m_textItemTakeNumber;
	std::vector<CImage*> m_RankBigNumber;
	CImage* m_text0g;
	CImage* m_textComma;
	CImage* m_textCommag;

	CImage* m_rankWaku;
	CImage* m_rankWaku2;

	// 外側
	CImage* m_UIFoundation;

	// ボム
	double m_bombIconSize;
	double m_bombIconSize2;
	CImage* m_bombIcon;
	CImage* m_bombText;
	// 残機
	CImage* m_playerIcon;

	// アイテム
	CImages* m_textScoreItem;
	double m_rotationAngle; // アイテムの回転
	double m_itemDrawSize1;
	double m_itemDrawSize2;
	CBulletImage* m_itemImage; // アイテム
	CBulletImage* m_shineImage; // アイテムの輝き
	void DrawItemGetCounter(double x, double xGap, double y, double size, int value);

	void DrawGameAreaUI(); // 画面内のUI描画
	void DrawOutArea(); // 画面外の描画
	void DrawOutArea_Rank(); // 画面外の描画rank
};