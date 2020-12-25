
#include "Launcher010.h"

// 1way 射角制限
//	発射間隔が早くなる	12fr～8fr
//	速度が速くなる		4～7
//	連弾になる			2~5弾
CLauncher010::CLauncher010(int rank, const CPos& pos) : CBaseLauncher(rank, pos){
	// 角度制限弾
	int r = CFunc::RandI(1, 3);
	dVal[0] = r * 15.0;
}; 
CLauncher010::~CLauncher010(){

};
void CLauncher010::Action(const CPos& newPos)
{
	__super::Action(newPos);

	int span = (int)(12.0 - 4.0 * RankRatio());
	double speed = 4.0 + 3.0 * RankRatio();
	int loop = (int)(2 + 3 * RankRatio());
	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_pos, SpeedRange(speed, ii,loop), 0.0, 0, 0, 0, 30, 0);
			CBaseLauncher::m_bulletManager->Add(b);
		}
		m_count = 0;

		return;
	}

	m_count++;
}
