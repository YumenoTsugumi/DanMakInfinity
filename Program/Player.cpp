
#include "Player.h"
#include "BaseBullet.h"
#include "BattleScene.h"

CPlayer::CPlayer():
	m_pos(0,0),
	m_playerImage(nullptr),
	m_slowMove(0),
	m_bulletShotCount(0)
{
}

CPlayer::~CPlayer() 
{
}

void CPlayer::Init()
{
	m_pos.x = 400;
	m_pos.y = 500;

	m_playerImage = (CImage*)CGame::GetResource(999);
}

void CPlayer::SetBulletManager(CBulletManager* playerBullet)
{
	m_playerBullet = playerBullet;
}


void CPlayer::Action(CInputAllStatus* input)
{
	//移動処理
	double move = 6.0;
	int moveCo = 0;

	if (input->GetBtnOnOff(INPUT_DEF_UP) == true) {
		moveCo++;
	}
	if (input->GetBtnOnOff(INPUT_DEF_DOWN) == true) {
		moveCo++;
	}
	if (input->GetBtnOnOff(INPUT_DEF_RIGHT) == true) {
		moveCo++;
	}
	if (input->GetBtnOnOff(INPUT_DEF_LEFT) == true) {
		moveCo++;
	}
	if (moveCo >= 2) {
		move *= 1.0 / sqrt(2.0);
		m_slowMove = MOVE_SLOW;
	}
	else {
		m_slowMove = MOVE_HI;
	}

	if (input->GetBtnOnOff(INPUT_DEF_UP) == true) {
		m_pos.y -= move;
	}
	if (input->GetBtnOnOff(INPUT_DEF_DOWN) == true) {
		m_pos.y += move;
	}
	if (input->GetBtnOnOff(INPUT_DEF_RIGHT) == true) {
		m_pos.x += move;
	}
	if (input->GetBtnOnOff(INPUT_DEF_LEFT) == true) {
		m_pos.x -= move;
	}

	if (input->GetBtnOnOff(INPUT_DEF_ENTER) == true) {
		if (m_bulletShotCount >= 3) {
			Shot();
		}
		m_bulletShotCount++;
	}
}
void CPlayer::Draw()
{
	CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, 0.25, 0.0, m_playerImage->m_iamge);
}

void CPlayer::Shot()
{
	{
		double speed = 32.0;
		double angle = 270.0;
		CPlayerBullet* b1 = new CPlayerBullet(10, m_pos, speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b1);

		CPlayerBullet* b2 = new CPlayerBullet(10, m_pos + CPos(30, 0), speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b2);

		CPlayerBullet* b3 = new CPlayerBullet(10, m_pos - CPos(30, 0), speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b3);
	}

	{
		CPos relativePos = m_pos + CPos(100, 50);
		double speed = 32.0;
		double angle = 270.0 + 15.0;
		CPlayerBullet* b1 = new CPlayerBullet(10, relativePos, speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b1);

		CPlayerBullet* b2 = new CPlayerBullet(10, relativePos + CPos(30, 0), speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b2);

		CPlayerBullet* b3 = new CPlayerBullet(10, relativePos - CPos(30, 0), speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b3);
	}

	{
		CPos relativePos = m_pos + CPos(-100, 50);
		double speed = 32.0;
		double angle = 270.0 - 15.0;
		CPlayerBullet* b1 = new CPlayerBullet(10, relativePos, speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b1);

		CPlayerBullet* b2 = new CPlayerBullet(10, relativePos + CPos(30, 0), speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b2);

		CPlayerBullet* b3 = new CPlayerBullet(10, relativePos - CPos(30, 0), speed, angle, 0, 0, 0, 0, 998/*"playerBullet"*/);
		m_playerBullet->Add(b3);
	}
}

//---------------------------------------------------------------------------------------------------------------
// プレイヤー専用の弾
//---------------------------------------------------------------------------------------------------------------
//コンストラクタ	弾作成
CPlayerBullet::CPlayerBullet(int damage, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
	CBaseBullet(EDirType::Abs, P, speed, angle, corner, acce, maxSpeed, nearAngle, image),
	m_damage(damage)
{
	m_hitSize = 8.0;
}
CPlayerBullet::CPlayerBullet(int damage, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* imageName) :
	CBaseBullet(EDirType::Abs, P, speed, angle, corner, acce, maxSpeed, nearAngle, imageName),
	m_damage(damage)
{
	m_hitSize = 8.0;
}

// 弾が何かに当たった時
void CPlayerBullet::Hit()
{
	__super::Hit(); // リムーブフラグON

	m_removeFlg = true;

	//CBattleScene::m_effectManager.Add()
}