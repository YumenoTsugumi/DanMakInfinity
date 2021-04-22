#include "EnemyM.h"
#include "DXFunc.h"
#include "BattleScene.h"
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

void CLauncherM02::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

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
		return;
	}
	m_count++;
}
