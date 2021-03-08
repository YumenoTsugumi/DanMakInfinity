#include "StageManager.h"
#include "BattleScene.h"

#include "EnemySmall.h"
#include "EnemyMedium.h"
#include "EnemyLarge.h"
#include "EnemyS01.h"

#include "Spawner.h"

SPAWNECPP(TestSpawner001)
void TestSpawner001::Spawne() 
{
	if (m_count == 0) {
		m_maxCount = ToSecond(2);
		m_spawneTiming = ToSecond(0.2);

	}
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}

	if (m_count == 0) {
		double x = ToGamePosX(0.5);
		double y = ToGamePosY(-0.1);
		CPos pos1(x, y);
		CEnemyS01* e1 = new CEnemyS01(pos1);
		CInOutBehavior* move = new CInOutBehavior(pos1, pos1 + CPos(0, ToGamePosX(0.4)), 7, 10, 180);
		e1->SetBehaviorComponent(move);

		CBattleScene::m_enemyManager.Add(e1);
	}
	m_count++;
}
int TestSpawner001::GetPower() { return 1; }



SPAWNECPP(Spawner001)
void Spawner001::Spawne()
{
	if (m_count == 0) {
		m_maxCount = ToSecond(2);
		m_spawneTiming = ToSecond(0.2);
		iVal[0] = CFunc::RandI(0, 2); // 出現位置
	}
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		double x;
		if (iVal[0] == 0) {
			x = ToGamePosX(CFunc::RandD(0.05, 0.4));
		}
		else if (iVal[0] == 1) {
			x = ToGamePosX(CFunc::RandD(0.3, 0.7));
		}
		else if (iVal[0] == 2) {
			x = ToGamePosX(CFunc::RandD(0.6, 0.95));
		}

		double y = ToGamePosY(-0.1);
		double goalAddY = ToGamePosY(0.20) + ToGamePosY(CFunc::RandD(0.05, 0.4));
		CPos pos1(x, y);
		CEnemyS01* e1 = new CEnemyS01(pos1);
		CInOutBehavior* move = new CInOutBehavior(pos1, pos1 + CPos(0, goalAddY), 7, 10, 180);
		e1->SetBehaviorComponent(move);

		CBattleScene::m_enemyManager.Add(e1);
	}
}

int Spawner001::GetPower(){	return 1; }



