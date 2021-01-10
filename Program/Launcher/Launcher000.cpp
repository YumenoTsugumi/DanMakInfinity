
#include "Launcher000.h"

// 1way　時機狙いを撃つ
//	発射間隔が早くなる	15fr～7fr
//	速度が速くなる		5～8
//	連弾になる			1~3弾
LANCHERCPP(CLauncher000)
void CLauncher000::Action(const CPos& newEnemyPos)
{
	__super::Action(newEnemyPos);

	int span = (int)(15.0 - 8.0 * RankRatio());
	double speed = 5.0 + 3.0 * RankRatio();
	int loop = (int)(1 + 2 * RankRatio());
	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			CBaseBullet* b = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii,loop), 0.0, 0, 0, 0, 0,0);
			CBaseLauncher::m_bulletManager->Add(b);
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
LANCHERCPP(CLauncher001)
void CLauncher001::Action(const CPos& newPos)
{
	__super::Action(newPos);

	int span = (int)(30.0 - 10.0 * RankRatio());
	double speed = 3.0 + 1.0 * RankRatio();
	int loop = (int)(1 + 2 * RankRatio());
	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			CBaseBullet* b1 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii, loop), -15.0, 0, 0, 0, 0, 0);
			CBaseBullet* b2 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii, loop), 0.0, 0, 0, 0, 0, 0);
			CBaseBullet* b3 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii, loop), 15.0, 0, 0, 0, 0, 0);
			CBaseLauncher::m_bulletManager->Add(b1);
			CBaseLauncher::m_bulletManager->Add(b2);
			CBaseLauncher::m_bulletManager->Add(b3);
		}
		m_count = 0;

		return;
	}

	m_count++;
}

#include "BaseEnemy.h"
// 等角度2wayしばらくの時間　時機狙いを撃つ　ぶっちゃけナイトメアの先頭砲台
LANCHERCPP(CLauncher002)
void CLauncher002::Action(const CPos& newPos)
{
	__super::Action(newPos);

	int start = 0;
	int end = 26;
	int reset = 60;

	if (m_count == 0) {
		dVal[0] = CFunc::GetTwoPointAngle_180Deg(m_enemyPos, m_target);
		dVal[1] = 1.5;
	}
	if (m_count > start && m_count < end) {
		if (m_count % 2 == 0) {
			CBaseBullet* b1 = new CBaseBullet(EDirType::Abs, m_enemyPos + m_relativePos + CPos(10.0,0), dVal[1], dVal[0], 0, 0, 0, 0, 11);
			CBaseLauncher::m_bulletManager->Add(b1);

			CBaseBullet* b2 = new CBaseBullet(EDirType::Abs, m_enemyPos + m_relativePos + CPos(-10.0, 0), dVal[1], dVal[0], 0, 0, 0, 0, 11);
			CBaseLauncher::m_bulletManager->Add(b2);

			dVal[1] += 0.4;
		}
		m_count++;
		return;
	}


	m_count++;

	if (m_count == reset) {
		m_count = 0;
	}
}