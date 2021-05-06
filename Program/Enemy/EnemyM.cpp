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
	m_shotTiming = true;
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
	//double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	//BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

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
	//return CFunc::ToRad(90.0);
	//return CFunc::GetTwoPointAngle(m_target, m_pos);
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
	m_shotTiming = true;
	std::vector<Collision> collisions = { 
		Collision(CPos(0,-9), 34.0),
		Collision(CPos(35,24), 24.0), 
		Collision(CPos(-35,24), 24.0)};
	Init(600, Medium, collisions);

	AddLauncher(new CLauncherM02(m_rank, m_pos, CPos(36, 46)));
	//AddLauncher(new CLauncherM02(m_rank, m_pos, CPos(-36, 46)));
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
double CEnemyM02::GetFinalDirectionRad()
{
	return CFunc::ToRad(90.0);
}

CLauncherM02::CLauncherM02(int rank, const CPos& enemyPos, const CPos& relativePos) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_shotAngle = CFunc::RandD(0, 360);
};

CLauncherM02::~CLauncherM02() {}

bool CLauncherM02::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 20; // 弾を撃ち始めるまでの時間
	int endTime = startTime + 10; // 弾を撃っている時間
	int resetTime = endTime + 300; // 弾を撃ち終わって、また弾を撃ち始めるまでの時間
	int span = 3 * RankSpan(); // 弾を撃つ間隔
	if (m_count == startTime) {
		struct LoopBullet {
			int num;
			double angle;
			double speed;
		};

		std::vector<LoopBullet> loopBullet;
		//								num angle speed
		loopBullet.push_back(LoopBullet{ 7 + RankRapidA(),  5.0 * Rank10To15(),  5.5 * Rank10To15() });
		loopBullet.push_back(LoopBullet{ 6 + RankRapidA(),  4.0 * Rank10To15(),  5.0 * Rank10To15() });
		loopBullet.push_back(LoopBullet{ 5 + RankRapidA(),  3.0 * Rank10To15(),  4.5 * Rank10To15() });

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
// CEnemyM08
//----------------------------------------------------------------------------------------------------------
CEnemyM08::CEnemyM08(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyM8");
	//m_shotTiming = true;
	std::vector<Collision> collisions = {
		Collision(CPos(0,27), 24.0),
		Collision(CPos(38,-14), 28.0),
		Collision(CPos(-38,-14), 28.0) };
	Init(600, Medium, collisions);

	AddLauncher(launcher = new CLauncherM08(m_rank, m_pos, CPos(0, 50)));
	//AddLauncher(new CLauncherM08(m_rank, m_pos, CPos(-36, 46)));
}
CEnemyM08::~CEnemyM08() {
}
void CEnemyM08::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	//double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	//BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	if (m_behaviorComponent->GetBehaviorStatus() != BehaviorStatus::Shot) {
		angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
		BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);
	}
	else if (launcher->m_shotAngleRock) {
		angle = CFunc::ToRad(launcher->m_shotAngle - 90.0);
		CPos pos = m_pos;
		pos.x += cos(CFunc::ToRad(launcher->m_shotAngle)) * m_nockbackLength;
		pos.y += sin(CFunc::ToRad(launcher->m_shotAngle)) * m_nockbackLength;
		BaseDraw(pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);
	}
	else {
		angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);
		BaseDraw(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);
	}
	

	DebugCollisionDraw();
	DebugLauncherDraw();
}

// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
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

	//return CFunc::ToRad(90.0);
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

	int effectStartTime = 0;
	int effectEndTime = 60;
	int effectSpan = 4;
	int shotTime = effectEndTime + 30;
	int nockbackTime = shotTime + 10;

	if (m_count >= effectStartTime && m_count <= effectEndTime) {
		if (m_count % effectSpan == 0) {
			// エフェクト
			CPos pos;
			double angleDeg = CFunc::RandD(0, 360);
			double angleRad = CFunc::ToRad(angleDeg);
			pos.x = cos(angleRad) * 40.0;
			pos.y = sin(angleRad) * 40.0;
			CBaseEffect* eff = new CBaseEffect(10, EDirType::Abs, m_enemyPos + nowRelativePos + pos, 1.0, 180.0+angleDeg, 0, 0, 0, 0, 01); // "BulletDeleteEffect0"
			eff->SetSize(1.0, -0.015);
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
			double speed = CFunc::RandD(15,25) * RankSpeed();
			double angle = CFunc::RandD(-3, 3);
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle, 0, 0, 0, 0, 1);
			
			b->SetShotEnemyId(m_parent->GetEnemyId());
			CBaseLauncher::m_bulletManager->Add(b);
		}
		((CEnemyM08*)m_parent)->m_nockbackLength = 0;
		m_tempCount = 0;
	}

	if (m_count >= shotTime && m_count <= nockbackTime) {
		m_tempCount++;
		((CEnemyM08*)m_parent)->m_nockbackLength -= 10 * (1.0 - (double)m_tempCount / 10.0);
	}


	if (m_count >= nockbackTime) {
		m_tempCount = 0;
		m_count = 0;
		return true;
	}
	m_count++;
	return true;
}
