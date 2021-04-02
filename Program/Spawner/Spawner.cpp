#include "StageManager.h"
#include "BattleScene.h"


#include "EnemyS01.h"
#include "EnemyM02.h"

#include "Spawner.h"


SpawnerSmallTop_Stop::SpawnerSmallTop_Stop()
{
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneCount = 12;
	m_spawneTiming = m_maxCount / m_spawneCount;
	m_appearancePosition = CFunc::RandD(-0.15, +0.15); // 出現位置の誤差
	returnPettern = CFunc::RandI(0, 3); // 元の場所に戻る、対角に進む、時機狙い１、時機狙い２
}

SpawnerSmallTop_Stop::~SpawnerSmallTop_Stop(){}

void SpawnerSmallTop_Stop::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {

		double x = ToGamePosX(CFunc::RandD(0.20, 0.70) + m_appearancePosition);
		double y = ToGamePosY(-0.1);
		double goalAddY = ToGamePosY(0.20) + ToGamePosY(CFunc::RandD(0.05, 0.4));
		CPos pos(x, y);
		CBaseEnemy* enemy = GetSmallEnemy(pos);
		CInOutBehavior* move = nullptr;

		if (returnPettern == 0) {
			move = new CInOutBehavior(pos, pos + CPos(0, goalAddY), pos, 7, 9, 180);
		}
		else if (returnPettern == 1) {
			double returnGoalAddY = ToGamePosY(1.20);
			move = new CInOutBehavior(pos, pos + CPos(0, goalAddY), pos + CPos(0, returnGoalAddY), 7, 9, 180);
		}
		else if (returnPettern == 2) {
			move = new CInOutBehavior(pos, pos + CPos(0, goalAddY), pos, 7, 8, 180);
			move->SetLeaveDirToPlayer();
		}
		else if (returnPettern == 3) {
			move = new CInOutBehavior(pos, pos + CPos(0, goalAddY), pos, 7, 7, 180);
			move->SetLeaveDirToPlayer2();
		}
		else {
			assert(0);
		}
			
		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);
	}
}

//-----------------------------------------------------------------------

SpawnerSmallLeftRight_Stop::SpawnerSmallLeftRight_Stop()
{
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneCount = 12;
	m_spawneTiming = m_maxCount / m_spawneCount;
	spawnerPettern = CFunc::RandI(0, 1); // 出現位置パターン
	returnPettern = CFunc::RandI(0, 3); // 元の場所に戻る、対角に進む、時機狙い１、時機狙い２

	if (spawnerPettern == 0) {
		spawnerTargetPosX = CFunc::RandD(0.35, 1.1);
	}
	else {
		spawnerTargetPosX = CFunc::RandD(-0.1, 0.65);
	}

	m_appearancePosition = CFunc::RandD(-0.15, +0.15); // 出現位置の誤差

	
}

SpawnerSmallLeftRight_Stop::~SpawnerSmallLeftRight_Stop() {}

void SpawnerSmallLeftRight_Stop::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		// 左右から出てきて、真ん中3割地点で減速して、うって、そのまま直進
		double x = 0;
		double tartgetAddX = 0;
		double goalAddX = 0;
		if (spawnerPettern == 0) {
			x = ToGamePosX(CFunc::RandD(-0.1, -0.2));
			tartgetAddX = ToGamePosX(spawnerTargetPosX + CFunc::RandD(-0.15, +0.15));
			goalAddX = ToGamePosX(1.40);
		}
		else if (spawnerPettern == 1) {
			x = ToGamePosX(CFunc::RandD(1.1, 1.2));
			tartgetAddX = ToGamePosX(spawnerTargetPosX + CFunc::RandD(-0.15, +0.15));
			goalAddX = ToGamePosX(-0.4);
		}
		double y1 = ToGamePosY(CFunc::RandD(0.1, 0.4));
		double y2 = ToGamePosY(CFunc::RandD(0.1, 0.3));
		CPos pos(x, y1);
		CBaseEnemy* enemy = GetSmallEnemy(pos);

		CInOutBehavior* move = nullptr;

		if (returnPettern == 0) {
			move = new CInOutBehavior(pos, CPos(tartgetAddX, y2), pos, 7, 10, 180);
		}
		else if (returnPettern == 1) {
			move = new CInOutBehavior(pos, CPos(tartgetAddX, y2), CPos(goalAddX, y1), 7, 10, 180);
		}
		else if (returnPettern == 2) {
			move = new CInOutBehavior(pos, CPos(tartgetAddX, y2), CPos(goalAddX, y1), 7, 10, 180);
			move->SetLeaveDirToPlayer();
		}
		else if (returnPettern == 3) {
			move = new CInOutBehavior(pos, CPos(tartgetAddX, y2), CPos(goalAddX, y1), 7, 10, 180);
			move->SetLeaveDirToPlayer2();
		}
		else {
			assert(0);
		}

		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);
	}
	
}









CBaseEnemy* SpawnerMedium::GetEnemy(const CPos& pos)
{
	return new CEnemyS01(pos);
}


SpawnerMedium::SpawnerMedium() {
	m_maxCount = ToSecond(2.5);
	m_spawneTiming = ToSecond(0.4);
	spawnerPettern = CFunc::RandI(0, 1); // 出現位置

	m_appearancePosition = -0.1;
}

SpawnerMedium::~SpawnerMedium() {

}
void SpawnerMedium::Spawne()
{

	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	// 左上から１体ずつ4帯
	if (spawnerPettern == 0 || 1) {
		double x;

		if (m_count % m_spawneTiming == 0) {
			x = ToGamePosX(m_appearancePosition);
			m_appearancePosition += 0.2;
		
			double y = ToGamePosY(-0.1);
			CPos pos1(x, y);
			CPos pos2(x, y + ToGamePosY(1.20));
			CEnemyM02* e2 = new CEnemyM02(CPos(x, y));
			CGoTargetBehavior* move = new CGoTargetBehavior(pos1, pos2, 1.35, 0);

			e2->SetBehaviorComponent(move);

			CBattleScene::m_enemyManager.Add(e2);
		}
	}
	//if (m_count % m_spawneTiming == 0) {
	//	double x;
	//	if (iVal[0] == 0) {
	//		x = ToGamePosX(CFunc::RandD(0.05, 0.4));
	//	}
	//	else if (iVal[0] == 1) {
	//		x = ToGamePosX(CFunc::RandD(0.3, 0.7));
	//	}
	//	else if (iVal[0] == 2) {
	//		x = ToGamePosX(CFunc::RandD(0.6, 0.95));
	//	}

	//	double y = ToGamePosY(-0.1);
	//	double goalAddY = ToGamePosY(0.20) + ToGamePosY(CFunc::RandD(0.05, 0.4));
	//	CPos pos1(x, y);
	//	CEnemyM02* e1 = new CEnemyM02(pos1);
	//	CInOutBehavior* move = new CInOutBehavior(pos1, pos1 + CPos(0, goalAddY), 7, 10, 180);
	//	e1->SetBehaviorComponent(move);

	//	CBattleScene::m_enemyManager.Add(e1);
	//}
}


