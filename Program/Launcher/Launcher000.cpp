
#include "Launcher000.h"
#include "BaseEnemy.h"

// 1way　時機狙いを撃つ
//	発射間隔が早くなる	15fr～7fr
//	速度が速くなる		5～8
//	連弾になる			1~3弾
LANCHERCPP(CLauncher000)
void CLauncher000::Action(const CPos& newEnemyPos)
{
	__super::Action(newEnemyPos);

	int span = (int)(30.0 - 8.0 * RankRatio());
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
	// ずっとループするゲームなんで、ランク（難易度）を導入
	int span = 60;
	double speed = 3.0;
	int loop = 4;
	if (m_count > span) {
		for (int ii = 0; ii < loop; ii++) {
			CBaseBullet* b1 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii, loop), -15.0, 0, 0, 0, 0, 1);
			CBaseBullet* b2 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii, loop), 0.0, 0, 0, 0, 0, 1);
			CBaseBullet* b3 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos, SpeedRange(speed, ii, loop), 15.0, 0, 0, 0, 0, 1);
			CBaseLauncher::m_bulletManager->Add(b1);
			CBaseLauncher::m_bulletManager->Add(b2);
			CBaseLauncher::m_bulletManager->Add(b3);
		}
		m_count = 0;

		return;
	}

	m_count++;
}


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


// 敵が向いている方向にうつ 
LANCHERCPP(CLauncher004)
void CLauncher004::Action(const CPos& newEnemyPos)
{
	__super::Action(newEnemyPos);

	if (m_count > 5) {
		double angle = m_parent->GetDirectionDeg();
		CBaseBullet* b = new CBaseBullet(EDirType::Abs, m_enemyPos + m_parent->GetCollisionData(m_relativePos), 4.0, angle, 0, 0, 0, 0, 0);
		CBaseLauncher::m_bulletManager->Add(b);
		m_count = 0;
		return;
	}

	m_count++;
}



// 敵が向いている方向にうつ 
LANCHERCPP(CLauncher005)
void CLauncher005::Action(const CPos& newEnemyPos)
{
	__super::Action(newEnemyPos);

	if (m_count > 20) { // 1s 60ms  5fr
		for (int ii = 0; ii < 4; ii++) {
			double angle = m_parent->GetDirectionDeg();
			CBaseBullet* b = new CBaseBullet(
				EDirType::Abs,  // 固定角度　か　時機狙いか
				m_enemyPos + m_parent->GetCollisionData(m_relativePos),  // 発射する座標
				0.0, // 速度
				45.0 + 90.0*ii, // 発射角度　固定角度の場合0が真右　90真下　　　　　時機狙いが0だと、プレイヤー
				0.0, // 曲がる感じ
				0.1, // 加速度
				4.0, // 最高速度
				0, // 角度制限
				0);// 画像 
			CBaseLauncher::m_bulletManager->Add(b);
		}
		m_count = 0;
		return;
	}

	m_count++;
}





// テスト
LANCHERCPP(CLauncher999)
void CLauncher999::Action(const CPos& newEnemyPos)
{
#if 0
	static double sppeed = 3.0;
	sppeed += 0.03;
	if (sppeed > 5.0)sppeed = 3.0;
	__super::Action(newEnemyPos);

	if (m_count > 10) {
		int bulletID[] = { 00,10,20,30,50, 60,70,80,90 };
		for (int jj = 0; jj < 9; jj++) {
			for (int ii = 0; ii < 1; ii++) {
				CBaseBullet* b1 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos + CPos(100+jj*80, 0), sppeed, 0, 0, 0, 0, 0, bulletID[jj] + 1);
				CBaseLauncher::m_bulletManager->Add(b1);

				CBaseBullet* b2 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos + CPos(100+jj*80, 0), sppeed, 180, 0, 0, 0, 0, bulletID[jj] + 2);
				CBaseLauncher::m_bulletManager->Add(b2);
			}
		}
		CBaseBullet* b2 = new CBaseBullet(EDirType::Player, m_enemyPos + m_relativePos + CPos(300,300), sppeed, 0, 0, 0, 0, 0, 41);
		CBaseLauncher::m_bulletManager->Add(b2);

		m_count = 0;
		return;
	}
#endif

	static double angle = 0;
	static bool flag = true;
	static double acc = 0.04;

	if (angle >= 1800) {
		flag = false;
	}
	if (angle <= -1800) {
		flag = true;
	}
	if (flag) {
		angle += acc;
		acc += 0.04;
	}
	else {
		angle += acc;
		acc -= 0.04;
	}

	int bulletID[] = { 00,10,20,30,50, 60,70,80,90 };
	int ran = rand() % 9;
	double speed = 6.0;
	CBaseBullet* b1 = new CBaseBullet(EDirType::Abs, CPos(GameWindowCenterX, GameWindowCenterY), speed, angle, 0, 0, 0, 0, bulletID[ran] + 1);
	CBaseLauncher::m_bulletManager->Add(b1);

	CBaseBullet* b2 = new CBaseBullet(EDirType::Abs, CPos(GameWindowCenterX, GameWindowCenterY), speed, angle + 180, 0, 0, 0, 0, bulletID[ran] + 2);
	CBaseLauncher::m_bulletManager->Add(b2);

	m_count++;
}