#include "EnemyL.h"
#include "DXFunc.h"
#include "BattleScene.h"
#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"
#include "CustomBullet.h"

//----------------------------------------------------------------------------------------------------------
// CEnemyL01
//----------------------------------------------------------------------------------------------------------
CEnemyL01::CEnemyL01(const CPos& pos) : CBaseEnemy(pos){
	SetDrawSize(2.0); // 画像が小さかったので二倍
	m_image = (CImage*)CGame::GetResource("enemyL1");
	m_shotTiming = true;
	m_drawSizeRatio = 2.0;
	std::vector<Collision> collisions = {	
		Collision(CPos(0,-26), 40.0),
		Collision(CPos(0,28), 24.0),
		Collision(CPos(59,-21), 30.0) ,
		Collision(CPos(-59,-21), 30.0) };
	Init(1200, Large, collisions);

	AddLauncher(new CLauncherL01a(m_rank, m_pos, CPos(0, 52)));
	AddLauncher(new CLauncherL01b(m_rank, m_pos, CPos(47, 20), LauncherPos::LauncherRight));
	AddLauncher(new CLauncherL01b(m_rank, m_pos, CPos(-47, 20), LauncherPos::LauncherLeft));
	m_shotAngleL = 90.0;
	m_shotAngleR = 90.0;
}
CEnemyL01::~CEnemyL01() {
}
void CEnemyL01::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyL01::GetFinalDirectionRad()
{
	return CFunc::ToRad(90.0);
}

CLauncherL01a::CLauncherL01a(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherL01a::~CLauncherL01a() {}
// 真下攻撃
bool CLauncherL01a::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 20;
	int endTime = startTime + 60;
	int resetTime = endTime + 120;
	int span = 5 * RankSpan();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {

			for (int ii = 0; ii < 5; ii++) {
				double speed = CFunc::RandD(7.2, 8.9) * RankSpeed();

				CPos p;
				p.x = cos(CFunc::ToRad(CFunc::RandD(0, 360))) * 5;
				p.y = sin(CFunc::ToRad(CFunc::RandD(0, 360))) * 5;
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos + p, speed, 90.0, 0, 0, 0, 0, 1);
				b->SetShotEnemyId(m_parent->GetEnemyId());
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}


CLauncherL01b::CLauncherL01b(int rank, const CPos& enemyPos, const CPos& relativePos, LauncherPos lr) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_lr = lr;
};

CLauncherL01b::~CLauncherL01b() {}
// 回転砲台
bool CLauncherL01b::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 40;
	int endTime = startTime + 60;
	int resetTime = endTime + 20;
	int span = 7 * RankSpan();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			double angle;
			if (m_lr == LauncherPos::LauncherLeft) {
				angle = ((CEnemyL01*)m_parent)->m_shotAngleL;
				((CEnemyL01*)m_parent)->m_shotAngleL += -3;
			}
			else {
				angle = ((CEnemyL01*)m_parent)->m_shotAngleR;
				((CEnemyL01*)m_parent)->m_shotAngleR -= -3;
			}
			double speed = 6.1 * RankSpeed();

			// // 25以下：1　 50以下：2　 75以下：3　100以下4
			int wayNum = 1 + RankRapidA();
			for (int ii = 0; ii < wayNum; ii++) {
				double shotAngle = angle + (360.0 / wayNum) * ii;
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos , 0.0, shotAngle, 0, 0.1, speed, 0, 82);
				b->SetShotEnemyId(m_parent->GetEnemyId());
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}




//----------------------------------------------------------------------------------------------------------
// CEnemyL02
//----------------------------------------------------------------------------------------------------------
CEnemyL02::CEnemyL02(const CPos& pos) : CBaseEnemy(pos) {
	SetDrawSize(2.0); // 画像が小さかったので二倍
	m_image = (CImage*)CGame::GetResource("enemyL2");
	m_shotTiming = true;
	m_drawSizeRatio = 2.0;
	std::vector<Collision> collisions = {
		Collision(CPos(0,-29), 75.0) };
	Init(1200, Large, collisions);

	AddLauncher(new CLauncherL02a(m_rank, m_pos, CPos(0, -60)));
	AddLauncher(new CLauncherL02b(m_rank, m_pos, CPos(83, 40), LauncherPos::LauncherRight));
	AddLauncher(new CLauncherL02b(m_rank, m_pos, CPos(-83, 40), LauncherPos::LauncherLeft));
}
CEnemyL02::~CEnemyL02() {
}
void CEnemyL02::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyL02::GetFinalDirectionRad()
{
	return CFunc::ToRad(90.0);
}

CLauncherL02a::CLauncherL02a(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_angle = CFunc::RandD(0, 360);
	m_angleDir = CFunc::RandB();
};

CLauncherL02a::~CLauncherL02a() {}
// 回転直線列弾
bool CLauncherL02a::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int span = 7 * RankSpan();

	if (m_count % span == 0) {
		if(m_angleDir)m_angle += 20.0;
		else m_angle -= 20.0;
		for (int ii = 0; ii < 5; ii++) {
			double speed = 8.6 * RankSpeed() + (double)0.4 * ii;
			{
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, m_angle, 0, 0, 0, 0, 82);
				CBaseLauncher::m_bulletManager->Add(b);
			}
			{
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, 180+m_angle, 0, 0, 0, 0, 82);
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
	}
	
	m_count++;
	return true;
}


CLauncherL02b::CLauncherL02b(int rank, const CPos& enemyPos, const CPos& relativePos, LauncherPos lr) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_lr = lr;
};

CLauncherL02b::~CLauncherL02b() {}
// 時期狙い
bool CLauncherL02b::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 40;
	int endTime = startTime + 40 * Rank10To20();
	int resetTime = endTime + 20;
	int span = 6 * RankSpan();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			double speed = 11.1 * RankSpeed();
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, 0, 0, 0, 0, 0, 91);
			b->SetShotEnemyId(m_parent->GetEnemyId());
			CBaseLauncher::m_bulletManager->Add(b);
		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}



//----------------------------------------------------------------------------------------------------------
// CEnemyL03
//----------------------------------------------------------------------------------------------------------
CEnemyL03::CEnemyL03(const CPos& pos) : CBaseEnemy(pos) {
	SetDrawSize(2.0); // 画像が小さかったので二倍
	m_image = (CImage*)CGame::GetResource("enemyL3");
	m_shotTiming = true;
	m_drawSizeRatio = 2.0;
	std::vector<Collision> collisions = {
		Collision(CPos(0,7), 59.0),
		Collision(CPos(60,-15), 39.0), 
		Collision(CPos(-60,-15), 39.0)};
	Init(1200, Large, collisions);

	AddLauncher(new CLauncherL03a(m_rank, m_pos, CPos(0, -37)));
	AddLauncher(new CLauncherL03b(m_rank, m_pos, CPos(82, 63)));
	AddLauncher(new CLauncherL03b(m_rank, m_pos, CPos(-82, 63)));

	m_shotAngleL = 90.0;
	m_shotAngleR = 90.0;
}
CEnemyL03::~CEnemyL03() {
}
void CEnemyL03::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyL03::GetFinalDirectionRad()
{
	return CFunc::ToRad(90.0);
}

CLauncherL03a::CLauncherL03a(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_speed = 0;
};

CLauncherL03a::~CLauncherL03a() {}
// エネルギーは波動弾
bool CLauncherL03a::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 50;
	int endTime = startTime + 40;
	int resetTime = endTime + 20;
	int span = 6 * RankSpan();
	if (m_count == startTime) {
		m_speed = 7.0;
		m_shotAngle = CFunc::ToDeg(CFunc::GetTwoPointAngle(m_target, m_parent->m_pos));
		m_deltaSpeed = 0.0;
	}
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			m_deltaSpeed += 0.15;
			m_speed += m_deltaSpeed;
			double range = 2 * Rank10To30();
			for (int jj = -1; jj <= 1; jj++) {
				int num = 5 + RankRapidA();
				for (int ii = 0; ii < num; ii++) {
					double angle = m_shotAngle + jj * 30.0 + CFunc::RandD(-range, range);
					double speed = m_speed + CFunc::RandD(0, 100) / 100.0;
					CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 32);
					CBaseLauncher::m_bulletManager->Add(b);

				}
			}
		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}


CLauncherL03b::CLauncherL03b(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherL03b::~CLauncherL03b() {}
// 時期狙い
bool CLauncherL03b::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 40;
	int endTime = startTime + 40 * Rank10To20();
	int resetTime = endTime + 20;
	int span = 20 * RankSpan();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {

			for (int ii = -2; ii <= 2; ii++) {
				double angle = ii * 45;
				double maxSpeed = 11.1 * RankSpeed();
				CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, 2.0, angle, 0, 0.155, maxSpeed, 0, 51);
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}


//----------------------------------------------------------------------------------------------------------
// CEnemyL04
//----------------------------------------------------------------------------------------------------------
CEnemyL04::CEnemyL04(const CPos& pos) : CBaseEnemy(pos) {
	SetDrawSize(2.0); // 画像が小さかったので二倍
	m_image = (CImage*)CGame::GetResource("enemyL4");
	m_shotTiming = true;
	m_drawSizeRatio = 1.5;
	std::vector<Collision> collisions = {
		Collision(CPos(0,-41), 64.0),
		Collision(CPos(0,65), 40.0),
	Collision(CPos(125,-45), 40.0), 
	Collision(CPos(-125,-45), 40.0)};
	Init(1200, Large, collisions);

	AddLauncher(new CLauncherL04a(m_rank, m_pos, CPos(0, -50)));
	AddLauncher(new CLauncherL04b(m_rank, m_pos, CPos(83, 36)));
	AddLauncher(new CLauncherL04b(m_rank, m_pos, CPos(-83, 36)));

	m_shotAngleL = 90.0;
	m_shotAngleR = 90.0;
}
CEnemyL04::~CEnemyL04() {
}
void CEnemyL04::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyL04::GetFinalDirectionRad()
{
	return CFunc::ToRad(90.0);
}

CLauncherL04a::CLauncherL04a(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_speed = 0;
};

CLauncherL04a::~CLauncherL04a() {}
// 上空展開雨
bool CLauncherL04a::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 50;
	int endTime = startTime + 40;
	int resetTime = endTime + 20;
	int span = 6 * RankSpan();
	if (m_count == startTime) {
		m_speed = 7.0;
		m_shotAngle = CFunc::ToDeg(CFunc::GetTwoPointAngle(m_target, m_parent->m_pos));
		m_deltaSpeed = 0.0;
	}
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {

			double angle = CFunc::RandD(180, 360);
			int changeTime = 60;
			int num = 3 * Rank10To40();
			for (int ii = 0; ii < num; ii++) {
				double baseSpeed = 8.0;
				double acce = baseSpeed / changeTime;
				CCustomBullet* bullet = new CCustomBullet(EDirType::Abs, m_enemyPos + nowRelativePos, baseSpeed, angle, 0, -acce, 0, 0, 1);
				
				double speed = (5.5 + (double)ii * 0.3) * RankSpeed();
				CCustomBullet* Trans = new CCustomBullet(EDirType::Player, CPos(0, 0), speed, 0, 0, 0, 0, 0, 91);
				bullet->SetTransBullet(Trans, changeTime);
				Trans->SetTransAngle(0); //EDirType::Player狙いの時だけ、変化後の角度を再設定してあげないといけない
				CBaseLauncher::m_bulletManager->Add(bullet);
			}

		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}


CLauncherL04b::CLauncherL04b(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherL04b::~CLauncherL04b() {}
// 時期狙い
bool CLauncherL04b::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 70;
	int endTime = startTime + 20 * Rank10To30();
	int resetTime = endTime + 50;
	int span = 5 * RankSpan();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			double angle = CFunc::RandD(-45,45);
			double maxSpeed = 5.1 * RankSpeed();
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, maxSpeed/2, angle, 0, 0.155, maxSpeed, 0, 32);
			CBaseLauncher::m_bulletManager->Add(b);
		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}
