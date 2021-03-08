
#include "Launcher030.h"

// 真下にマシンガン
CLauncher030::CLauncher030(int rank, const CPos& enemyPos, const CPos& relativePos) : CBaseLauncher(rank, enemyPos, relativePos){

	
}; 
CLauncher030::~CLauncher030(){

};
void CLauncher030::Action(const CPos& newEnemyPos, const CPos& nowRelativePos)
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
			CBaseBullet* b1 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, speed, angle+ii, 0, 0, 0, 0, 11);
			CBaseLauncher::m_bulletManager->Add(b1);

			CBaseBullet* b2 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, speed, angle-ii + 180.0, 0, 0.05, speed*1.5, 0, 11);
			CBaseLauncher::m_bulletManager->Add(b2);
		}


		return;
	}

	m_count++;
}
