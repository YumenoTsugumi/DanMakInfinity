#include "StageManager.h"
#include "BattleScene.h"


#include "EnemyS.h"
#include "EnemyM.h"

#include "Spawner.h"

const int m_maxSpawneCount = 12;



// 帰り道共通パターン
SpawnerMoveStopMove::SpawnerMoveStopMove(EnemySize spawnerSize) : SpawnerBase(spawnerSize){
	m_returnAngle = CFunc::RandD(180, 360);
	m_returnPettern = CFunc::RandI(0, 3);
	m_random = CFunc::RandI(0, 1);

	m_returnPettern = CFunc::RandI(2, 3);
}
SpawnerMoveStopMove::~SpawnerMoveStopMove() {}

CInOutBehavior* SpawnerMoveStopMove::GetReturnPettern(int spawnerIndex,const CPos& pos, const CPos& targetPos) {
	CInOutBehavior* move = nullptr;
	CPos returnPos;
	if (m_returnPettern == 0) { // 全員時期狙い
		move = new CInOutBehavior(pos, targetPos, pos, 7, 8, 180);
		move->SetLeaveDirToPlayer();
	}
	else if (m_returnPettern == 1) { // 全員ランダムな方向に
		
		double returnAngle = m_returnAngle + CFunc::RandD(-5, 5);
		returnPos.x = targetPos.x + cos(returnAngle / CFunc::RAD) * 1920;
		returnPos.y = targetPos.y + sin(returnAngle / CFunc::RAD) * 1920;
		move = new CInOutBehavior(pos, targetPos, returnPos, 7, 9, 180);
	}
	else if (m_returnPettern == 2) { // インデックスが若い半分は左に　若くない半分は右に
		double returnAngle = 0;
		if (spawnerIndex < m_maxSpawneCount / 2) {
			if (m_random == 0) {
				returnAngle = 180.0;
			} else if(m_random == 1) {
				returnAngle = 180.0 + 45;
			}
		} else {
			if (m_random == 0) {
				returnAngle = 0.0;
			}else if (m_random == 1) {
				returnAngle = 0.0 - 45;
			}
		}
		returnPos.x = targetPos.x + cos(returnAngle / CFunc::RAD) * 1920;
		returnPos.y = targetPos.y + sin(returnAngle / CFunc::RAD) * 1920;
		move = new CInOutBehavior(pos, targetPos, returnPos, 7, 9, 180);
	}
	else if (m_returnPettern == 3) { // インデックスが偶数　奇数で
		double returnAngle = 0;
		if (spawnerIndex % 2 == 0) {
			if (m_random == 0) {
				returnAngle = 180.0;
			}else if (m_random == 1) {
				returnAngle = 180.0 + 45;
			}
		}else {
			if (m_random == 0) {
				returnAngle = 0.0;
			}else if (m_random == 1) {
				returnAngle = 0.0 - 45;
			}
		}
		returnPos.x = targetPos.x + cos(returnAngle / CFunc::RAD) * 1920;
		returnPos.y = targetPos.y + sin(returnAngle / CFunc::RAD) * 1920;
		move = new CInOutBehavior(pos, targetPos, returnPos, 7, 9, 180);
	}

	return move;
}

//-----------------------------------------------------------------------
// 上から真ん中まで動いて停止　その後撤退 

SpawnerSmallTop_Stop::SpawnerSmallTop_Stop(EnemySize spawnerSize) : SpawnerMoveStopMove(spawnerSize)
{
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;
	m_appearancePosition = CFunc::RandD(-0.15, +0.15); // 出現位置の誤差
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
		CBaseEnemy* enemy = GetEnemy(pos);

		CPos targetPos = pos + CPos(0, goalAddY);
		
		CInOutBehavior* move = GetReturnPettern(m_spawnerIndex ,pos, targetPos);

		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);
	}
}

//-----------------------------------------------------------------------
// 左右から真ん中まで動いて停止　その後撤退 
SpawnerSmallLeftRight_Stop::SpawnerSmallLeftRight_Stop(EnemySize spawnerSize) : SpawnerMoveStopMove(spawnerSize)
{
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;
	m_LRPettern = (StartLRPos)CFunc::RandI(0, 1); // 出現位置パターン
	if (m_LRPettern == StartLRPos::Left) {
		m_spawnerTargetPosX = CFunc::RandD(0.35, 0.9);
	}
	else {
		m_spawnerTargetPosX = CFunc::RandD(0.1, 0.65);
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
		if (m_LRPettern == StartLRPos::Left) {
			x = ToGamePosX(CFunc::RandD(-0.1, -0.2));
			tartgetAddX = ToGamePosX(m_spawnerTargetPosX + CFunc::RandD(-0.15, +0.15));
			goalAddX = ToGamePosX(1.40);
		}
		else if (m_LRPettern == StartLRPos::Right) {
			x = ToGamePosX(CFunc::RandD(1.1, 1.2));
			tartgetAddX = ToGamePosX(m_spawnerTargetPosX + CFunc::RandD(-0.15, +0.15));
			goalAddX = ToGamePosX(-0.4);
		}
		double y1 = ToGamePosY(CFunc::RandD(0.1, 0.4));
		double y2 = ToGamePosY(CFunc::RandD(0.1, 0.3));
		CPos pos(x, y1);
		CBaseEnemy* enemy = GetEnemy(pos);

		CPos targetPos = CPos(tartgetAddX, y2);

		CInOutBehavior* move = GetReturnPettern(m_spawnerIndex, pos, targetPos);

		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);
	}
}



//------------------------------------------------------------------------------
// 上から真ん中まで動いて停止　その後撤退 (Line
SpawnerSmall_Line_Top_Stop::SpawnerSmall_Line_Top_Stop(EnemySize spawnerSize) : SpawnerMoveStopMove(spawnerSize)
{
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;

	m_spawnerIndex = 0;
	/*	m_inPettern
	0	左上から右上	123456789
	1	右上から左上	987654321
	2	0の2重		56789	出現位置は0.0～1.0
					01234
	3				98765
					43210
	*/
	m_inPettern = CFunc::RandI(0, 3);
	m_inPettern = 2;

	for (int ii = 0; ii < m_maxSpawneCount; ii++) {
		if (m_inPettern == 0) {
			m_inPos.push_back(CPos(ToGamePosX(0.1 + 0.8 * ((float)ii / m_maxSpawneCount)), ToGamePosY(-0.1)));
		}else if (m_inPettern == 1) {
			m_inPos.push_back(CPos(ToGamePosX(0.9 - 0.8 * ((float)ii / m_maxSpawneCount)), ToGamePosY(-0.1)));
		}else if (m_inPettern == 2) {
			m_appearancePosition = CFunc::RandD(0.1, 0.6); // パターン2,3の出現位置の誤差
			if (ii < m_maxSpawneCount / 2) {
				m_inPos.push_back(CPos(ToGamePosX(0.0 + 0.05 * ii), ToGamePosY(-0.1)));
			}else {
				m_inPos.push_back(CPos(ToGamePosX(0.0 + 0.05 * (ii-(m_maxSpawneCount/2))), ToGamePosY(-0.1)));
			}
		}else if (m_inPettern == 3) {
			m_appearancePosition = CFunc::RandD(0.4, 0.9); // パターン2,3の出現位置の誤差
			if (ii < m_maxSpawneCount / 2) {
				m_inPos.push_back(CPos(ToGamePosX(0.0 - 0.05 * ii), ToGamePosY(-0.1)));
			}else {
				m_inPos.push_back(CPos(ToGamePosX(0.0 - 0.05 * (ii - (m_maxSpawneCount / 2))), ToGamePosY(-0.1)));
			}
		}
	}

	/*	m_spawnerYPettern
	0	Yランダム	1	Y固定	2	Y高い順	3	Y低い順	*/
	m_spawnerYPettern = CFunc::RandI(0, 3);
	m_spawnerYPettern = 1;
	if (m_spawnerYPettern == 1) {
		m_goalY = CFunc::RandD(0.2, 0.5);
	}
	else if (m_spawnerYPettern == 2 || m_spawnerYPettern == 3) {
		m_goalY = CFunc::RandD(0.05, 0.25);
	}
}

SpawnerSmall_Line_Top_Stop::~SpawnerSmall_Line_Top_Stop() {}

void SpawnerSmall_Line_Top_Stop::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_inPettern >= 0 && m_inPettern <= 9) {
		if (m_count % m_spawneTiming == 0) {
			CPos pos = m_inPos[m_spawnerIndex];
			
			if (m_inPettern == 2 || m_inPettern == 3) {
				pos.x += ToGameSizeX(m_appearancePosition);
			}
			CBaseEnemy* enemy = GetEnemy(pos);

			double goalY = 0.0;
			if (m_spawnerYPettern == 0) {
				goalY = ToGamePosY(CFunc::RandD(0.2, 0.5)); // 3	Yランダム
			}
			else if (m_spawnerYPettern == 1) {
				if (m_inPettern == 0 || m_inPettern == 1) {
					goalY = ToGamePosY(m_goalY);
				}
				else {
					if (m_spawnerIndex < m_maxSpawneCount / 2) {
						goalY = ToGamePosY(m_goalY);
					}
					else {
						goalY = ToGamePosY(m_goalY-0.1);
					}
				}
			}
			else {
				if (m_inPettern == 0 || m_inPettern == 1) {
					if (m_spawnerYPettern == 2) {
						goalY = ToGamePosY(0.1 + 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY); // 2	Y高い順
					}
					else if (m_spawnerYPettern == 3) {
						goalY = ToGamePosY(0.45 - 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY); // 3	Y低い順
					}
				}
				// 2重編隊だとY座標を調整しないといけない
				else if (m_spawnerYPettern == 2) {
						if (m_spawnerIndex < m_maxSpawneCount / 2) {
							goalY = ToGamePosY(0.1 + 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY); // 2	Y高い順
						} else {
							goalY = ToGamePosY(0.1 + 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY - 0.07); // 2	Y高い順
						}
				} 
				else if (m_spawnerYPettern == 3) {
					if (m_spawnerIndex < m_maxSpawneCount / 2) {
						goalY = ToGamePosY(0.45 - 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY); // 3	Y低い順
					}
					else {
						goalY = ToGamePosY(0.45 - 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY + 0.07); // 3	Y低い順
					}
				}
			}

			CPos targetPos = pos + CPos(0, goalY);

			CInOutBehavior* move = GetReturnPettern(m_spawnerIndex, pos, targetPos);

			enemy->SetBehaviorComponent(move);
			CBattleScene::m_enemyManager.Add(enemy);

			m_spawnerIndex++;
		}
	}
}

//---------------------------------------------------------------------------
// 左右から真ん中まで動いて停止　その後撤退 (Line
SpawnerSmall_Line_LeftRight_Stop::SpawnerSmall_Line_LeftRight_Stop(EnemySize spawnerSize) : SpawnerMoveStopMove(spawnerSize)
{
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;
	m_LRPettern = (StartLRPos)CFunc::RandI(0, 1); // 出現位置パターン

	/*	m_inPettern

	0	2重			01
					23
					45
					67
					89	出現位置は0.0～0.5
	1	0のXがズレる版
	*/
	m_inPettern = CFunc::RandI(0, 1);
	double rrr = CFunc::RandD(0.0, 0.2);
	if (m_inPettern == 0) {
		if (m_LRPettern == StartLRPos::Left) {
			m_goalX = CFunc::RandD(0.3, 0.8);
		}
		else {
			m_goalX = CFunc::RandD(0.7, 0.2);
		}
	} else if (m_inPettern == 1) {
		m_haba = CFunc::RandD(0.15, 0.85);
	}

	for (int ii = 0; ii < m_maxSpawneCount; ii++) {
		bool firstHalf = true; // 前半部隊
		if (ii >= m_maxSpawneCount / 2) 	firstHalf = false;

		double YY;
		if (firstHalf)	YY = ToGamePosY(0.05 + 0.3 * (float)ii / (m_maxSpawneCount/2));
		else			YY = ToGamePosY(0.05 + 0.3 * ((float)(ii - m_maxSpawneCount/2) / (m_maxSpawneCount/2)));

		if (m_inPettern == 0) {
			if (m_LRPettern == StartLRPos::Left) {
				m_inPos.push_back(CPos(ToGamePosX(-0.1), YY));
				if(firstHalf)	m_targetPos.push_back(CPos(ToGamePosX(m_goalX), YY));
				else			m_targetPos.push_back(CPos(ToGamePosX(m_goalX - 0.1), YY));
			}else {
				m_inPos.push_back(CPos(ToGamePosX(1.1),	YY));
				if (firstHalf)	m_targetPos.push_back(CPos(ToGamePosX(m_goalX), YY));
				else			m_targetPos.push_back(CPos(ToGamePosX(m_goalX + 0.1), YY));
			}
		}
		else if (m_inPettern == 1) {
			if (m_LRPettern == StartLRPos::Left) {
				m_inPos.push_back(CPos(ToGamePosX(-0.1), YY));
				if (firstHalf)	m_targetPos.push_back(CPos(ToGamePosX(0.85 - m_haba * (float)ii / m_maxSpawneCount) + rrr,	YY));
				else			m_targetPos.push_back(CPos(ToGamePosX(0.85 - m_haba * (float)ii / m_maxSpawneCount - 0.1) + rrr, YY));
			}
			else {
				m_inPos.push_back(CPos(ToGamePosX(1.1),	YY));
				if (firstHalf)	m_targetPos.push_back(CPos(ToGamePosX(0.15 + m_haba * (float)ii / m_maxSpawneCount) + rrr,	YY));
				else			m_targetPos.push_back(CPos(ToGamePosX(0.15 + m_haba * (float)ii / m_maxSpawneCount + 0.1) + rrr, YY));
			}
		}
	}
}

SpawnerSmall_Line_LeftRight_Stop::~SpawnerSmall_Line_LeftRight_Stop() {}

void SpawnerSmall_Line_LeftRight_Stop::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_inPettern >= 0 && m_inPettern <= 9) {
		if (m_count % m_spawneTiming == 0) {
			CPos pos = m_inPos[m_spawnerIndex];
			CPos targetPos = m_targetPos[m_spawnerIndex];

			CBaseEnemy* enemy = GetEnemy(pos);
			CInOutBehavior* move = GetReturnPettern(m_spawnerIndex, pos, targetPos);

			enemy->SetBehaviorComponent(move);
			CBattleScene::m_enemyManager.Add(enemy);

			m_spawnerIndex++;
		}
	}
}




//------------------------------------------------------------------------
// ノンストップゆっくり上から下に

//SpawnerSmallTop_NoStop::SpawnerSmallTop_NoStop(EnemySize spawnerSize) : SpawnerBase(spawnerSize)
//{
//	m_maxCount = ToSecond(FormationSpawneFinishTiming);
//	m_spawneCount = SpawneCount;
//	m_spawneTiming = m_maxCount / m_spawneCount;
//	m_appearancePosition = CFunc::RandD(-0.15, +0.15); // 出現位置の誤差
//
//	m_index = GetSmallEnemyIndex();
//}
//
//SpawnerSmallTop_NoStop::~SpawnerSmallTop_NoStop() {}
//
//void SpawnerSmallTop_NoStop::Spawne()
//{
//	if (m_count >= m_maxCount) {
//		m_deleteFlg = true;
//		return;
//	}
//	m_count++;
//
//	if (m_count % m_spawneTiming == 0) {
//
//		double x = ToGamePosX(CFunc::RandD(0.20, 0.80) + m_appearancePosition);
//		double y = ToGamePosY(-0.1);
//		double goalAddY = ToGamePosY(1.20);
//		CPos pos(x, y);
//		CBaseEnemy* enemy = GetSmallEnemy(m_index, pos);
//		CPos targetPos = pos + CPos(0, goalAddY);
//
//		CGoTargetBehavior* move = new CGoTargetBehavior(pos, targetPos, 1.15, 999);
//
//		enemy->SetBehaviorComponent(move);
//		enemy->SetMovingShot();
//		CBattleScene::m_enemyManager.Add(enemy);
//	}
//}




//------------------------------------------------------------------------------
//
//CBaseEnemy* SpawnerMedium::GetEnemy(const CPos& pos)
//{
//	return new CEnemyS01(pos);
//}
//
//
//SpawnerMedium::SpawnerMedium(EnemySize spawnerSize) : SpawnerBase(spawnerSize) {
//	m_maxCount = ToSecond(2.5);
//	m_spawneTiming = ToSecond(0.4);
//	spawnerPettern = CFunc::RandI(0, 1); // 出現位置
//
//	m_appearancePosition = -0.1;
//}
//
//SpawnerMedium::~SpawnerMedium() {
//
//}
//void SpawnerMedium::Spawne()
//{
//
//	if (m_count >= m_maxCount) {
//		m_deleteFlg = true;
//		return;
//	}
//	m_count++;
//
//	// 左上から１体ずつ4帯
//	if (spawnerPettern == 0 || 1) {
//		double x;
//
//		if (m_count % m_spawneTiming == 0) {
//			x = ToGamePosX(m_appearancePosition);
//			m_appearancePosition += 0.2;
//		
//			double y = ToGamePosY(-0.1);
//			CPos pos1(x, y);
//			CPos pos2(x, y + ToGamePosY(1.20));
//			CEnemyM02* e2 = new CEnemyM02(CPos(x, y));
//			CGoTargetBehavior* move = new CGoTargetBehavior(pos1, pos2, 1.35, 0);
//
//			e2->SetBehaviorComponent(move);
//
//			CBattleScene::m_enemyManager.Add(e2);
//		}
//	}
//	//if (m_count % m_spawneTiming == 0) {
//	//	double x;
//	//	if (iVal[0] == 0) {
//	//		x = ToGamePosX(CFunc::RandD(0.05, 0.4));
//	//	}
//	//	else if (iVal[0] == 1) {
//	//		x = ToGamePosX(CFunc::RandD(0.3, 0.7));
//	//	}
//	//	else if (iVal[0] == 2) {
//	//		x = ToGamePosX(CFunc::RandD(0.6, 0.95));
//	//	}
//
//	//	double y = ToGamePosY(-0.1);
//	//	double goalAddY = ToGamePosY(0.20) + ToGamePosY(CFunc::RandD(0.05, 0.4));
//	//	CPos pos1(x, y);
//	//	CEnemyM02* e1 = new CEnemyM02(pos1);
//	//	CInOutBehavior* move = new CInOutBehavior(pos1, pos1 + CPos(0, goalAddY), 7, 10, 180);
//	//	e1->SetBehaviorComponent(move);
//
//	//	CBattleScene::m_enemyManager.Add(e1);
//	//}
//}
//
//
