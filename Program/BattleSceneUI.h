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
	std::vector<CImage*> m_RankBigNumber;
	CImage* m_text0g;
	CImage* m_textComma;
	CImage* m_textCommag;

	CImage* m_rankWaku;
	CImage* m_rankWaku2;

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
};