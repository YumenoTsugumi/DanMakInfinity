
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
	m_pos.x = GameWindowCenterX;
	m_pos.y = (WindowY *0.8);

	m_playerImage = (CImage*)CGame::GetResource(800);
	m_bitRImage = (CImage*)CGame::GetResource(801);
}

void CPlayer::SetBulletManager(CBulletManager* playerBullet)
{
	m_playerBullet = playerBullet;
}


void CPlayer::Action(CInputAllStatus* input)
{
	//移動処理
	double move = 9.0;
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
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, 1.0, 0.0, m_playerImage->m_iamge);

	static int aaa = 0;
	if (aaa == 0) {
		aaa++;
		m_posBitAngleL[0] = 0;
		m_posBitAngleL[1] = 180.0;
		m_posBitAngleR[0] = 0;
		m_posBitAngleR[1] = 180.0;
	}
	m_posBitAngleL[0] += 3.0;
	m_posBitAngleL[1] += 3.0;
	m_posBitAngleR[0] += 3.0;
	m_posBitAngleR[1] += 3.0;


	CPos posAng1;
	posAng1.x = cos(m_posBitAngleL[0] / 57.27) * 40;
	posAng1.y = 0 * sin(m_posBitAngleL[0] / 57.27) * 20;
	CDxFunc::MyDrawRotaGraph(m_pos.x - 100 + posAng1.x, m_pos.y + 100 + posAng1.y, 0.5, 0.0, m_bitRImage->m_iamge);

	CPos posAng2;
	posAng2.x = cos(m_posBitAngleL[1] / 57.27) * 40;
	posAng2.y = 0 * sin(m_posBitAngleL[1] / 57.27) * 20;
	CDxFunc::MyDrawRotaGraph(m_pos.x - 100 + posAng2.x, m_pos.y + 100 + posAng2.y, 0.5, 0.0, m_bitRImage->m_iamge);

	CPos posAng3;
	posAng3.x = cos(m_posBitAngleL[0] / 57.27) * 40;
	posAng3.y = 0 * sin(m_posBitAngleL[0] / 57.27) * 20;
	CDxFunc::MyDrawRotaGraph(m_pos.x + 100 + posAng3.x, m_pos.y + 100 + posAng3.y, 0.5, 0.0, m_bitRImage->m_iamge);

	CPos posAng4;
	posAng4.x = cos(m_posBitAngleL[1] / 57.27) * 40;
	posAng4.y = 0 * sin(m_posBitAngleL[1] / 57.27) * 20;
	CDxFunc::MyDrawRotaGraph(m_pos.x + 100 + posAng4.x, m_pos.y + 100 + posAng4.y, 0.5, 0.0, m_bitRImage->m_iamge);
}

void CPlayer::Shot()
{
	double speed = 48.0;
	int bulletImage = 998;
	{
		
		double angle = 270.0;
		CPlayerBullet* b1 = new CPlayerBullet(10, m_pos + CPos(0, -30), speed, angle, 0, 0, 0, 0, 997/*"playerBullet"*/);
		m_playerBullet->Add(b1);

		//CPlayerBullet* b2 = new CPlayerBullet(10, m_pos + CPos(30, 0), speed, angle, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		//m_playerBullet->Add(b2);

		//CPlayerBullet* b3 = new CPlayerBullet(10, m_pos - CPos(30, 0), speed, angle, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		//m_playerBullet->Add(b3);
	}

	//{
	//	CPos relativePos = m_pos + CPos(100, 50);
	//	double angle = 270.0 + 15.0;
	//	CPlayerBullet* b1 = new CPlayerBullet(10, relativePos, speed, angle, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
	//	m_playerBullet->Add(b1);

	//	CPlayerBullet* b2 = new CPlayerBullet(10, relativePos + CPos(30, 0), speed, angle, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
	//	m_playerBullet->Add(b2);

	//	CPlayerBullet* b3 = new CPlayerBullet(10, relativePos - CPos(30, 0), speed, angle, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
	//	m_playerBullet->Add(b3);
	//}

	//{
	//	CPos relativePos = m_pos + CPos(-100, 50);
	//	double angle = 270.0 - 15.0;
	//	CPlayerBullet* b1 = new CPlayerBullet(10, relativePos, speed, angle, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
	//	m_playerBullet->Add(b1);

	//	CPlayerBullet* b2 = new CPlayerBullet(10, relativePos + CPos(30, 0), speed, angle, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
	//	m_playerBullet->Add(b2);

	//	CPlayerBullet* b3 = new CPlayerBullet(10, relativePos - CPos(30, 0), speed, angle, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
	//	m_playerBullet->Add(b3);
	//}

	CPos posAng1;
	posAng1.x = cos(m_posBitAngleL[0] / 57.27) * 40;
	posAng1.y = 0 * sin(m_posBitAngleL[0] / 57.27) * 20;
	{
		CPlayerBullet* b1 = new CPlayerBullet(10, CPos(m_pos.x - 100 + posAng1.x - 6, m_pos.y + 100 + posAng1.y), speed, 270.0, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		m_playerBullet->Add(b1);
		CPlayerBullet* b2 = new CPlayerBullet(10, CPos(m_pos.x - 100 + posAng1.x + 6, m_pos.y + 100 + posAng1.y), speed, 270.0, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		m_playerBullet->Add(b2);
	}


	CPos posAng2;
	posAng2.x = cos(m_posBitAngleL[1] / 57.27) * 40;
	posAng2.y = 0 * sin(m_posBitAngleL[1] / 57.27) * 20;
	{
		CPlayerBullet* b2 = new CPlayerBullet(10, CPos(m_pos.x - 100 + posAng2.x - 6, m_pos.y + 100 + posAng2.y), speed, 270.0, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		m_playerBullet->Add(b2);
		CPlayerBullet* b3 = new CPlayerBullet(10, CPos(m_pos.x - 100 + posAng2.x + 6, m_pos.y + 100 + posAng2.y), speed, 270.0, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		m_playerBullet->Add(b3);
	}


	CPos posAng3;
	posAng3.x = cos(m_posBitAngleR[0] / 57.27) * 40;
	posAng3.y = 0 * sin(m_posBitAngleR[0] / 57.27) * 20;
	{
		CPlayerBullet* b3 = new CPlayerBullet(10, CPos(m_pos.x + 100 + posAng3.x - 6, m_pos.y + 100 + posAng3.y), speed, 270.0, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		m_playerBullet->Add(b3);
		CPlayerBullet* b4 = new CPlayerBullet(10, CPos(m_pos.x + 100 + posAng3.x + 6, m_pos.y + 100 + posAng3.y), speed, 270.0, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		m_playerBullet->Add(b4);
	}


	CPos posAng4;
	posAng4.x = cos(m_posBitAngleR[1] / 57.27) * 40;
	posAng4.y = 0 * sin(m_posBitAngleR[1] / 57.27) * 20;
	{
		CPlayerBullet* b4 = new CPlayerBullet(10, CPos(m_pos.x + 100 + posAng4.x - 6, m_pos.y + 100 + posAng4.y), speed, 270.0, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		m_playerBullet->Add(b4);
		CPlayerBullet* b5 = new CPlayerBullet(10, CPos(m_pos.x + 100 + posAng4.x + 6, m_pos.y + 100 + posAng4.y), speed, 270.0, 0, 0, 0, 0, bulletImage/*"playerBullet"*/);
		m_playerBullet->Add(b5);
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
//CPlayerBullet::CPlayerBullet(int damage, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* imageName) :
//	CBaseBullet(EDirType::Abs, P, speed, angle, corner, acce, maxSpeed, nearAngle, imageName),
//	m_damage(damage)
//{
//	m_hitSize = 8.0;
//}

// 弾が何かに当たった時
void CPlayerBullet::Hit()
{
	__super::Hit(); // リムーブフラグON

	m_removeFlg = true;
	m_enableRemoveEffect = false; // エフェクトなし
	// "FireballExplosion2", 20500
	// "FireballExplosion3", 20501
	// "FireballExplosion7", 20502
	// "FireballExplosion10", 20503

	CPos pp(CFunc::RandF(0, WindowX), CFunc::RandF(0, 400));
	double ang = CFunc::RandF(0, 360);
	CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_pos, 0.1, ang, 0, 0, 0, 0, 20600);
	eff->SetSize(0.35, +0.0);
	eff->SetBlend(64, -0, 0);
	eff->SetBlendType(DX_BLENDMODE_ADD);
	eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
	eff->SetRemoveCount(60);	//60frで削除

	CBattleScene::m_effectManager.Add(eff);
}