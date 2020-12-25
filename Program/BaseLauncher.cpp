﻿
#include "BaseLauncher.h"

CBulletManager* CBaseLauncher::m_bulletManager = nullptr;
CBeamManager* CBaseLauncher::m_beamManager = nullptr;

void CBaseLauncher::SetBulletManagerPointer(CBulletManager* manager) {
	m_bulletManager = manager;
}
void CBaseLauncher::SetBeamManagerPointer(CBeamManager* manager){
	m_beamManager = manager;
}


CBaseLauncher::CBaseLauncher(int rank, const CPos& pos) :
	m_rank(rank),
	m_count(0),
	m_pos(pos)
{
	memset(iVal, 0, sizeof(iVal));
	memset(dVal, 0, sizeof(dVal));
	memset(cVal, 0, sizeof(cVal));
	memset(bVal, 0, sizeof(bVal));
};
CBaseLauncher::~CBaseLauncher() {

};


void CBaseLauncher::Action(const CPos& newPos) {
	m_pos = newPos;
}


// 現在のランクの割合を返す 0.0～1.0
double CBaseLauncher::RankRatio() {
	return ((double)m_rank / (double)MAX_RANK);
}
// 連弾の速度差を生成する(speedの10％までをindexで等分割する)
// 10連弾なら　indexは0～9 maxIndexは10
double CBaseLauncher::SpeedRange(double speed, int index, int max)
{
	return speed + (0.1*speed * ((double)index / (double)(max)));
}