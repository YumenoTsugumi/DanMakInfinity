
#include "Player.h"
#include "BaseBullet.h"
#include "BattleScene.h"

CPlayer::CPlayer():
	m_pos(0,0),
	m_playerImage(nullptr),
	m_slowMove(0),
	m_bulletShotCount(0),
	m_bulletMainShotCount(0)
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
	m_hitMakerImage = (CImage*)CGame::GetResource(850);

	m_playerBarrier1 = (CImage*)CGame::GetResource(980);
	m_playerBarrier2 = (CImage*)CGame::GetResource(981);

	m_posBitAngleL[0] = 0;
	m_posBitAngleL[1] = 180.0;
	m_posBitAngleR[0] = 0;
	m_posBitAngleR[1] = 180.0;

	m_bombOn = false;
	m_bombCount = 0;

	m_barrierAngle = 0.0;
	m_barrierAlpha = 1.0;
}

void CPlayer::SetBulletManager(CBulletManager* playerBullet)
{
	m_playerBullet = playerBullet;
}


void CPlayer::Action(CInputAllStatus* input)
{
	//移動処理
	double move = 11.0;
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
		//constexpr double r2 = 1.0 / sqrt(2.0);
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
			m_bulletShotCount = 0;
		}
		m_bulletShotCount++;

		if (m_bulletMainShotCount >= 2) {
			MainShot();
			m_bulletMainShotCount = 0;
		}
		m_bulletMainShotCount++;
	}
	if (input->GetBtnOnOff(INPUT_DEF_CANCEL) == true) {
		BomberSet();
	}
	Bomber();

	// 無敵時間
	MutekiTime();
}


constexpr int bomberStartDist = 600;
constexpr int bomberEndDist = 50;
constexpr int bomberTime1 = 60 * 1; // 爆発が収束する時間
constexpr int bomberTime2 = bomberTime1 + 60*0.3; // 爆発が始まる時間
constexpr int bomberFinishTime = bomberTime2 + 60 * 2; // 終わる時間
constexpr int bomberTotalDamage = 600;

constexpr int startDist = 200;
constexpr int endDist = 50;
constexpr int mutekiTime = bomberFinishTime + 60 * 1;
constexpr int mutekiTimeAlphaDown = mutekiTime * 0.1;

void CPlayer::BomberSet() {
	int bomb = CBattleScene::GetHaveBomb();
	if (bomb <= 0) {
		return;
	}
	if (!m_bombOn) {
		m_bombOn = true;
		m_bombCount = 0;
		m_mutekiCount = mutekiTime;
		m_barrierAlpha = 1.0;
		m_bomberDist = bomberStartDist;
		m_bomberPos = m_pos + CPos(0, -300);
		if (m_bomberPos.y <= 100) {
			m_bomberPos.y = 100;
		}
		CBattleScene::BombDecrement();
	}
}

void CPlayer::MutekiTime() {
	if (m_mutekiCount <= 0) {
		return;
	}
	m_mutekiCount--;
}

void CPlayer::Bomber()
{
	if (!m_bombOn)return;

	int fireball_image4[4] = { 982 ,983 ,984 ,985 };
	int image4[4] = { 20503 ,20501 ,20501 ,20501 };

	if (m_bombCount <= bomberTime1) {
		
		if (m_bomberDist > bomberEndDist) {
			m_bomberDist -= (double)(bomberStartDist - bomberEndDist) / bomberTime1;
		}
		if (m_bombCount % 4 == 0) {
			for (int ii = 0; ii < 3; ii++) {
				CPos p;
				p.x = cos(CFunc::ToRad(90 + 3 * m_bombCount + 120 * ii)) * m_bomberDist;
				p.y = sin(CFunc::ToRad(90 + 3 * m_bombCount + 120 * ii)) * m_bomberDist;
				p.x += CFunc::RandD(-10, 10);
				p.y += CFunc::RandD(-10, 10);

				CPos pp(CFunc::RandI(0, 30), CFunc::RandI(0, 30));
				double ang = 270 + CFunc::RandD(-30, 30);
				double speed = 0;
				CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_bomberPos + p, speed, ang, 0, 0.0, 0.0, 0, 20503);
				//double size = 0.8 - (double)ii * 0.1;
				double size = 0.5;
				eff->SetSize(size, +0.0);
				//eff->SetWaitTime(ii * 4);
				eff->SetBlend(255, +0.0);
				eff->SetBlendType(DX_BLENDMODE_NOBLEND);
				eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
				eff->SetRemoveCount(60);	//60frで削除

				CBattleScene::m_effectManager.Add(eff);
			}
		}

		for (int ii = 0; ii < 3; ii++) {
			if (m_bombCount % 1 == 0) {
				double posAngle = CFunc::RandD(0, 360);
				CPos p;
				p.x = cos(CFunc::ToRad(posAngle)) * m_bomberDist * 1.25;
				p.y = sin(CFunc::ToRad(posAngle)) * m_bomberDist * 1.25;

				double dirAngle = CFunc::ToDeg(CFunc::GetTwoPointAngle(m_bomberPos + p, m_bomberPos)) + 180.0;

				int imageIndex = CFunc::RandI(0, 3);
				double speed = 0.5;
				CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_bomberPos + p, speed, dirAngle, 0, 0.0, 15.0, 0, fireball_image4[imageIndex]);
				eff->SetAddAcce2(0.001);
				double size = 0.2;
				eff->SetSize(size, +0.0);
				eff->SetBlend(255, +0.0);
				eff->SetRemoveCount(30);	//60frで削除
				CBattleScene::m_effectManager.Add(eff);
			}
		}
	}

	if (m_bombCount >= bomberTime2) {
		m_scene->DamageAllEnemy(bomberTotalDamage / bomberFinishTime);

		if (m_bombCount % 1 == 0) {
			double dirAngle = CFunc::RandD(0, 360);

			int imageIndex = CFunc::RandI(0, 3);
			double speed = CFunc::RandD(7, 8);
			double acceSpeed = CFunc::RandD(0.1, 0.3);
			//
			CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_bomberPos, speed, dirAngle, 0, acceSpeed, 15.0, 0, 20503);
			double size = 1.0;
			eff->SetSize(size, +0.0);
			eff->SetBlend(255, -1.0, 0);
			eff->SetRemoveCount(60);	//60frで削除
			CBattleScene::m_effectManager.Add(eff);
		}

		if (m_bombCount % 1 == 0) {
			double dirAngle = CFunc::RandD(0, 360);

			int imageIndex = CFunc::RandI(0, 3);
			double speed = CFunc::RandD(2, 3);
			CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_bomberPos, speed, dirAngle, 0, 0.0, 15.0, 0, fireball_image4[3]);
			double size = 1.0;
			eff->SetSize(size, +0.0);
			eff->SetBlend(255, -10.0, 0);
			eff->SetRemoveCount(30);	//60frで削除
			CBattleScene::m_effectManager.Add(eff);
		}
	}
	if (m_bombCount >= bomberTime2 && m_bombCount <= bomberFinishTime - 30) {
		if (m_bombCount == bomberTime2) {
			CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_bomberPos, 0.0, 0.0, 0, 0.0, 0.0, 0, 998);
			eff->SetSize(0.0, +0.15);
			eff->SetBlend(255, -4.0, 0);
			eff->SetRemoveCount(60);	//60frで削除
			CBattleScene::m_effectManager.Add(eff);
		}
		if (m_bombCount % 2 == 0) {
			double dirAngle = CFunc::RandD(0, 360);

			int imageIndex = CFunc::RandI(0, 3);
			double speed = 0.0;
			CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_bomberPos, speed, dirAngle, 0, 0.0, 15.0, 0, 986);
			double size = 2.0;
			eff->SetSize(size, +0.0);
			eff->SetBlend(255, -10.0, 0);
			eff->SetRemoveCount(60);	//60frで削除
			CBattleScene::m_effectManager.Add(eff);
		}
	}

	m_scene->SetBulletRemoveTime(CBattleScene::BulletRemoveType::Nothing, 30);
	

	m_bombCount++;
	if (m_bombCount > bomberFinishTime) {
		m_bombCount = 0;
		m_bombOn = false;
	}
}

void CPlayer::Draw()
{
	
	if (m_mutekiCount > 0) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255 * m_barrierAlpha);
		CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, 2.4, m_barrierAngle, m_playerBarrier1->m_iamge);
		CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, 2.4, -m_barrierAngle, m_playerBarrier1->m_iamge);
		m_barrierAngle += 0.02;

		if (m_mutekiCount < mutekiTimeAlphaDown) {
			m_barrierAlpha -= (double)1.0 / mutekiTimeAlphaDown;
			if (m_barrierAlpha < 0) {
				m_barrierAlpha = 0.0;
			}
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, 1.0, 0.0, m_playerImage->m_iamge);
	CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, 0.75, m_hitMakerRotateAngle, m_hitMakerImage->m_iamge);
	CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, 0.75, -m_hitMakerRotateAngle, m_hitMakerImage->m_iamge);
	m_hitMakerRotateAngle += CFunc::ToRad(1.0);

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


void CPlayer::MainShot()
{
	double speedMain = 35.0;
	{
		double angle = 270.0;
		CPlayerBullet* b1 = new CPlayerBullet(30, m_pos + CPos(0, -30), speedMain, angle, 0, 0, 0, 0, 997/*"playerBullet"*/);
		m_playerBullet->Add(b1);
	}
}

void CPlayer::Shot()
{
	double speed = 20.0;

	int bulletImageA = 990;

	CPos posAng1;
	posAng1.x = cos(m_posBitAngleL[0] / 57.27) * 40;
	posAng1.y = 0 * sin(m_posBitAngleL[0] / 57.27) * 20;
	{
		CPlayerBullet* b1 = new CPlayerBullet(10, CPos(m_pos.x - 100 + posAng1.x, m_pos.y + 100 + posAng1.y), speed, 270.0, 0, 0, 0, 0, bulletImageA);
		m_playerBullet->Add(b1);
	}

	CPos posAng2;
	posAng2.x = cos(m_posBitAngleL[1] / 57.27) * 40;
	posAng2.y = 0 * sin(m_posBitAngleL[1] / 57.27) * 20;
	{
		CPlayerBullet* b2 = new CPlayerBullet(10, CPos(m_pos.x - 100 + posAng2.x, m_pos.y + 100 + posAng2.y), speed, 270.0, 0, 0, 0, 0, bulletImageA);
		m_playerBullet->Add(b2);
	}

	CPos posAng3;
	posAng3.x = cos(m_posBitAngleR[0] / 57.27) * 40;
	posAng3.y = 0 * sin(m_posBitAngleR[0] / 57.27) * 20;
	{
		CPlayerBullet* b3 = new CPlayerBullet(10, CPos(m_pos.x + 100 + posAng3.x, m_pos.y + 100 + posAng3.y), speed, 270.0, 0, 0, 0, 0, bulletImageA/*"playerBullet"*/);
		m_playerBullet->Add(b3);
	}

	CPos posAng4;
	posAng4.x = cos(m_posBitAngleR[1] / 57.27) * 40;
	posAng4.y = 0 * sin(m_posBitAngleR[1] / 57.27) * 20;
	{
		CPlayerBullet* b4 = new CPlayerBullet(10, CPos(m_pos.x + 100 + posAng4.x, m_pos.y + 100 + posAng4.y), speed, 270.0, 0, 0, 0, 0, bulletImageA/*"playerBullet"*/);
		m_playerBullet->Add(b4);
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
	m_hitSize = 24.0;
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
	CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_pos - CBattleScene::GetBattleScene()->GetBackGroundscrollSmall(), 0.1, ang, 0, 0, 0, 0, 20600);
	eff->SetSize(0.35, +0.0);
	eff->SetBlend(64, -0, 0);
	eff->SetBlendType(DX_BLENDMODE_ADD);
	eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
	eff->SetRemoveCount(60);	//60frで削除

	CBattleScene::m_effectManager.Add(eff);
}

//メイン描画
void CPlayerBullet::Draw()
{
	SetDrawBlendMode(m_blendType, 255);

	//アニメフラグがONなら
	if (m_imageInfo.m_animeFlg == true) {
		if (m_count % m_imageInfo.m_animeSpeed == 0) {
			m_imageInfo.m_animePos++;
			if (m_imageInfo.m_animePos >= m_imageInfo.m_animeNum) {
				m_imageInfo.m_animePos = 0;
			}
		}
	}
	//回転フラグがONなら
	if (m_imageInfo.m_rotationFlg == true) {
		m_imageInfo.m_rotationAngle += m_imageInfo.m_rotationSpeed;
	}

	SetDrawBlendMode(m_blendType, 255);
	//描画
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0f, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, m_image[m_imageInfo.m_animePos]);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}