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
	Init(100, Small, collisions);

	AddLauncher(CPos(0, 0), new CLauncherS01(0, m_pos, CPos(21, 31), ShotTypeS01::Slow));
	AddLauncher(CPos(0, 0), new CLauncherS01(0, m_pos, CPos(-21, 31), ShotTypeS01::Slow));
	AddLauncher(CPos(0, 0), new CLauncherS01(0, m_pos, CPos(0, 14), ShotTypeS01::Rapid));
}
CEnemyS01::~CEnemyS01() {
}
void CEnemyS01::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}

// 1way　時機狙いを撃つ
//	発射間隔が早くなる	15fr～7fr
//	速度が速くなる		5～8
//	連弾になる			1~3弾
CLauncherS01::CLauncherS01(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type) :
	CBaseLauncher(rank, enemyPos, relativePos) {
	m_type = type;
};
CLauncherS01::~CLauncherS01() {

}
void CLauncherS01::Action(const CPos& newEnemyPos)
{
	__super::Action(newEnemyPos);

	int span = (int)(30.0 - 8.0 * RankRatio());
	double speed = 5.0 + 3.0 * RankRatio();
	int loop = (int)(1 + 2 * RankRatio());
	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii, loop), 0.0, 0, 0, 0, 0, 1);
			CBaseLauncher::m_bulletManager->Add(b);
		}
		m_count = 0;

		return;
	}

	m_count++;
}
