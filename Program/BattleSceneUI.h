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
	// UI関連
	long long m_hiScore;
	long long m_score;

	// 内側
	CImage* m_textScore;
	CImage* m_textHiScore;
	std::vector<CImage*> m_textNumber;
	CPos m_numberMaxSize; // 数値の中で一番大きいサイズを覚えておく
	CImage* m_text0g;
	CImage* m_textComma;
	CImage* m_textCommag;

	// 外側
	CImage* m_UIFoundation;


	void DrawGameAreaUI(); // 画面内のUI描画
	void DrawOutArea(); // 画面外の描画
};