﻿
#include "Launcher010.h"

// 1way 射角制限
//	発射間隔が早くなる	12fr～8fr
//	速度が速くなる		4～7
//	連弾になる			2~5弾
CLauncher010::CLauncher010(int rank, const CPos& enemyPos, const CPos& relativePos) : CBaseLauncher(rank, enemyPos, relativePos){
	// 角度制限弾
	int r = CFunc::RandI(1, 3);
	dVal[0] = r * 15.0;
}; 
CLauncher010::~CLauncher010(){

};
void CLauncher010::Action(const CPos& newEnemyPos)
{
	__super::Action(newEnemyPos);

	int span = (int)(12.0 - 4.0 * RankRatio());
	double speed = 4.0 + 3.0 * RankRatio();
	int loop = (int)(6 + 3 * RankRatio());
	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii,loop), ii*60, 0, 0, 0, 30, 20 + ii%3);
			CBaseLauncher::m_bulletManager->Add(b);
		}
		//for (int ii = 0; ii < loop; ii++) {
		//	CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii, loop), -45, 0, 0, 0, 30, 1);
		//	CBaseLauncher::m_bulletManager->Add(b);
		//}
		m_count = 0;

		return;
	}

	m_count++;
}
