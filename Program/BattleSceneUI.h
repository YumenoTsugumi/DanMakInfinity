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
	// UI�֘A
	long long m_hiScore;
	long long m_score;

	// ����
	CImage* m_textScore;
	CImage* m_textHiScore;
	std::vector<CImage*> m_textNumber;
	CPos m_numberMaxSize; // ���l�̒��ň�ԑ傫���T�C�Y���o���Ă���
	CImage* m_text0g;
	CImage* m_textComma;
	CImage* m_textCommag;

	// �O��
	CImage* m_UIFoundation;


	void DrawGameAreaUI(); // ��ʓ���UI�`��
	void DrawOutArea(); // ��ʊO�̕`��
};