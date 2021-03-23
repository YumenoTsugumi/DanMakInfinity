#include "StageManager.h"
#include "BattleScene.h"

//#include "EnemySmall.h"
//#include "EnemyMedium.h"
//#include "EnemyLarge.h"
#include "EnemyS01.h"
#include "EnemyM02.h"

#include "Spawner.h"

SPAWNECPP(SpawnerS01)
void SpawnerS01::Spawne()
{
	if (m_count == 0) {
		m_maxCount = ToSecond(FormationSpawneFinishTiming);
		m_spawneCount = 12;
		m_spawneTiming = m_maxCount / m_spawneCount;
		//iVal[0] = CFunc::RandI(0, 7); // 出現位置パターン

		static int once = -1;
		once++;
		iVal[0] = once;

		if (iVal[0] == 5 || iVal[0] == 6 || iVal[0] == 7) {
			// 同時に3体出すのでスポーン回数を/3する
			m_spawneCount = 12/3;
			m_spawneTiming = m_maxCount / m_spawneCount;
			dVal[0] = CFunc::RandD(-0.1, +0.1); // 出現位置の誤差
		}
	}
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		if (iVal[0] == 0 || iVal[0] == 1 || iVal[0] == 2) { // 上から出てきて帰る
			double x = 0;
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
			CInOutBehavior* move = new CInOutBehavior(pos1, pos1 + CPos(0, goalAddY), pos1, 7, 10, 180);
			e1->SetBehaviorComponent(move);
			CBattleScene::m_enemyManager.Add(e1);
		}
		else if (iVal[0] == 3 || iVal[0] == 4) { // 左右から出てきて、真ん中3割地点で減速して、うって、そのまま直進
			double x = 0;
			double tartgetAddX = 0;
			double goalAddX = 0;
			if (iVal[0] == 3) {
				x = ToGamePosX(CFunc::RandD(-0.1, -0.1));
				tartgetAddX = ToGamePosX(CFunc::RandD(0.15, 0.6));
				goalAddX = ToGamePosX(1.40);
			}
			else if (iVal[0] == 4) {
				x = ToGamePosX(CFunc::RandD(1.1, 1.2));
				tartgetAddX = ToGamePosX(CFunc::RandD(0.3, 0.85));
				goalAddX = ToGamePosX(-0.4);
			}
			double y1 = ToGamePosY(CFunc::RandD(0.1, 0.4));
			double y2 = ToGamePosY(CFunc::RandD(0.1, 0.3));
			CPos pos1(x, y1);
			CEnemyS01* e1 = new CEnemyS01(pos1);
			CInOutBehavior* move = new CInOutBehavior(pos1, CPos(tartgetAddX, y2), CPos(goalAddX, y1), 7, 10, 180);
			e1->SetBehaviorComponent(move);
			CBattleScene::m_enemyManager.Add(e1);
		}
		else if (iVal[0] == 5 || iVal[0] == 6 || iVal[0] == 7) { // 上からそのまま画面下にゆっくり進行
			double x = 0;
			if (iVal[0] == 5) {
				x = ToGamePosX(0.3 + dVal[0]);
			}
			else if (iVal[0] == 6) {
				x = ToGamePosX(0.5 + dVal[0]);
			}
			else if (iVal[0] == 7) {
				x = ToGamePosX(0.7 + dVal[0]);
			}
			double y = ToGamePosY(-0.1);

			for (int ii = -1; ii <= 1; ii++) {
				CPos pos1(x + (ToGameSizeX(0.1)*ii), y);
				CEnemyS01* e1 = new CEnemyS01(pos1);
				CGoTargetBehavior* move = new CGoTargetBehavior(pos1, pos1 + CPos(0, ToGamePosY(1.2)), 3.2, 10);
				e1->SetBehaviorComponent(move);
				e1->SetMovingShot();
				CBattleScene::m_enemyManager.Add(e1);
			}
		}
	}
}

int SpawnerS01::GetPower(){	return 1; }




SPAWNECPP(SpawnerM02)
void SpawnerM02::Spawne()
{
	if (m_count == 0) {
		m_maxCount = ToSecond(2.5);
		m_spawneTiming = ToSecond(0.4);
		iVal[0] = CFunc::RandI(0, 1); // 出現位置
		bVal[0] = true; // init
	}
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	// 左上から１体ずつ4帯
	if (iVal[0] == 0 || 1) {
		double x;
		if (bVal[0]) {
			x = dVal[0] = -0.1; // 出現X座標
			bVal[0] = false; // init
		}

		if (m_count % m_spawneTiming == 0) {
			x = ToGamePosX(dVal[0]);
			dVal[0] += 0.2;
		
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

int SpawnerM02::GetPower() { return 1; }

