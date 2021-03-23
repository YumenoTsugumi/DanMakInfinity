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
