
#pragma once

#include "Game.h"
#include "Resource.h"
#include "BaseBullet.h"

const int MOVE_HI = 0;
const int MOVE_SLOW = 1;

class CBulletManager;
class CPlayer {
public:
	CPos m_pos;

	double m_posBitAngleL[2];
	double m_posBitAngleR[2];

	CImage* m_playerImage;
	CImage* m_bitRImage;
	int m_slowMove;	//0(MOVE_HI)通常　1(MOVE_SLOW)低速

	int m_bulletShotCount;
	CBulletManager* m_playerBullet; // 上位のシーンからポインタをもらう

	CPlayer();
	~CPlayer();
	void Init();
	void SetBulletManager(CBulletManager* playerBullet);

	void Action(CInputAllStatus* input);
	void Draw();
	void Shot();
	
};

//---------------------------------------------------------------------------------
//	CBaseBullet
//---------------------------------------------------------------------------------
class CPlayerBullet : public CBaseBullet {
public:
	//コンストラクタ	弾作成
	CPlayerBullet(int damage, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//CPlayerBullet(int damage, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* imageName);

	double m_hitSize; // 当たり判定
	int m_damage; // 与えるダメージ

	// 弾が何かに当たった時
	void virtual Hit();
};