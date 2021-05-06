
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


bool CBaseLauncher::Action(const CPos& newEnemyPos, const CPos& nowRelativePos) {
	m_enemyPos = newEnemyPos;

	if (m_parent->GetWaitShotTime() >= 0) return true;
	return false;
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


/*
ランク0　　（普通）弾速1.0　弾数1.0　間隔1.0

ランク50   （普通）弾速1.5　弾数1.5　間隔*(1/1.25)
　弾追加　　（追加）弾速1.0　弾数1.0　間隔1.0
ランク99　（普通）弾速2.0　弾数2.0　間隔*(1/1.5)
　弾追加　　（追加）弾速2.0　弾数2.0　間隔1.5
ランク200 ここから適当
　弾追加　　（追加）弾速3.0　弾数3.0　間隔1.5
 ランク300
　弾追加　　（追加）弾速4.0　弾数4.0　間隔1.5
*/

double CBaseLauncher::Rank10To15() {
	return 1.0 + 0.5 * (m_rank / 100.0);
}

// ランクによる速度変化を返す
double CBaseLauncher::RankSpeed(){
	return 1.0 + 1.0 * (m_rank / 100.0);
}
// ランクによる弾の総数を返す
double CBaseLauncher::RankBulletNum(){
	return 1.0 + 1.0 * (m_rank / 100.0);
}
// ランクによる発射間隔を返す
double CBaseLauncher::RankSpan(){
	if (m_rank <= 100) {
		return 1.0 / (1.0 + 0.5 * (m_rank / 100.0));
	} 
	return 1.0 / 1.5;
}
int CBaseLauncher::RankWay() {
	// 1~33:0   34~66:2   67~99:4    100
	return (m_rank / 34) * 2;
}
// ランクによる連弾の個数を返す
int CBaseLauncher::RankRapidA() {
	// 25以下：1　 50以下：2　 100以下4
	return 1 + m_rank / 25;
}
// ランクによる連弾の個数を返す
int CBaseLauncher::RankRapidB() {
	// 25以下：1　 50以下：3　 100以下7
	return 1 + (m_rank / 25 * 2);
}