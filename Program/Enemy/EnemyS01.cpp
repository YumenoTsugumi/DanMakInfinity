#include "EnemyS01.h"
#include "DXFunc.h"

#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"

//----------------------------------------------------------------------------------------------------------
// CEnemyS01
//----------------------------------------------------------------------------------------------------------
CEnemyS01::CEnemyS01(const CPos& pos) : CBaseEnemy(pos){
	m_image = (CImage*)CGame::GetResource("enemyS1");

	std::vector<Collision> collisions = { Collision(CPos(0,-18), 39.0) };
	Init(200, Small, collisions);

	AddLauncher(new CLauncherS01(m_rank, m_pos, CPos(0, 14)));
	if (m_rank >= 50) {
		AddLauncher(new CLauncherS01_50(m_rank, m_pos, CPos(21, 31)));
		AddLauncher(new CLauncherS01_50(m_rank, m_pos, CPos(-21, 31)));
	}
}
CEnemyS01::~CEnemyS01() {}
void CEnemyS01::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyS01::GetFinalDirectionRad()
{
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}


CLauncherS01::CLauncherS01(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS01::~CLauncherS01() {}
void CLauncherS01::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	double speed = 6.0 * RankSpeed();
	int span = (int)(30.0 * RankSpan());
	int loop = RankRapidA();

	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			double speedTmp = SpeedRange(speed, ii, loop);
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speedTmp, 0.0, 0, 0, 0, 0, 1);
			CBaseLauncher::m_bulletManager->Add(b);
		}
		m_count = 0;
		return;
	}
	m_count++;
}

CLauncherS01_50::CLauncherS01_50(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS01_50::~CLauncherS01_50() {}
void CLauncherS01_50::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	double speed = 2.0 * RankSpeed();
	int span = (int)(60.0 * RankSpan());
	int loop = RankRapidA();

	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			double speedTmp = SpeedRange(speed, ii, loop);
			double angle = CFunc::RandD(-4, 4);
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speedTmp, angle, 0, 0, 0, 0, 2);
			CBaseLauncher::m_bulletManager->Add(b);
		}
		m_count = 0;
		return;
	}
	m_count++;
}




//----------------------------------------------------------------------------------------------------------
// CEnemyS02
//----------------------------------------------------------------------------------------------------------
CEnemyS02::CEnemyS02(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS2");

	std::vector<Collision> collisions = { Collision(CPos(0,0), 28.0) };
	Init(200, Small, collisions);

	AddLauncher(new CLauncherS02(m_rank, m_pos, CPos(0, 26)));
}
CEnemyS02::~CEnemyS02() {}
void CEnemyS02::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyS02::GetFinalDirectionRad()
{
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

CLauncherS02::CLauncherS02(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS02::~CLauncherS02() {

}
void CLauncherS02::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);
	// 3 + n way弾を撃つ 
	double speed = 4.0 * RankSpeed();
	int way = 3 + RankWay();

	int startTime = 0;
	int endTime = startTime + 40;
	int resetTime = endTime + 60;
	int span = 19 * RankSpan();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			int st = 0 - way / 2;
			int ed = 0 + way / 2;
			for (int ii = st; ii <= ed; ii++) {
				double angle = ii * 15;
				CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 1);
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return;
	}
	m_count++;
}


//----------------------------------------------------------------------------------------------------------
// CEnemyS03
//----------------------------------------------------------------------------------------------------------
CEnemyS03::CEnemyS03(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS3");

	std::vector<Collision> collisions = { 
		Collision(CPos(0,-12), 18.0) ,
		Collision(CPos(+24,5), 16.0) ,
		Collision(CPos(-24,5), 16.0) };
	Init(200, Small, collisions);

	AddLauncher(new CLauncherS03(m_rank, m_pos, CPos(22, 30)));
	AddLauncher(new CLauncherS03(m_rank, m_pos, CPos(-22, 30)));
	if (m_rank >= 50) {
		AddLauncher(new CLauncherS03_50(m_rank, m_pos, CPos(0, 7)));
	}
}
CEnemyS03::~CEnemyS03() {
}
void CEnemyS03::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyS03::GetFinalDirectionRad()
{
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

CLauncherS03::CLauncherS03(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS03::~CLauncherS03() {}
void CLauncherS03::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	// 自機への方向へ
	int startTime = 0;
	int endTime = startTime + 60;
	int resetTime = endTime + 60;
	int span = 15 * RankSpan();
	int loop =  1 + RankRapidA();
	double speed = 3.8 * RankSpeed();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			for (int ii = 0; ii < loop; ii++) {
				double speedTmp = SpeedRange(speed, ii, loop);
				double angle = this->m_parent->GetToPlayerAngle();
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speedTmp, angle, 0, 0, 0, 0, 2);
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
	}
	if (m_count > resetTime) {
		m_count = 0;
		return;
	}
	m_count++;
}

CLauncherS03_50::CLauncherS03_50(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS03_50::~CLauncherS03_50() {}
void CLauncherS03_50::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	int span = 30 * RankSpan();
	if (m_count % span == 0) {
		double speed = CFunc::RandD(2.8, 4.5) * RankSpeed();
		double angle = CFunc::RandD(-30, 30);
		CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 2);
		CBaseLauncher::m_bulletManager->Add(b);
	}

	m_count++;
}





//----------------------------------------------------------------------------------------------------------
// CEnemyS05
//----------------------------------------------------------------------------------------------------------
CEnemyS05::CEnemyS05(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS5");
	m_drawAngle = 0;
	std::vector<Collision> collisions = { Collision(CPos(0,-18), 39.0) };
	Init(200, Small, collisions);

	AddLauncher(new CLauncherS05(m_rank, m_pos, CPos(0, 26)));
}
CEnemyS05::~CEnemyS05() {
}
void CEnemyS05::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	m_drawAngle += CFunc::ToRad(5.0);
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, m_drawAngle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyS05::GetFinalDirectionRad()
{
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}



CLauncherS05::CLauncherS05(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS05::~CLauncherS05() {

}
void CLauncherS05::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	double speed = 3.65 * RankSpeed();

	int start_time = 120;
	int end_time = start_time + 60;
	int span = 12 * RankSpan();
	if (m_count >= start_time && m_count <= end_time) {
		if (m_count % span == 0) {
			double angle = this->m_parent->GetToPlayerAngle();
			double nearAngle = CFunc::GetNearAngle(angle, 30);
			CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, nearAngle, 0, 0, 0, 0, 1);
			CBaseLauncher::m_bulletManager->Add(b);
		}
		if (m_count >= end_time) {
			m_count = 0;
			return;
		}
	}

	m_count++;
}


