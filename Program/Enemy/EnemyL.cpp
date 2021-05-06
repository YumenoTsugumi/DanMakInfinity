#include "EnemyL.h"
#include "DXFunc.h"

#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"

//----------------------------------------------------------------------------------------------------------
// CEnemyL01
//----------------------------------------------------------------------------------------------------------
CEnemyL01::CEnemyL01(const CPos& pos) : CBaseEnemy(pos){
	SetDrawSize(2.0); // 画像が小さかったので二倍
	m_image = (CImage*)CGame::GetResource("enemyL1");
	m_shotTiming = true;
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

bool CLauncherL01a::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 20;
	int endTime = startTime + 60;
	int resetTime = endTime + 120;
	int span = 1;
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			double angle = CFunc::GetTwoPointAngle_180Deg(m_enemyPos, m_target); // angle
			double speed = 10.0 + 2.0 * RankRatio();

			for (int ii = 0; ii < 5; ii++) {
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

bool CLauncherL01b::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	bool waitShot = __super::Action(newEnemyPos, nowRelativePos);
	if (!waitShot) return true;

	int startTime = 20;
	int endTime = startTime + 60;
	int resetTime = endTime + 20;
	int span = 5;
	if (m_count >= startTime && m_count <= endTime) {
		if (m_count % span == 0) {
			double angle;
			if (m_lr == LauncherPos::LauncherLeft) {
				angle = ((CEnemyL01*)m_parent)->m_shotAngleL;
				((CEnemyL01*)m_parent)->m_shotAngleL += -5;
			}
			else {
				angle = ((CEnemyL01*)m_parent)->m_shotAngleR;
				((CEnemyL01*)m_parent)->m_shotAngleR -= -5;
			}
			double speed = 5.0 + 2.0 * RankRatio();

			for (int ii = 0; ii < 4; ii++) {
				double shotAngle = angle + 90.0 * ii;
				CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos , speed, shotAngle, 0, 0, 0, 0, 1);
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
