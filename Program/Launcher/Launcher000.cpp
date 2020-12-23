
#include "Launcher000.h"

// 1way　時機狙いを撃つ
//	発射間隔が早くなる	15fr～7fr
//	速度が速くなる		5～8
//	連弾になる			1~3弾
LANCHERCPP(Launcher000)
void Launcher000::Action(const CPos& newPos)
{
	__super::Action(newPos);

	int span = (int)(15.0 - 8.0 * RankRatio());
	double speed = 5.0 + 3.0 * RankRatio();
	int loop = (int)(1 + 2 * RankRatio());
	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_pos, SpeedRange(speed, ii,loop), 0.0, 0, 0, 0, 0,0);
			BaseLauncher::m_bulletManager->Add(b);
		}
		m_count = 0;

		return;
	}

	m_count++;
}

// 3way　時機狙いを撃つ
//	発射間隔が早くなる	30fr～20fr
//	速度が速くなる		3～4
//	連弾になる			1~3弾
LANCHERCPP(Launcher001)
void Launcher001::Action(const CPos& newPos)
{
	__super::Action(newPos);

	int span = (int)(30.0 - 10.0 * RankRatio());
	double speed = 3.0 + 1.0 * RankRatio();
	int loop = (int)(1 + 2 * RankRatio());
	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			CBaseBullet* b1 = new CBaseBullet(EDirType::Player, m_pos, SpeedRange(speed, ii, loop), -15.0, 0, 0, 0, 0, 0);
			CBaseBullet* b2 = new CBaseBullet(EDirType::Player, m_pos, SpeedRange(speed, ii, loop), 0.0, 0, 0, 0, 0, 0);
			CBaseBullet* b3 = new CBaseBullet(EDirType::Player, m_pos, SpeedRange(speed, ii, loop), 15.0, 0, 0, 0, 0, 0);
			BaseLauncher::m_bulletManager->Add(b1);
			BaseLauncher::m_bulletManager->Add(b2);
			BaseLauncher::m_bulletManager->Add(b3);
		}
		m_count = 0;

		return;
	}

	m_count++;
}