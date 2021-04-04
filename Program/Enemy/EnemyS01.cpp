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

	AddLauncher(new CLauncherS01(m_rank, m_pos, CPos(21, 31), ShotTypeS01::Slow));
	AddLauncher(new CLauncherS01(m_rank, m_pos, CPos(-21, 31), ShotTypeS01::Slow));
	AddLauncher(new CLauncherS01(m_rank, m_pos, CPos(0, 14), ShotTypeS01::Rapid));
}
CEnemyS01::~CEnemyS01() {
}
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



CLauncherS01::CLauncherS01(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_type = type;
};
CLauncherS01::~CLauncherS01() {

}
void CLauncherS01::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	int span;
	double speed;

	if (m_type == ShotTypeS01::Slow) {
		span = (int)(45.0 - 4.0 * RankRatio());
		speed = 2.0 + 2.0 * RankRatio();
		if (m_rank <= 49) {

		}
		else {
			if (m_count > span) {
				CBaseBullet* b = new CBaseBullet(EDirType::Player, newEnemyPos + nowRelativePos, speed, 0.0, 0, 0, 0, 0, 1);
				CBaseLauncher::m_bulletManager->Add(b);
				m_count = 0;
				return;
			}
		}
		
	}
	else {

		speed = 6.0 + 2.0 * RankRatio();
		span = (int)(30.0 - 8.0 * RankRatio());

		int loop = 1;
		if (m_rank <= 29)loop = 1;
		else if (m_rank <= 69)loop = 2;
		else if (m_rank <= 99)loop = 3;
		else loop = 4;

		if (m_count > span) {
			for (int ii = 0; ii < loop; ii++) {
				CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, SpeedRange(speed, ii, loop), 0.0, 0, 0, 0, 0, 1);
				CBaseLauncher::m_bulletManager->Add(b);
			}
			m_count = 0;
			return;
		}
	}
	

	m_count++;
}



//----------------------------------------------------------------------------------------------------------
// CEnemyS02
//----------------------------------------------------------------------------------------------------------
CEnemyS02::CEnemyS02(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS2");

	std::vector<Collision> collisions = { Collision(CPos(0,-18), 39.0) };
	Init(200, Small, collisions);

	AddLauncher(new CLauncherS02(m_rank, m_pos, CPos(0, 26), ShotTypeS01::Rapid));
}
CEnemyS02::~CEnemyS02() {
}
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



CLauncherS02::CLauncherS02(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS02::~CLauncherS02() {

}
void CLauncherS02::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	double speed;

	// 3秒に1秒間3way弾を撃つ 
	speed = 4.0;
	int start_time = (int)(60.0);
	int end_time = (int)(start_time + 60*0.2);
	int span = 4;
	if (m_count >= start_time && m_count <= end_time) {
		if (m_count % span == 0) {
			for (int ii = -1; ii <= 1; ii++) {
				double angle = ii * 15;
				CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 1);
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
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

	std::vector<Collision> collisions = { Collision(CPos(0,-18), 39.0) };
	Init(200, Small, collisions);

	AddLauncher(new CLauncherS03a(m_rank, m_pos, CPos(0, 7), ShotTypeS01::Rapid));

	AddLauncher(new CLauncherS03b(m_rank, m_pos, CPos(22, 30), ShotTypeS01::Rapid));
	AddLauncher(new CLauncherS03b(m_rank, m_pos, CPos(-22, 30), ShotTypeS01::Rapid));
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



CLauncherS03a::CLauncherS03a(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS03a::~CLauncherS03a() {}
void CLauncherS03a::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	int span = 30;
	if (m_count % span == 0) {
		double speed = CFunc::RandD(2.8, 4.5);
		double angle = CFunc::RandD(-30,30);
		CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 1);
		CBaseLauncher::m_bulletManager->Add(b);
	}

	m_count++;
}

CLauncherS03b::CLauncherS03b(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS03b::~CLauncherS03b() {}
void CLauncherS03b::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	// 3秒に1秒間3way弾を撃つ 
	double speed = 4.0;
	int start_time = (int)(60.0);
	int end_time = (int)(start_time + 60);
	int span = 20;
	if (m_count >= start_time && m_count <= end_time) {
		if (m_count % span == 0) {
			for (int ii = -1; ii <= 1; ii++) {

				double angle = this->m_parent->GetToPlayerAngle();
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 2);
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
		if (m_count > end_time) {
			m_count = 0;
			return;
		}
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

	AddLauncher(new CLauncherS05(m_rank, m_pos, CPos(0, 26), ShotTypeS01::Rapid));
}
CEnemyS05::~CEnemyS05() {
}
void CEnemyS05::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

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



CLauncherS05::CLauncherS05(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS05::~CLauncherS05() {

}
void CLauncherS05::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	double speed;

	// 3秒に1秒間3way弾を撃つ 
	speed = 3.65;
	int start_time = (int)(120);
	int end_time = (int)(start_time + 60);
	int span = 12;
	if (m_count >= start_time && m_count <= end_time) {
		if (m_count % span == 0) {
			double angle = this->m_parent->GetToPlayerAngle();
			double nearAngle = CFunc::GetNearAngle(angle, 30);
			CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, nearAngle, 0, 0, 0, 0, 2);
			CBaseLauncher::m_bulletManager->Add(b);
		}
		if (m_count >= end_time) {
			m_count = 0;
			return;
		}
	}

	m_count++;
}


