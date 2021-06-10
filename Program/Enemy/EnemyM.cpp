#include "EnemyM.h"
#include "DXFunc.h"
#include "BattleScene.h"
#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"



//----------------------------------------------------------------------------------------------------------
// CEnemyM01
//----------------------------------------------------------------------------------------------------------
CEnemyM01::CEnemyM01(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM1");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(0,9), 24.0),
		Collision(CPos(35,-15), 19.0),
		Collision(CPos(-35,-15), 19.0) };
	Init(600, Medium, collisions);

	AddLauncher(launcher = new CLauncherM01(m_rank, m_pos, CPos(0, 42)));
}
CEnemyM01::~CEnemyM01() {
}
void CEnemyM01::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	if (m_behaviorComponent->GetBehaviorStatus() != BehaviorStatus::Shot) {
		angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
	}
	else if (launcher->m_shotAngleRock) {
		angle = CFunc::ToRad(launcher->m_shotAngle - 90.0);
	}
	else {
		angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
	}
	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyM01::GetFinalDirectionRad()
{
	if (m_behaviorComponent->GetBehaviorStatus() != BehaviorStatus::Shot) {
		return CFunc::GetTwoPointAngle(m_target, m_pos);
	}
	else {
		if (launcher->m_shotAngleRock) {
			return CFunc::ToRad(launcher->m_shotAngle);
		}
		else {
			return CFunc::GetTwoPointAngle(m_target, m_pos);
		}
	}
}

CLauncherM01::CLauncherM01(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_shotAngle = 0;
	m_shotSpeed = CFunc::RandD(6.5, 10);
};

CLauncherM01::~CLauncherM01() {}

bool CLauncherM01::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 20; // 弾を撃ち始めるまでの時間
	int endTime = startTime + 10000; // 弾を撃っている時間
	int resetTime = endTime + 500; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間
	int span = 50 * RankSpan(); // 弾を撃つ間隔

	if (m_count == startTime) {
		m_shotAngleRock = true;
		m_shotAngle = CFunc::ToDeg(CFunc::GetTwoPointAngle(m_target, m_parent->m_pos));
	}
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			double speed = m_shotSpeed * RankSpeed();
			
			for (int ii = -2; ii <= 2; ii++) {
				double angle = m_shotAngle + 40.0*ii;
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 42);
				b->SetShotEnemyId(m_parent->GetEnemyId());
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
	}

	if (m_count == resetTime) {
		m_shotAngleRock = false;
	}

	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}


//----------------------------------------------------------------------------------------------------------
// CEnemyM02
//----------------------------------------------------------------------------------------------------------
CEnemyM02::CEnemyM02(const CPos& pos) : CBaseEnemy(pos){
	m_image = (CImage*)CGame::GetResource("enemyM2");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = { 
		Collision(CPos(0,-9), 34.0),
		Collision(CPos(35,24), 24.0), 
		Collision(CPos(-35,24), 24.0)};
	Init(600, Medium, collisions);

	AddLauncher(new CLauncherM02(m_rank, m_pos, CPos(36, 46)));
	AddLauncher(new CLauncherM02(m_rank, m_pos, CPos(-36, 46)));
}
CEnemyM02::~CEnemyM02() {
}
void CEnemyM02::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}
// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyM02::GetFinalDirectionRad(){
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}
CLauncherM02::CLauncherM02(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherM02::~CLauncherM02() {}
bool CLauncherM02::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 20; // 弾を撃ち始めるまでの時間
	if (m_parent->GetMoveType() == CBaseEnemy::MoveType::MoveingShot) {
		startTime += 45;
	}
	int resetTime = startTime + 300; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間

	if (m_count == startTime) {
		struct LoopBullet {
			int num;
			double angle;
			double speed;
		};

		std::vector<LoopBullet> loopBullet;
		//								num angle speed
		// 25以下：1　 50以下：2　75:3	 100以下4
		loopBullet.push_back(LoopBullet{ 3 + RankRapidA() * 2,  5.0 * Rank10To15(),  5.5 * Rank10To15() });
		loopBullet.push_back(LoopBullet{ 2 + RankRapidA() * 2,  4.0 * Rank10To15(),  5.0 * Rank10To15() });
		loopBullet.push_back(LoopBullet{ 1 + RankRapidA() * 2,  3.0 * Rank10To15(),  4.5 * Rank10To15() });

		for (const LoopBullet& bullet : loopBullet) {
			for (int jj = 0; jj < bullet.num; jj++) {
				double speed = 6.5 * RankSpeed() + bullet.speed;
				double angle = -bullet.angle + bullet.angle * ((double)jj / bullet.num) * 2.0;
				CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 1);
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
// CEnemyM03
//----------------------------------------------------------------------------------------------------------
CEnemyM03::CEnemyM03(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM3");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(0,20), 24.0),
		Collision(CPos(33,-12), 35.0),
		Collision(CPos(-33,-12), 35.0) };
	Init(600, Medium, collisions);

	AddLauncher(new CLauncherM03(m_rank, m_pos, CPos(36, 46)));
	AddLauncher(new CLauncherM03(m_rank, m_pos, CPos(-36, 46)));
}
CEnemyM03::~CEnemyM03() {
}
void CEnemyM03::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CEnemyM03::GetFinalDirectionRad() {
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

CLauncherM03::CLauncherM03(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherM03::~CLauncherM03() {}

bool CLauncherM03::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 0; // 弾を撃ち始めるまでの時間
	if (m_parent->GetMoveType() == CBaseEnemy::MoveType::MoveingShot) {
		startTime += 45;
	}
	int endTime = startTime + 13 * Rank10To30(); // 弾を撃っている時間
	int resetTime = endTime + 200; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間
	int span = 6 * RankSpan(); // 弾を撃つ間隔

	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			double speed = 8.65 * RankSpeed();

			double angle = CFunc::ToDeg(CFunc::GetTwoPointAngle(m_target, m_parent->m_pos));
			CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 1);
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
// CEnemyM04	ランダム弾
//----------------------------------------------------------------------------------------------------------
CEnemyM04::CEnemyM04(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM4");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(0,0), 23.0),
		Collision(CPos(29,0), 25.0),
		Collision(CPos(-29,0), 25.0) };
	Init(600, Medium, collisions);

	AddLauncher(new CLauncherM04(m_rank, m_pos, CPos(27, 32)));
	AddLauncher(new CLauncherM04(m_rank, m_pos, CPos(-27, 32)));
}
CEnemyM04::~CEnemyM04() {
}
void CEnemyM04::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

double CEnemyM04::GetFinalDirectionRad() {
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

CLauncherM04::CLauncherM04(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherM04::~CLauncherM04() {}

bool CLauncherM04::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 0; // 弾を撃ち始めるまでの時間
	if (m_parent->GetMoveType() == CBaseEnemy::MoveType::MoveingShot) {
		startTime += 45;
	}
	int endTime = startTime + 45; // 弾を撃っている時間
	int resetTime = endTime + 150; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間
	int span = 20; // 弾を撃つ間隔

	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			for (int ii = 0; ii < 5; ii++) {
				double speed = CFunc::RandD(7.5, 9) * RankSpeed();

				double angle = CFunc::RandD(-4,4);
				CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 91);
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
// CEnemyM05	
//----------------------------------------------------------------------------------------------------------
CEnemyM05::CEnemyM05(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM5");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(0,22), 20.0),
		Collision(CPos(18,-13), 30.0),
		Collision(CPos(18,-13), 30.0) };
	Init(600, Medium, collisions);

	AddLauncher(new CLauncherM05(m_rank, m_pos, CPos(0, 42)));
}
CEnemyM05::~CEnemyM05() {
}
void CEnemyM05::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

double CEnemyM05::GetFinalDirectionRad() {
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

CLauncherM05::CLauncherM05(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherM05::~CLauncherM05() {}

bool CLauncherM05::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 0; // 弾を撃ち始めるまでの時間
	if (m_parent->GetMoveType() == CBaseEnemy::MoveType::MoveingShot) {
		startTime += 45;
	}
	int shotTime = startTime + 45; // 弾を撃っている時間
	int resetTime = shotTime + 150; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間

	if (m_count == startTime) {
		int bulletnum = 2 * Rank10To40();
		double basespeed = 10.0 * RankSpeed();
		double slowMaxSpeed = basespeed * 0.5;
		double rangeAngle = 15.0 * Rank10To30();
		for (int jj = -1; jj <= 1; jj += 2) {
			for (int ii = 0; ii < bulletnum; ii++) {
				double speed = basespeed - ((double)ii * slowMaxSpeed / bulletnum);
				double angle = (jj * 3) + (jj * ii * ((double)rangeAngle / bulletnum));
				CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 1);
				b->SetShotEnemyId(m_parent->GetEnemyId());
				CBaseLauncher::m_bulletManager->Add(b);
			}
		}
		double speed = basespeed + 0.55;
		double angle = 0;
		CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 1);
		b->SetShotEnemyId(m_parent->GetEnemyId());
		CBaseLauncher::m_bulletManager->Add(b);
	}

	if (m_count >= resetTime) {
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}



//----------------------------------------------------------------------------------------------------------
// CEnemyM06	
//----------------------------------------------------------------------------------------------------------
CEnemyM06::CEnemyM06(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM6");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(0,13), 30.0),
		Collision(CPos(38,-15), 24.0),
		Collision(CPos(38,-15), 24.0) };
	Init(600, Medium, collisions);

	AddLauncher(new CLauncherM06(m_rank, m_pos, CPos(61, 7)));
	AddLauncher(new CLauncherM06(m_rank, m_pos, CPos(-61, 7)));
}
CEnemyM06::~CEnemyM06() {
}
void CEnemyM06::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

double CEnemyM06::GetFinalDirectionRad() {
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

CLauncherM06::CLauncherM06(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherM06::~CLauncherM06() {}

bool CLauncherM06::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 0; // 弾を撃ち始めるまでの時間
	if (m_parent->GetMoveType() == CBaseEnemy::MoveType::MoveingShot) {
		startTime += 45;
	}
	int endTime = startTime + 30 * Rank10To15(); // 弾を撃っている時間
	int resetTime = endTime + 70; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間
	int span = 7.0 * RankSpan();
	if (m_count == startTime) {
		m_speed = 4.0;
		m_deltaSpeed = 0.2;
	}
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			m_deltaSpeed += 0.2;
			m_speed += m_deltaSpeed;
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, m_speed, 0, 0, 0, 0, 0, 2);
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
// CEnemyM07
//----------------------------------------------------------------------------------------------------------
CEnemyM07::CEnemyM07(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM7");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(0,-12), 39.0),
		Collision(CPos(47,-4), 34),
		Collision(CPos(-47,-4), 34)
	};
	Init(600, Medium, collisions);

	AddLauncher(launcher = new CLauncherM07(m_rank, m_pos, CPos(69, 39)));
	AddLauncher(new CLauncherM07(m_rank, m_pos, CPos(-69, 39)));
}
CEnemyM07::~CEnemyM07() {
}
void CEnemyM07::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

double CEnemyM07::GetFinalDirectionRad() {
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}
CLauncherM07::CLauncherM07(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};
CLauncherM07::~CLauncherM07() {}

bool CLauncherM07::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 50; // 弾を撃ち始めるまでの時間
	if (m_parent->GetMoveType() == CBaseEnemy::MoveType::MoveingShot) {
		startTime += 45;
	}
	int shotTime = startTime + 50; // 弾を撃っている時間
	int resetTime = shotTime + 100; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間

	if (m_count == shotTime) {
		double tempAngle = CFunc::ToDeg(CFunc::GetTwoPointAngle(m_target, m_parent->m_pos));
		double baseSpeed = 8.0 * RankSpeed();
		for (int ii = -1; ii <= 1; ii++) {
			double angle = tempAngle + 10.0 * ii;

			for (int jj = 0; jj < 4; jj++) {
				double speed = baseSpeed + (double)jj * 0.4 + (abs(ii) * -0.5); // -1と1の時はちょっとだけ遅くする
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 92);
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
// CEnemyM08
//----------------------------------------------------------------------------------------------------------
CEnemyM08::CEnemyM08(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM8");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(0,27), 24.0),
		Collision(CPos(38,-14), 28.0),
		Collision(CPos(-38,-14), 28.0) };
	Init(600, Medium, collisions);

	AddLauncher(launcher = new CLauncherM08(m_rank, m_pos, CPos(0, 50)));
}
CEnemyM08::~CEnemyM08() {
}
void CEnemyM08::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	if (m_behaviorComponent->GetBehaviorStatus() != BehaviorStatus::Shot) {
		angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
		BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);
	}
	else if (launcher->m_shotAngleRock) {
		angle = CFunc::ToRad(launcher->m_shotAngle - 90.0);
		m_pos.x += cos(CFunc::ToRad(launcher->m_shotAngle)) * m_nockbackLength;
		m_pos.y += sin(CFunc::ToRad(launcher->m_shotAngle)) * m_nockbackLength;
		BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);
	}
	else {
		angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
		BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);
	}
	

	DebugCollisionDraw();
	DebugLauncherDraw();
}

double CEnemyM08::GetFinalDirectionRad()
{
	if (m_behaviorComponent->GetBehaviorStatus() != BehaviorStatus::Shot) {
		return CFunc::GetTwoPointAngle(m_target, m_pos);
	}
	else {
		if (launcher->m_shotAngleRock) {
			return CFunc::ToRad(launcher->m_shotAngle);
		}
		else {
			return CFunc::GetTwoPointAngle(m_target, m_pos);
		}
	}
}

CLauncherM08::CLauncherM08(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_shotAngle = CFunc::RandD(0, 360);
};

CLauncherM08::~CLauncherM08() {}

bool CLauncherM08::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	constexpr int effectStartTime = 0;
	constexpr int effectEndTime = 60;
	constexpr int effectSpan = 4;
	constexpr int shotTime = effectEndTime + 30;
	constexpr int nockbackTimeS = shotTime + 10;
	constexpr int nockbackTimeE = nockbackTimeS + 10;
	constexpr int endtime = 180;

	if (m_count >= effectStartTime && m_count <= effectEndTime) {
		if (m_count % effectSpan == 0) {
			// 周りから光が集まるエフェクト
			CPos pos;
			double angleDeg = CFunc::RandD(0, 360);
			double angleRad = CFunc::ToRad(angleDeg);
			pos.x = cos(angleRad) * 40.0;
			pos.y = sin(angleRad) * 40.0;
			CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_enemyPos + nowRelativePos + pos, 1.0, 180.0+angleDeg, 0, 0, 0, 0, 01); // "BulletDeleteEffect0"
			eff->SetSize(1.0, -0.015);
			eff->SetBlend(128, -3.0, 0);
			eff->SetBlendType(DX_BLENDMODE_ADD);
			eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
			CBattleScene::m_effectManager.Add(eff);
		}
		if (m_count % effectSpan == 1) { // ちょっとだけずらす
			// エナジーがたまったエフェクト
			double size = 2.0 * ((double)m_count / effectEndTime);
			CPos pos;
			double angleDeg = CFunc::RandD(0, 360);
			double angleRad = CFunc::ToRad(angleDeg);
			CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_enemyPos + nowRelativePos, 0.1, angleDeg, 0, 0, 0, 0, 01); // "BulletDeleteEffect0"
			eff->SetSize(size, 0.015);
			eff->SetBlend(255, -3.0, 0);
			eff->SetBlendType(DX_BLENDMODE_ADD);
			eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
			CBattleScene::m_effectManager.Add(eff);
		}
	}
	if (m_count == shotTime) {
		m_shotAngleRock = true;
		m_shotAngle = CFunc::ToDeg(CFunc::GetTwoPointAngle(m_target, m_parent->m_pos));

		for (int jj = 0; jj < 30; jj++) {
			double speed = CFunc::RandD(10,20) * RankSpeed();
			double angle = CFunc::RandD(-3, 3);
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 61);
			
			b->SetShotEnemyId(m_parent->GetEnemyId());
			CBaseLauncher::m_bulletManager->Add(b);
		}
		((CEnemyM08*)m_parent)->m_nockbackLength = 0;
		m_tempCount = 0;
	}

	if (m_count >= shotTime && m_count <= nockbackTimeS) {
		m_tempCount++;
		((CEnemyM08*)m_parent)->m_nockbackLength += 0.2 * (0.2 - (double)m_tempCount / 1.0);
	} else if (m_count >= nockbackTimeS && m_count <= nockbackTimeE) {
		m_tempCount++;
		((CEnemyM08*)m_parent)->m_nockbackLength -= 0.075 * (0.075 - (double)m_tempCount / 1.0);
	}
	if (m_count == nockbackTimeE) {
		m_shotAngleRock = false;
		((CEnemyM08*)m_parent)->m_nockbackLength = 0;
	}

	if (m_count >= endtime) {
		return true;
	}
	m_count++;
	return true;
}





//----------------------------------------------------------------------------------------------------------
// CEnemyM09	
//----------------------------------------------------------------------------------------------------------
CEnemyM09::CEnemyM09(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM9");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(21,1), 30.0),
		Collision(CPos(-21,-1), 30.0) };
	Init(600, Medium, collisions);

	AddLauncher(new CLauncherM09(m_rank, m_pos, CPos(0, 23)));
}
CEnemyM09::~CEnemyM09() {}
void CEnemyM09::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

double CEnemyM09::GetFinalDirectionRad() {
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

CLauncherM09::CLauncherM09(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherM09::~CLauncherM09() {}

bool CLauncherM09::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 5; // 弾を撃ち始めるまでの時間
	if (m_parent->GetMoveType() == CBaseEnemy::MoveType::MoveingShot) {
		startTime += 45;
	}
	int endTime = startTime + 150; // 弾を撃っている時間
	int resetTime = endTime + 70; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間
	int span = 70 * RankSpan();

	int bulletWay = 1 + RankWay(); // 1~4
	int bulletNum = 2 + RankWay();
	double acce = ((double)bulletNum / 3) * 2.0;
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {

			double baseSpeed = 6.6 * RankSpeed();
			for (int ii = -bulletWay; ii <= bulletWay; ii++) {
				double baseAngle = ii * 36;

				for (int jj = 0; jj < bulletNum; jj++) {
					double angle = baseAngle + CFunc::RandD(-50, 50) / 50.0;
					double speed = baseSpeed + CFunc::RandD(-50, 50) / (400.0 / acce);
					CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 62);
					b->SetShotEnemyId(m_parent->GetEnemyId());
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



//----------------------------------------------------------------------------------------------------------
// CEnemyM10	
//----------------------------------------------------------------------------------------------------------
CEnemyM10::CEnemyM10(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM10");
	m_drawSizeRatio = 1.3;
	std::vector<Collision> collisions = {
		Collision(CPos(0,-15), 40.0) };
	Init(600, Medium, collisions);

	AddLauncher(new CLauncherM10(m_rank, m_pos, CPos(0, 0)));
}
CEnemyM10::~CEnemyM10() {
}
void CEnemyM10::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
	DebugLauncherDraw();
}

double CEnemyM10::GetFinalDirectionRad() {
	return CFunc::GetTwoPointAngle(m_target, m_pos);
}

CLauncherM10::CLauncherM10(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
};

CLauncherM10::~CLauncherM10() {}

bool CLauncherM10::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 30; // 弾を撃ち始めるまでの時間
	if (m_parent->GetMoveType() == CBaseEnemy::MoveType::MoveingShot) {
		startTime += 45;
	}
	int endTime = startTime + 300; // 弾を撃っている時間
	int resetTime = endTime + 70; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間
	int span = 50;
	if (m_count == startTime) {
		m_shotCount = 0;
	}

	int bulletnum = 1 + RankWay(); // 1~4
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {

			double basespeed = 6.6 * RankSpeed();
			if (m_shotCount == 0) {
				for (int jj = 0; jj < bulletnum; jj++) {
					double speed2 = basespeed + (basespeed * 0.05) * jj;
					for (int ii = -1; ii <= 1; ii++) {
						double angle = ii * 12;
						CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed2, angle, 0, 0, 0, 0, 12);
						b->SetShotEnemyId(m_parent->GetEnemyId());
						CBaseLauncher::m_bulletManager->Add(b);
					}
				}
			}
			if (m_shotCount == 1) {
				for (int jj = 0; jj < bulletnum; jj++) {
					double speed2 = basespeed + (basespeed * 0.05) * jj;
					for (int ii = 0; ii < 4; ii++) {
						double angle = -21 + ii * 14; //-21	-7	+7	+21
						CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed2, angle, 0, 0, 0, 0, 12);
						b->SetShotEnemyId(m_parent->GetEnemyId());
						CBaseLauncher::m_bulletManager->Add(b);
					}
				}
			}
			if (m_shotCount == 2) {
				for (int jj = 0; jj < bulletnum; jj++) {
					double speed2 = basespeed + (basespeed * 0.05) * jj;
					for (int ii = -2; ii <= 2; ii++) {
						double angle = ii * 16;
						CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed2, angle, 0, 0, 0, 0, 12);
						b->SetShotEnemyId(m_parent->GetEnemyId());
						CBaseLauncher::m_bulletManager->Add(b);
					}
				}
			}
			m_shotCount++;
			if (m_shotCount > 2) {
				m_shotCount = 0;
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