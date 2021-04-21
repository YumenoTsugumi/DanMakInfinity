#include "EnemyS.h"
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

	double speed = 8.2 * RankSpeed();
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
	double speed = 7.35 * RankSpeed();
	int way = 3 + RankWay();

	int startTime = 0;
	int endTime = startTime + 40;
	int resetTime = endTime + 60;
	int span = 50 * RankSpan();
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
	int span = 30 * RankSpan();
	int loop =  1 + RankRapidA();
	double speed = 8.8 * RankSpeed();
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
// CEnemyS04
//----------------------------------------------------------------------------------------------------------
CEnemyS04::CEnemyS04(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS4");

	std::vector<Collision> collisions = { 
		Collision(CPos(14,2), 24.0) ,
		Collision(CPos(-14,2), 24.0) 
	};
	Init(200, Small, collisions);

	AddLauncher(launcher = new CLauncherS04(m_rank, m_pos, CPos(20, 41)));
	AddLauncher(new CLauncherS04(m_rank, m_pos, CPos(-20, 41)));
}
CEnemyS04::~CEnemyS04() {}
void CEnemyS04::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	if (m_behaviorComponent->GetBehaviorStatus() != BehaviorStatus::Shot) {
		angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
	} else if (launcher->m_shotAngleRock) {
		angle = CFunc::ToRad(launcher->m_shotAngle - 90.0);
	}
	else {
		angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
	}
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyS04::GetFinalDirectionRad()
{
	if (m_behaviorComponent->GetBehaviorStatus() != BehaviorStatus::Shot) {
		return CFunc::GetTwoPointAngle(m_target, m_pos);
	} else 
	if (launcher->m_shotAngleRock) {
		return CFunc::ToRad(launcher->m_shotAngle);
	}
	else {
		return CFunc::GetTwoPointAngle(m_target, m_pos);
	}

	//return CFunc::GetTwoPointAngle(m_target, m_pos);
}
CLauncherS04::CLauncherS04(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_shotAngleRock = false;
};
CLauncherS04::~CLauncherS04() {}
void CLauncherS04::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	// ナイトメア弾
	int initTime = 25;
	int startTime = 30;
	int endTime = startTime + 80;
	int resetTime = endTime + 160;
	int span = 7 * RankSpan();
	
	if (m_count == initTime) {
		m_shotAngleRock = true;
		m_shotAngle = this->m_parent->GetToPlayerAngle();
		m_shotSpeed = 4.0 * RankSpeed();
	}
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			m_shotSpeed += 0.4 * RankSpeed();
			CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, m_shotSpeed, m_shotAngle, 0, 0, 0, 0, 61);
			CBaseLauncher::m_bulletManager->Add(b);
		}
		if (m_count == endTime) {
			m_shotAngleRock = false;
		}
	}

	if (m_count >= resetTime) {
		m_count = 0;
		return;
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
CEnemyS05::~CEnemyS05() {}
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


// 角度制限弾
CLauncherS05::CLauncherS05(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS05::~CLauncherS05() {}
void CLauncherS05::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	double speed = 8.65 * RankSpeed();

	int startTime = 30;
	int endTime = startTime + 60;
	int resetTime = endTime + 90;
	int span = 24 * RankSpan();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			double angle = this->m_parent->GetToPlayerAngle();
			double nearAngle = CFunc::GetNearAngle(angle, 30);
			CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, nearAngle, 0, 0, 0, 0, 1);
			CBaseLauncher::m_bulletManager->Add(b);
		}
	}
	if (m_count >= resetTime) {
		m_count = 0;
		return;
	}
	m_count++;
}

//----------------------------------------------------------------------------------------------------------
// CEnemyS06
//----------------------------------------------------------------------------------------------------------
// 自機方面のばらまき
CEnemyS06::CEnemyS06(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS6");
	std::vector<Collision> collisions = { 
		Collision(CPos(0,-5), 30.0) ,
		Collision(CPos(18,12), 10.0),
		Collision(CPos(-18,12), 10.0)
	};
	Init(200, Small, collisions);

	AddLauncher(new CLauncherS06(m_rank, m_pos, CPos(0, 15)));
}
CEnemyS06::~CEnemyS06() {}
void CEnemyS06::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyS06::GetFinalDirectionRad()
{
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}
// 自機方面のばらまき
CLauncherS06::CLauncherS06(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS06::~CLauncherS06() {}
void CLauncherS06::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	int startTime = 20;
	int endTime = startTime + 120;
	int resetTime = endTime + 120;
	int span = 45 * RankSpan();
	int loop = 2 * RankBulletNum();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			for (int ii = 0; ii < loop;ii++) {
				double speed = CFunc::RandD(5.5, 7.5) * RankSpeed();
				double angle = CFunc::RandD(-45, 45);
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
// CEnemyS07
//----------------------------------------------------------------------------------------------------------
CEnemyS07::CEnemyS07(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS7");
	std::vector<Collision> collisions = {
		Collision(CPos(0,-5), 25.0) ,
		Collision(CPos(33,0), 9.0),
		Collision(CPos(45,-8), 5.0)
	};
	Init(200, Small, collisions);

	AddLauncher(new CLauncherS07(m_rank, m_pos, CPos(0, 29)));
}
CEnemyS07::~CEnemyS07() {}
void CEnemyS07::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyS07::GetFinalDirectionRad()
{
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

// 時期狙い狭い3way　いわゆるワイバーン
CLauncherS07::CLauncherS07(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherS07::~CLauncherS07() {}
void CLauncherS07::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	int startTime = 20;
	int endTime = startTime + 120;
	int resetTime = endTime + 120;
	int span = 45 * RankSpan();
	int loop = 2 * RankBulletNum();
	int way = 3 * RankBulletNum();
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			int st = -way / 2;
			int ed = way / 2;
			for (int ii = st; ii <= ed; ii++) {
				double speed = 7.4 * RankSpeed();
				double angle = ii*1.5;
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


