#include "EnemyM02.h"
#include "DXFunc.h"

#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"

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
	Init(1500, Medium, collisions);

	AddLauncher(new CLauncherM02(m_rank, m_pos, CPos(36, 46)));
	AddLauncher(new CLauncherM02(m_rank, m_pos, CPos(-36, 46)));
}
CEnemyM02::~CEnemyM02() {
}
void CEnemyM02::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = GetFinalDirectionRad() - CFunc::ToRad(90.0);

	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

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
};

CLauncherM02::~CLauncherM02() {

}

void CLauncherM02::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	int span;
	double speed;

	speed = 6.0 + 2.0 * RankRatio();
	span = (int)(30.0 - 8.0 * RankRatio());

	if (m_count >= 0 && m_count < 40) {
		if (m_count == 0) {
			dVal[0] = CFunc::GetTwoPointAngle_180Deg(m_enemyPos, m_target); // angle
			dVal[1] = 4.3; // speed
		}
		if (m_count % 4 == 0) {
			CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + nowRelativePos, dVal[1], dVal[0], 0, 0, 0, 0, 1);
			CBaseLauncher::m_bulletManager->Add(b);
		}
	}
	else if (m_count == 60) {
		m_count = 0;
		return;
	}	

	m_count++;
}
