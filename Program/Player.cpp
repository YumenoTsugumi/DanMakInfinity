
#include "Player.h"
#include "BaseBullet.h"
#include "BattleScene.h"

constexpr double TiltAngle = 1.5;
constexpr double TiltAngleMax = 40.0;
constexpr int ChangeShotTime = 15;
CPlayer::CPlayer():
	m_pos(0,0),
	m_playerImage(nullptr),
	m_slowMove(0),
	m_bulletShotCount(0),
	m_bulletMainShotCount(0),
	m_hitSize(1),
	m_dieAnimetion(0)
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

	m_forwardAngle[0] = 0;
	m_forwardAngle[1] = 180.0;
	m_forwardAngle[2] = 0;
	m_forwardAngle[3] = 180.0;

	m_bombOn = false;
	m_bombCount = 0;

	m_barrierAngle = 0.0;
	m_barrierAlpha = 1.0;

	for (int ii = 0; ii < 4; ii++) {
		m_tracePos[ii] = m_pos;
	}
	for (int ii = 0; ii < TraceBitNum; ii++) {
		m_traceSubPos[ii] = m_pos;
	}
	m_tracePrePos = m_pos;
	m_tiltAngle = 0.0;

	m_shotChangeCount = 0;

	//SetShotType(Trace, Wide);
	SetShotType(Tilt, Forward);
}

void CPlayer::SetBulletManager(CBulletManager* playerBullet)
{
	m_playerBullet = playerBullet;
}


void CPlayer::Action(CInputAllStatus* input)
{
	m_dieAnimetion--;
	if (IsDieAnimetion()) {
		return;
	}

	//移動処理
	double move = m_rapidSpeed;
	int moveCo = 0;

	if (input->GetBtnOnOff(INPUT_DEF_UP) == true) {
		moveCo++;
	}
	if (input->GetBtnOnOff(INPUT_DEF_DOWN) == true) {
		moveCo++;
	}
	bool leftRight = false;
	if (input->GetBtnOnOff(INPUT_DEF_RIGHT) == true) {
		m_tiltAngle += TiltAngle;
		if (m_tiltAngle > TiltAngleMax)m_tiltAngle = TiltAngleMax;
		moveCo++;
		leftRight = true;
	}
	if (input->GetBtnOnOff(INPUT_DEF_LEFT) == true) {
		m_tiltAngle -= TiltAngle;
		if (m_tiltAngle < -TiltAngleMax)m_tiltAngle = -TiltAngleMax;
		moveCo++;
		leftRight = true;
	}
	if(!leftRight) {
		if (m_tiltAngle <= TiltAngleMax && m_tiltAngle > 0) {
			m_tiltAngle -= TiltAngle * 3;
			if (m_tiltAngle < 0)m_tiltAngle = 0;
		}
		if (m_tiltAngle >= -TiltAngleMax && m_tiltAngle < 0) {
			m_tiltAngle += TiltAngle * 3;
			if (m_tiltAngle > 0)m_tiltAngle = 0;
		}
	}
	bool isSlow = false;
	if (input->GetBtnOnOff(INPUT_DEF_SLOW) == true) {
		isSlow = true;
		if (m_shotChangeCount < ChangeShotTime) {
			m_shotChangeCount++;
		}
	}
	else {
		if (m_shotChangeCount > 0) {
			m_shotChangeCount--;
		}
	}

	if (moveCo >= 2) {
		move *= 1.0 / sqrt(2.0);
		m_slowMove = MOVE_SLOW;
	}
	else {
		m_slowMove = MOVE_HI;
	}
	if (isSlow) {
		move = m_slowSpeed;
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

	SubShotAction();
	if (input->GetBtnOnOff(INPUT_DEF_ENTER) == true) {
		if (m_bulletShotCount >= 3) {
			SubShotFire();
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
constexpr int bomberFinishTime = bomberTime2 + 60 * 1; // 終わる時間
constexpr int bomberTotalDamage = 600;

constexpr int startDist = 200;
constexpr int endDist = 50;
constexpr int mutekiTime = bomberFinishTime + 60 * 3;
constexpr int mutekiTimeAlphaDown = mutekiTime * 0.1;

void CPlayer::SetMuteki(int mutekiTime)
{
	m_mutekiCount = mutekiTime;
	m_barrierAlpha = 1.0;
}
void CPlayer::BomberSet() {
	int bomb = CBattleScene::GetHaveBomb();
	if (bomb <= 0) {
		return;
	}
	if (!m_bombOn) {
		m_bombOn = true;
		m_bombCount = 0;
		SetMuteki(mutekiTime);
		m_bomberDist = bomberStartDist;
		m_bomberPos = m_pos + CPos(0, -300);
		if (m_bomberPos.y <= 100) {
			m_bomberPos.y = 100;
		}
		CBattleScene::BombDecrement();

		double ratio = CBattleScene::GetRankRatio() * 0.8;
		if (ratio < 1.0)ratio = 1.0;
		CBattleScene::SetRankRatio(ratio);
		CBattleScene::AddRank(-5);
	}
}

void CPlayer::MutekiTime() {
	if (m_mutekiCount <= 0) {
		return;
	}
	m_mutekiCount--;
}
bool CPlayer::IsMuteki() {
	if (m_mutekiCount > 0) {
		return true;
	}
	return false;
}

void CPlayer::Die()
{
	m_dieAnimetion = 60;
	SetMuteki(60*3);

	m_scene->LifeDecrement();

	double ang = CFunc::RandF(0, 360);
	CBaseEffect* eff = new CBaseEffect(50, EDirType::Abs, m_pos - CBattleScene::GetBattleScene()->GetBackGroundscrollSmall(),
		0.1, ang, 0, 0, 0, 0, 20608);
	eff->SetSize(1.0, +0.0);
	eff->SetBlend(255, -2, 0);
	eff->SetBlendType(DX_BLENDMODE_ALPHA);
	eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
	eff->SetRemoveCount(60);	//60frで削除
	CBattleScene::m_effectManager.Add(eff);
}
bool CPlayer::IsDieAnimetion()
{
	if (m_dieAnimetion >= 0) {
		return true;
	}
	return false;
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
	if (IsDieAnimetion()) {
		return;
	}

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

	SubShotDraw();
}

void CPlayer::SetShotType(ShotType rapid, ShotType slow) {
	m_rapidShotType = rapid;
	m_slowShotType = slow;
	if (m_rapidShotType == ShotType::Forward)m_rapidSubShotGetFunc = GetSubShotForward;
	if (m_rapidShotType == ShotType::Wide)m_rapidSubShotGetFunc = GetSubShotWide;
	if (m_rapidShotType == ShotType::Trace)m_rapidSubShotGetFunc = GetSubShotTrace;
	if (m_rapidShotType == ShotType::Tilt)m_rapidSubShotGetFunc = GetSubShotTilt;
	if (m_slowShotType == ShotType::Forward)m_slowSubShotGetFunc = GetSubShotForward;
	if (m_slowShotType == ShotType::Wide)m_slowSubShotGetFunc = GetSubShotWide;
	if (m_slowShotType == ShotType::Trace)m_slowSubShotGetFunc = GetSubShotTrace;
	if (m_slowShotType == ShotType::Tilt)m_slowSubShotGetFunc = GetSubShotTilt;
}
void CPlayer::SetSpeed(int rapid, int slow)
{
	// 1-9    1.5-13.5
	m_rapidSpeed = ((double)rapid) * 1.5;
	m_slowSpeed = ((double)slow) * 1.5;
}

void CPlayer::SubShotAction(){
	SubShot_Forward_Action();
	SubShot_Wide_Action();
	SubShot_Trace_Action();
	SubShot_Tilt_Action();

	double ratio = (double)m_shotChangeCount / ChangeShotTime;

	CPos rapidPos[4];
	double rapidAngle[4];
	CPos slowPos[4];
	double slowAngle[4];

	m_rapidSubShotGetFunc(this, rapidPos, rapidAngle);
	m_slowSubShotGetFunc(this, slowPos, slowAngle);

	for (int ii = 0; ii < 4; ii++) {
		m_subShotPos[ii].x = rapidPos[ii].x + (slowPos[ii].x - rapidPos[ii].x) * ratio;
		m_subShotPos[ii].y = rapidPos[ii].y + (slowPos[ii].y - rapidPos[ii].y) * ratio;
		m_subShotAngle[ii] = rapidAngle[ii] + (slowAngle[ii] - rapidAngle[ii]) * ratio;
	}
}

void CPlayer::MainShot(){
	double speedMain = 35.0;
	double angle = 270.0;
	CPlayerBullet* b1 = new CPlayerBullet(30, m_pos + CPos(0, -30), speedMain, angle, 0, 0, 0, 0, 997/*"playerBullet"*/);
	m_playerBullet->Add(b1);
}

void CPlayer::SubShotDraw(){
	CDxFunc::MyDrawRotaGraph(m_subShotPos[0], 0.5, CFunc::ToRad(m_subShotAngle[0]), m_bitRImage->m_iamge);
	CDxFunc::MyDrawRotaGraph(m_subShotPos[1], 0.5, CFunc::ToRad(m_subShotAngle[1]), m_bitRImage->m_iamge);
	CDxFunc::MyDrawRotaGraph(m_subShotPos[2], 0.5, CFunc::ToRad(m_subShotAngle[2]), m_bitRImage->m_iamge);
	CDxFunc::MyDrawRotaGraph(m_subShotPos[3], 0.5, CFunc::ToRad(m_subShotAngle[3]), m_bitRImage->m_iamge);
}
void CPlayer::SubShotFire() {
	int bulletImageA = 990;
	m_playerBullet->Add(new CPlayerBullet(10, m_subShotPos[0], 20.0, 270.0 + m_subShotAngle[0], 0, 0, 0, 0, bulletImageA));
	m_playerBullet->Add(new CPlayerBullet(10, m_subShotPos[1], 20.0, 270.0 + m_subShotAngle[1], 0, 0, 0, 0, bulletImageA));
	m_playerBullet->Add(new CPlayerBullet(10, m_subShotPos[2], 20.0, 270.0 + m_subShotAngle[2], 0, 0, 0, 0, bulletImageA));
	m_playerBullet->Add(new CPlayerBullet(10, m_subShotPos[3], 20.0, 270.0 + m_subShotAngle[3], 0, 0, 0, 0, bulletImageA));
}


void CPlayer::SubShot_Forward_Action(){
	m_forwardAngle[0] += 3.0;
	m_forwardAngle[1] += 3.0;
	m_forwardAngle[2] += 3.0;
	m_forwardAngle[3] += 3.0;

	constexpr double distX = 40;
	constexpr double distY = 20;
	m_forwardPos[0].x = m_pos.x - 100 + cos(CFunc::ToRad(m_forwardAngle[0])) * distX;
	m_forwardPos[0].y = m_pos.y + 100;
	m_forwardPos[1].x = m_pos.x - 100 + cos(CFunc::ToRad(m_forwardAngle[1])) * distX;
	m_forwardPos[1].y = m_pos.y + 100;
	m_forwardPos[2].x = m_pos.x + 100 + cos(CFunc::ToRad(m_forwardAngle[2])) * distX;
	m_forwardPos[2].y = m_pos.y + 100;
	m_forwardPos[3].x = m_pos.x + 100 + cos(CFunc::ToRad(m_forwardAngle[3])) * distX;
	m_forwardPos[3].y = m_pos.y + 100;

	m_forwardAngles[0] = 0;
	m_forwardAngles[1] = 0;
	m_forwardAngles[2] = 0;
	m_forwardAngles[3] = 0;
}
void CPlayer::GetSubShotForward(CPlayer* player, CPos forwardPos[4], double forwardAngle[4]){
	memcpy(forwardPos, player->m_forwardPos, sizeof(CPos[4]));
	memcpy(forwardAngle, player->m_forwardAngles, sizeof(double[4]));
}

void CPlayer::SubShot_Wide_Action(){
	m_widePos[0].x = m_pos.x - 60;
	m_widePos[0].y = m_pos.y + 30;
	m_widePos[1].x = m_pos.x - 100;
	m_widePos[1].y = m_pos.y + 100;

	m_widePos[2].x = m_pos.x + 60;
	m_widePos[2].y = m_pos.y + 30;
	m_widePos[3].x = m_pos.x + 100;
	m_widePos[3].y = m_pos.y + 100;

	m_wideAngle[0] = -12 - 4;
	m_wideAngle[1] = -25 - 4;
	m_wideAngle[2] = +12 - 4;
	m_wideAngle[3] = +25 - 4;
}
void CPlayer::GetSubShotWide(CPlayer* player, CPos widePos[4], double wideAngle[4]) {
	memcpy(widePos, player->m_widePos, sizeof(CPos[4]));
	memcpy(wideAngle, player->m_wideAngle, sizeof(double[4]));
}

void CPlayer::SubShot_Trace_Action() {
	if (m_tracePrePos.x != m_pos.x || m_tracePrePos.y != m_pos.y) {
		for (int ii = TraceBitNum - 1; ii >= 1; ii--) {
			m_traceSubPos[ii] = m_traceSubPos[ii - 1];
		}
		m_traceSubPos[0] = m_pos;
	}
	m_tracePrePos = m_pos;

	m_tracePos[0] = m_traceSubPos[(int)((double)TraceBitNum * 0.25)];
	m_tracePos[1] = m_traceSubPos[(int)((double)TraceBitNum * 0.50)];
	m_tracePos[2] = m_traceSubPos[(int)((double)TraceBitNum * 0.75)];
	m_tracePos[3] = m_traceSubPos[TraceBitNum-1];

	m_traceAngle[0] = 0;
	m_traceAngle[1] = 0;
	m_traceAngle[2] = 0;
	m_traceAngle[3] = 0;
}
void CPlayer::GetSubShotTrace(CPlayer* player, CPos tracePos[4], double traceAngle[4]) {
	memcpy(tracePos, player->m_tracePos, sizeof(CPos[4]));
	memcpy(traceAngle, player->m_traceAngle, sizeof(double[4]));
}

void CPlayer::SubShot_Tilt_Action() {
	m_tiltPos[0].x = m_pos.x - 60;
	m_tiltPos[0].y = m_pos.y + 100;
	m_tiltPos[1].x = m_pos.x - 100;
	m_tiltPos[1].y = m_pos.y + 30;

	m_tiltPos[2].x = m_pos.x + 60;
	m_tiltPos[2].y = m_pos.y + 100;
	m_tiltPos[3].x = m_pos.x + 100;
	m_tiltPos[3].y = m_pos.y + 30;

	m_tiltAngles[0] = m_tiltAngle;
	m_tiltAngles[1] = m_tiltAngle;
	m_tiltAngles[2] = m_tiltAngle;
	m_tiltAngles[3] = m_tiltAngle;
}
void CPlayer::GetSubShotTilt(CPlayer* player, CPos tiltPos[4], double tiltAngle[4]) {
	memcpy(tiltPos, player->m_tiltPos, sizeof(CPos[4]));
	memcpy(tiltAngle, player->m_tiltAngles, sizeof(double[4]));
}

//---------------------------------------------------------------------------------------------------------------
// プレイヤー専用の弾
//---------------------------------------------------------------------------------------------------------------
//コンストラクタ	弾作成
CPlayerBullet::CPlayerBullet(int damage, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
	CBaseBullet(EDirType::Abs, P, speed, angle, corner, acce, maxSpeed, nearAngle, image),
	m_damage(damage)
{
	m_hitSize = 48.0;
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

