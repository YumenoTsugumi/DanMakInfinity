
#include "Launcher020.h"

// 蛾 の回転砲台

//	速度が速くなる		4～7
//	連弾になる			2~5弾
CLauncher020::CLauncher020(int rank, const CPos& enemyPos, const CPos& relativePos, bool left) : CBaseLauncher(rank, enemyPos, relativePos){
	bVal[0] = left;

	if (left) {
		dVal[0] = 180.0; // 発射開始角度
	}
	else {
		dVal[0] = 0.0; // 発射開始角度
	}
	dVal[1] = -20.0; // フレーム毎の回転角度
}; 
CLauncher020::~CLauncher020(){

};
void CLauncher020::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
{
	__super::Action(newEnemyPos, nowRelativePos);

	if (bVal[0] == true) {
		dVal[0] += dVal[1];
	}
	else {
		dVal[0] -= dVal[1];
	}

	int span = 10;

	if (m_count > span) {
		m_count = 0;

		int rendanLoop = 5;
		for (int ii = 0; ii < rendanLoop; ii++) {
			double speed = 1.5 + ii*0.1;
			double angle = dVal[0];
			CBaseBullet* b1 = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle+ii, 0, 0, 0, 0, 01);
			CBaseLauncher::m_bulletManager->Add(b1);

			CBaseBullet* b2 = new CBaseBullet(EDirType::Player, m_enemyPos + nowRelativePos, speed, angle-ii + 180.0, 0, 0.05, speed*1.5, 0, 01);
			CBaseLauncher::m_bulletManager->Add(b2);
		}

		return;
	}

	m_count++;
}
