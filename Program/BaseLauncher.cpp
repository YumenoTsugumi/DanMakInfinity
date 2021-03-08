
#include "BaseLauncher.h"
#include "BaseEnemy.h"

CBulletManager* CBaseLauncher::m_bulletManager = nullptr;
CBeamManager* CBaseLauncher::m_beamManager = nullptr;
CPos CBaseLauncher::m_target;

void CBaseLauncher::SetBulletManagerPointer(CBulletManager* manager) {
	m_bulletManager = manager;
}
void CBaseLauncher::SetBeamManagerPointer(CBeamManager* manager){
	m_beamManager = manager;
}


CBaseLauncher::CBaseLauncher(int rank, const CPos& enemyPos, const CPos& relativePos) :
	m_rank(rank),
	m_count(0),
	m_enemyPos(enemyPos),
	m_relativePos(relativePos)
{
	memset(iVal, 0, sizeof(iVal));
	memset(dVal, 0, sizeof(dVal));
	memset(cVal, 0, sizeof(cVal));
	memset(bVal, 0, sizeof(bVal));
};
CBaseLauncher::~CBaseLauncher() {

};


void CBaseLauncher::Action(const CPos& newEnemyPos, const CPos& nowRelativePos) {
	m_enemyPos = newEnemyPos;
}


// 現在のランクの割合を返す 0.0～1.0
double CBaseLauncher::RankRatio() {
	int flor = m_rank;
	if (flor >= MAX_RANK)flor = MAX_RANK; // 100以上は1.0とみなす
	return ((double)flor / (double)MAX_RANK);
}
// 連弾の速度差を生成する(speedの10％までをindexで等分割する)
// 10連弾なら　indexは0～9 maxIndexは10
double CBaseLauncher::SpeedRange(double speed, int index, int max)
{
	return speed + (0.1*speed * ((double)index / (double)(max)));
}

void CBaseLauncher::SetTarget(CPos target) {
	m_target = target;
}

void CBaseLauncher::SetParent(CBaseEnemy* parent)
{
	m_parent = parent;
}