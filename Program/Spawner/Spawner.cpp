#include "StageManager.h"
#include "BattleScene.h"


#include "EnemyS.h"
#include "EnemyM.h"

#include "Spawner.h"

const int m_maxSpawneCount = 12;

const int FormationSpawneFinishTiming = 2; // 出現する時間 2秒でで12体スポーンさせる

// 帰り道共通パターン
SpawnerMoveStopMove::SpawnerMoveStopMove(EnemySize spawnerSize) : SpawnerBase(spawnerSize){
	m_returnAngle = CFunc::RandD(180, 360);
	if (spawnerSize == EnemySize::Small) {
		m_returnPettern = CFunc::RandI(0, 3);
	}
	else {
		m_returnPettern = CFunc::RandI(1, 3); // 時期狙いは無し
	}
	m_random = CFunc::RandI(0, 1);
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
	SetStopSpawner(spawnerSize);
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

		double x = CGame::ToGamePosX(CFunc::RandD(0.20, 0.70) + m_appearancePosition);
		double y = CGame::ToGamePosY(-0.1);
		double goalAddY = CGame::ToGamePosY(0.20) + CGame::ToGamePosY(CFunc::RandD(0.05, 0.4));
		CPos pos(x, y);
		CBaseEnemy* enemy = GetStopEnemy(pos);

		CPos targetPos = pos + CPos(0, goalAddY);
		
		CInOutBehavior* move = GetReturnPettern(m_spawnerIndex ,pos, targetPos);
		enemy->SetBehaviorComponent(move);

		CBattleScene::m_enemyManager.Add(enemy);
		m_spawnerIndex++;
	}
}

//-----------------------------------------------------------------------
// 左右から真ん中まで動いて停止　その後撤退 
SpawnerSmallLeftRight_Stop::SpawnerSmallLeftRight_Stop(EnemySize spawnerSize) : SpawnerMoveStopMove(spawnerSize)
{
	SetStopSpawner(spawnerSize);
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
			x = CGame::ToGamePosX(CFunc::RandD(-0.1, -0.2));
			tartgetAddX = CGame::ToGamePosX(m_spawnerTargetPosX + CFunc::RandD(-0.15, +0.15));
			goalAddX = CGame::ToGamePosX(1.40);
		}
		else if (m_LRPettern == StartLRPos::Right) {
			x = CGame::ToGamePosX(CFunc::RandD(1.1, 1.2));
			tartgetAddX = CGame::ToGamePosX(m_spawnerTargetPosX + CFunc::RandD(-0.15, +0.15));
			goalAddX = CGame::ToGamePosX(-0.4);
		}
		double y1 = CGame::ToGamePosY(CFunc::RandD(0.1, 0.4));
		double y2 = CGame::ToGamePosY(CFunc::RandD(0.1, 0.3));
		CPos pos(x, y1);
		CBaseEnemy* enemy = GetStopEnemy(pos);

		CPos targetPos = CPos(tartgetAddX, y2);

		CInOutBehavior* move = GetReturnPettern(m_spawnerIndex, pos, targetPos);

		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);
		m_spawnerIndex++;
	}
}



//------------------------------------------------------------------------------
// 上から真ん中まで動いて停止　その後撤退 (Line
SpawnerSmall_Line_Top_Stop::SpawnerSmall_Line_Top_Stop(EnemySize spawnerSize) : SpawnerMoveStopMove(spawnerSize)
{
	SetStopSpawner(spawnerSize);
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

	for (int ii = 0; ii < m_maxSpawneCount; ii++) {
		if (m_inPettern == 0) {
			m_inPos.push_back(CPos(CGame::ToGamePosX(0.1 + 0.8 * ((float)ii / m_maxSpawneCount)), CGame::ToGamePosY(-0.1)));
		}else if (m_inPettern == 1) {
			m_inPos.push_back(CPos(CGame::ToGamePosX(0.9 - 0.8 * ((float)ii / m_maxSpawneCount)), CGame::ToGamePosY(-0.1)));
		}else if (m_inPettern == 2) {
			m_appearancePosition = CFunc::RandD(0.1, 0.6); // パターン2,3の出現位置の誤差
			if (ii < m_maxSpawneCount / 2) {
				m_inPos.push_back(CPos(CGame::ToGamePosX(0.0 + 0.05 * ii), CGame::ToGamePosY(-0.1)));
			}else {
				m_inPos.push_back(CPos(CGame::ToGamePosX(0.0 + 0.05 * (ii-(m_maxSpawneCount/2))), CGame::ToGamePosY(-0.1)));
			}
		}else if (m_inPettern == 3) {
			m_appearancePosition = CFunc::RandD(0.4, 0.9); // パターン2,3の出現位置の誤差
			if (ii < m_maxSpawneCount / 2) {
				m_inPos.push_back(CPos(CGame::ToGamePosX(0.0 - 0.05 * ii), CGame::ToGamePosY(-0.1)));
			}else {
				m_inPos.push_back(CPos(CGame::ToGamePosX(0.0 - 0.05 * (ii - (m_maxSpawneCount / 2))), CGame::ToGamePosY(-0.1)));
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
				pos.x += CGame::ToGameSizeX(m_appearancePosition);
			}
			CBaseEnemy* enemy = GetStopEnemy(pos);

			double goalY = 0.0;
			if (m_spawnerYPettern == 0) {
				goalY = CGame::ToGamePosY(CFunc::RandD(0.2, 0.5)); // 3	Yランダム
			}
			else if (m_spawnerYPettern == 1) {
				if (m_inPettern == 0 || m_inPettern == 1) {
					goalY = CGame::ToGamePosY(m_goalY);
				}
				else {
					if (m_spawnerIndex < m_maxSpawneCount / 2) {
						goalY = CGame::ToGamePosY(m_goalY);
					}
					else {
						goalY = CGame::ToGamePosY(m_goalY-0.1);
					}
				}
			}
			else {
				if (m_inPettern == 0 || m_inPettern == 1) {
					if (m_spawnerYPettern == 2) {
						goalY = CGame::ToGamePosY(0.1 + 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY); // 2	Y高い順
					}
					else if (m_spawnerYPettern == 3) {
						goalY = CGame::ToGamePosY(0.45 - 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY); // 3	Y低い順
					}
				}
				// 2重編隊だとY座標を調整しないといけない
				else if (m_spawnerYPettern == 2) {
						if (m_spawnerIndex < m_maxSpawneCount / 2) {
							goalY = CGame::ToGamePosY(0.1 + 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY); // 2	Y高い順
						} else {
							goalY = CGame::ToGamePosY(0.1 + 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY - 0.07); // 2	Y高い順
						}
				} 
				else if (m_spawnerYPettern == 3) {
					if (m_spawnerIndex < m_maxSpawneCount / 2) {
						goalY = CGame::ToGamePosY(0.45 - 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY); // 3	Y低い順
					}
					else {
						goalY = CGame::ToGamePosY(0.45 - 0.35 * ((float)m_spawnerIndex / m_maxSpawneCount) + m_goalY + 0.07); // 3	Y低い順
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
	SetStopSpawner(spawnerSize);
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
		if (firstHalf)	YY = CGame::ToGamePosY(0.05 + 0.3 * (float)ii / (m_maxSpawneCount/2));
		else			YY = CGame::ToGamePosY(0.05 + 0.3 * ((float)(ii - m_maxSpawneCount/2) / (m_maxSpawneCount/2)));

		if (m_inPettern == 0) {
			if (m_LRPettern == StartLRPos::Left) {
				m_inPos.push_back(CPos(CGame::ToGamePosX(-0.1), YY));
				if(firstHalf)	m_targetPos.push_back(CPos(CGame::ToGamePosX(m_goalX), YY));
				else			m_targetPos.push_back(CPos(CGame::ToGamePosX(m_goalX - 0.1), YY));
			}else {
				m_inPos.push_back(CPos(CGame::ToGamePosX(1.1),	YY));
				if (firstHalf)	m_targetPos.push_back(CPos(CGame::ToGamePosX(m_goalX), YY));
				else			m_targetPos.push_back(CPos(CGame::ToGamePosX(m_goalX + 0.1), YY));
			}
		}
		else if (m_inPettern == 1) {
			if (m_LRPettern == StartLRPos::Left) {
				m_inPos.push_back(CPos(CGame::ToGamePosX(-0.1), YY));
				if (firstHalf)	m_targetPos.push_back(CPos(CGame::ToGamePosX(0.85 - m_haba * (float)ii / m_maxSpawneCount) + rrr,	YY));
				else			m_targetPos.push_back(CPos(CGame::ToGamePosX(0.85 - m_haba * (float)ii / m_maxSpawneCount - 0.1) + rrr, YY));
			}
			else {
				m_inPos.push_back(CPos(CGame::ToGamePosX(1.1),	YY));
				if (firstHalf)	m_targetPos.push_back(CPos(CGame::ToGamePosX(0.15 + m_haba * (float)ii / m_maxSpawneCount) + rrr,	YY));
				else			m_targetPos.push_back(CPos(CGame::ToGamePosX(0.15 + m_haba * (float)ii / m_maxSpawneCount + 0.1) + rrr, YY));
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

	if (m_count % m_spawneTiming == 0) {
		CPos pos = m_inPos[m_spawnerIndex];
		CPos targetPos = m_targetPos[m_spawnerIndex];

		CBaseEnemy* enemy = GetStopEnemy(pos);
		CInOutBehavior* move = GetReturnPettern(m_spawnerIndex, pos, targetPos);

		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);

		m_spawnerIndex++;
	}

}

// ------------------------------------------------------------------------------------------------------------
// 止まらず動き続けるパターン
// ------------------------------------------------------------------------------------------------------------
SpawnerSmallTop_NoStop_Uturn::SpawnerSmallTop_NoStop_Uturn(EnemySize spawnerSize) : SpawnerBase(spawnerSize)
{
	SetNonStopSpawner(spawnerSize);
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;
	m_spawnerPosition = (StartLRPos)CFunc::RandI(0, 2); // 出現位置パターン

	if (m_spawnerPosition == StartLRPos::Left ||
		m_spawnerPosition == StartLRPos::Right) {

		double stY = 0.05 + CFunc::RandD(0, 0.08);
		double edY = 0.4 + CFunc::RandD(0, 0.08);
		//if (CFunc::RandI(0, 1)) {
		//	std::swap(stY, edY);
		//}
		if (m_spawnerPosition == StartLRPos::Left) {
			m_posAry = std::vector<CPos>{
				CPos(CGame::ToGamePosX(-0.2), CGame::ToGamePosY(stY)),
				CPos(CGame::ToGamePosX(1.4),  CGame::ToGamePosY(stY)),
				CPos(CGame::ToGamePosX(1.4),  CGame::ToGamePosY(edY)),
				CPos(CGame::ToGamePosX(-0.2), CGame::ToGamePosY(edY)) };
		}
		else if (m_spawnerPosition == StartLRPos::Right) {
			m_posAry = std::vector<CPos>{
				CPos(CGame::ToGamePosX(1.2),  CGame::ToGamePosY(stY)),
				CPos(CGame::ToGamePosX(-0.4), CGame::ToGamePosY(stY)),
				CPos(CGame::ToGamePosX(-0.4), CGame::ToGamePosY(edY)),
				CPos(CGame::ToGamePosX(1.2),  CGame::ToGamePosY(edY)) };
		}
	} else if (m_spawnerPosition == StartLRPos::Top) {
		double stX = CFunc::RandD(0.1, 0.3);
		double edX = CFunc::RandD(0.7, 0.9);
		if (CFunc::RandI(0, 1)) {
			std::swap(stX, edX);
		}
		double YY = CFunc::RandD(0.5, 0.7);
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(stX), CGame::ToGamePosY(-0.1)),
			CPos(CGame::ToGamePosX(stX), CGame::ToGamePosY(YY)),
			CPos(CGame::ToGamePosX(edX), CGame::ToGamePosY(YY)),
			CPos(CGame::ToGamePosX(edX), CGame::ToGamePosY(-0.1)) };
	}
}

SpawnerSmallTop_NoStop_Uturn::~SpawnerSmallTop_NoStop_Uturn() {}

void SpawnerSmallTop_NoStop_Uturn::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		CBaseEnemy* enemy = GetNonStopEnemy(m_posAry[0]); // bazieなので0
		CBezierBehavior* move = new CBezierBehavior(m_posAry, m_speed);
		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);
		m_spawnerIndex++;
	}
}



SpawnerSmallTop_NoStop_LRTurn::SpawnerSmallTop_NoStop_LRTurn(EnemySize spawnerSize) : SpawnerBase(spawnerSize)
{
	SetNonStopSpawner(spawnerSize);
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;

	// 画面上から出てきて、左にいく
	// 画面上から出てきて、右にいく
	m_spawnerPosition = (StartLRPos)CFunc::RandI(2, 3);

	// 0と1は画面の下の方から出てくるのはナンセンスなのでやめた
	if (m_spawnerPosition == 0) {
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(-0.2), CGame::ToGamePosY(0.4)),
			CPos(CGame::ToGamePosX(1.0), CGame::ToGamePosY(0.5)),
			CPos(CGame::ToGamePosX(1.0), CGame::ToGamePosY(0.0)),
			CPos(CGame::ToGamePosX(1.0), CGame::ToGamePosY(-0.3)) };
	} else if (m_spawnerPosition == 1) {
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(1.2), CGame::ToGamePosY(0.4)),
			CPos(CGame::ToGamePosX(0.0), CGame::ToGamePosY(0.5)),
			CPos(CGame::ToGamePosX(0.0), CGame::ToGamePosY(0.0)),
			CPos(CGame::ToGamePosX(0.0), CGame::ToGamePosY(-0.3)) };
	} else if (m_spawnerPosition == 2) {
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(1.0), CGame::ToGamePosY(-0.3)),
			CPos(CGame::ToGamePosX(1.0), CGame::ToGamePosY(0.0)),
			CPos(CGame::ToGamePosX(1.0), CGame::ToGamePosY(0.5)),
			CPos(CGame::ToGamePosX(-0.2), CGame::ToGamePosY(0.4)) };
	} else if (m_spawnerPosition == 3) {
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(0.0), CGame::ToGamePosY(-0.3)),
			CPos(CGame::ToGamePosX(0.0), CGame::ToGamePosY(0.0)),
			CPos(CGame::ToGamePosX(0.0), CGame::ToGamePosY(0.5)),
			CPos(CGame::ToGamePosX(1.2), CGame::ToGamePosY(0.4)) };
	}
	for (CPos& p : m_posAry) {
		p.x += CGame::ToGameSizeX(CFunc::RandD(-0.1, 0.1));
		p.y += CGame::ToGameSizeY(CFunc::RandD(-0.1, 0.1));
	}

}

SpawnerSmallTop_NoStop_LRTurn::~SpawnerSmallTop_NoStop_LRTurn() {}

void SpawnerSmallTop_NoStop_LRTurn::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		CBaseEnemy* enemy = GetNonStopEnemy(m_posAry[0]); // bazieなので0
		
		CBezierBehavior* move = new CBezierBehavior(m_posAry, m_speed);
		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);

		m_spawnerIndex++;
	}
}


SpawnerSmallTop_NoStop_LRCos::SpawnerSmallTop_NoStop_LRCos(EnemySize spawnerSize) : SpawnerBase(spawnerSize)
{
	SetNonStopSpawner(spawnerSize);
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;

	// 画面左から　Z
	// 画面右から　S
	// 画面左から　~
	// 画面右から　N
	m_spawnerPosition = (StartLRPos)CFunc::RandI(0, 3);
	if (m_spawnerPosition == 0) {
		double posX = CFunc::RandD(0.3, 0.4);
		double posY = CFunc::RandD(0.3, 0.5);
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(-0.2),CGame::ToGamePosY(0.0 + CFunc::RandD(0,0.4))),
			CPos(CGame::ToGamePosX(1.2 + CFunc::RandD(0,0.1)), CGame::ToGamePosY(0.1 + CFunc::RandD(0,0.2))),
			CPos(CGame::ToGamePosX(1.2), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(posY)),

			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(-0.1), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(-0.1 + CFunc::RandD(0,0.1)), CGame::ToGamePosY(posY + CFunc::RandD(0.1,0.2))),
			CPos(CGame::ToGamePosX(1.2), CGame::ToGamePosY(0.3 + CFunc::RandD(0,0.3)))
		};
	}
	else if (m_spawnerPosition == 1) {
		double posX = CFunc::RandD(0.7, 0.8);
		double posY = CFunc::RandD(0.3, 0.5);
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(1.2),CGame::ToGamePosY(0.0 + CFunc::RandD(0,0.2))),
			CPos(CGame::ToGamePosX(-0.2 + CFunc::RandD(0,0.1)), CGame::ToGamePosY(0.1 + CFunc::RandD(0,0.2))),
			CPos(CGame::ToGamePosX(-0.2), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(posY)),

			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(1.1), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(1.1 + -CFunc::RandD(0,0.1)), CGame::ToGamePosY(posY + CFunc::RandD(0.1,0.2))),
			CPos(CGame::ToGamePosX(-0.2), CGame::ToGamePosY(0.3 + CFunc::RandD(0,0.3)))
		};
	}
	else if (m_spawnerPosition == 2) {
		double posX = CFunc::RandD(0.4, 0.6);
		double posY = CFunc::RandD(0.2, 0.4);
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(-0.2),CGame::ToGamePosY(-0.0)),
			CPos(CGame::ToGamePosX(0.1), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(0.1)),

			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(0.1)),
			CPos(CGame::ToGamePosX(posX + 0.1), CGame::ToGamePosY(-0.2)),
			CPos(CGame::ToGamePosX(posX+ CFunc::RandD(0.2, 0.4)), CGame::ToGamePosY(posY+0.1)),
			CPos(CGame::ToGamePosX(1.2), CGame::ToGamePosY(0.3))
		};
	}
	else if (m_spawnerPosition == 3) {
		double posX = CFunc::RandD(0.4, 0.6);
		double posY = CFunc::RandD(0.2, 0.4);
		m_posAry = std::vector<CPos>{
			CPos(CGame::ToGamePosX(1.2),CGame::ToGamePosY(-0.0)),
			CPos(CGame::ToGamePosX(0.9), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(posY)),
			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(0.1)),

			CPos(CGame::ToGamePosX(posX), CGame::ToGamePosY(0.1)),
			CPos(CGame::ToGamePosX(posX - 0.1), CGame::ToGamePosY(-0.2)),
			CPos(CGame::ToGamePosX(posX - CFunc::RandD(0.2, 0.4)), CGame::ToGamePosY(posY + 0.1)),
			CPos(CGame::ToGamePosX(-0.2), CGame::ToGamePosY(0.3))
		};
	}

	if (m_spawnerSize == EnemySize::Small) {
		m_speed = CFunc::RandD(4.0, 5.5);
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		m_speed = CFunc::RandD(1.0, 2.5);
	}
}

SpawnerSmallTop_NoStop_LRCos::~SpawnerSmallTop_NoStop_LRCos() {}

void SpawnerSmallTop_NoStop_LRCos::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		CBaseEnemy* enemy = GetNonStopEnemy(m_posAry[0]);
		CBezierBehavior* move = new CBezierBehavior(m_posAry, m_speed);
		enemy->SetBehaviorComponent(move);
		CBattleScene::m_enemyManager.Add(enemy);
		m_spawnerIndex++;
	}
}

//-----------------------------------------------------------------------------
// ずっとプレイヤーを時期狙い
//-----------------------------------------------------------------------------
SpawnerSmallTop_TracePlayer::SpawnerSmallTop_TracePlayer(EnemySize spawnerSize) : SpawnerBase(spawnerSize)
{
	SetNonStopSpawner(spawnerSize);
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;

	int rand = CFunc::RandI(0, 2);

	if (rand == 0) {
		double x = CGame::ToGamePosX(CFunc::RandD(0.1,0.9));
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			m_posAry.push_back(CPos(x, CGame::ToGamePosY(-0.1)));
		}
	}
	else if (rand == 1) {
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(0.1 + ((double)0.8 / m_maxSpawneCount) * ii);
			m_posAry.push_back(CPos(x, CGame::ToGamePosY(-0.1)));
		}
	}
	else if (rand == 2) {
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(0.9 - ((double)0.8 / m_maxSpawneCount) * ii);
			m_posAry.push_back(CPos(x, CGame::ToGamePosY(-0.1)));
		}
	}

}

SpawnerSmallTop_TracePlayer::~SpawnerSmallTop_TracePlayer(){}

void SpawnerSmallTop_TracePlayer::Spawne() {
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		CPos pos = m_posAry[m_spawnerIndex];
		CBaseEnemy* enemy = GetNonStopEnemy(pos);
		CInOutBehavior *move = new CInOutBehavior(pos, pos, pos, 7, 8, 10);
		move->SetLeaveDirToPlayer2();
		enemy->SetBehaviorComponent(move);
		enemy->SetMovingShot();
		CBattleScene::m_enemyManager.Add(enemy);

		m_spawnerIndex++;
	}
}




//-----------------------------------------------------------------------
// 上から真ん中まで動いて、そのままうごうご(中型機)
SpawnerMediumTop_Step::SpawnerMediumTop_Step(EnemySize spawnerSize) : SpawnerMoveStopMove(spawnerSize)
{
	SetStepSpawner(spawnerSize);
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;


	int rand = CFunc::RandI(0, 2);
	if (rand == 0) { // 完全ランダム
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(CFunc::RandD(0.20, 0.80));
			double y = CGame::ToGamePosY(-0.1);
			m_posAry.push_back(CPos(x,y));
		}
	} else if (rand == 1) { // 連続0123
		double rx = CFunc::RandD(0.10, 0.60);
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(rx + ii * 0.05);
			double y = CGame::ToGamePosY(-0.1);
			m_posAry.push_back(CPos(x, y));
		}
	} else if (rand == 2) { // 連続3210
		double rx = CFunc::RandD(0.30, 0.90);
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(rx - ii * 0.05);
			double y = CGame::ToGamePosY(-0.1);
			m_posAry.push_back(CPos(x, y));
		}
	}
	int rand2 = CFunc::RandI(0, 1);
	if (rand2 == 0) {
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double goalAddY = CGame::ToGamePosY(0.20) + CGame::ToGamePosY(CFunc::RandD(0.05, 0.4));
			m_targetAry.push_back(CPos(0, goalAddY));
		}
	}
	else {
		double goalAddY = CGame::ToGamePosY(0.20) + CGame::ToGamePosY(CFunc::RandD(0.05, 0.4));
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			m_targetAry.push_back(CPos(0, goalAddY));
		}
	}
}
SpawnerMediumTop_Step::~SpawnerMediumTop_Step() {}
void SpawnerMediumTop_Step::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		CPos pos = m_posAry[m_spawnerIndex];
		double goalAddY = m_targetAry[m_spawnerIndex].y;
		CBaseEnemy* enemy = GetStepEnemy(pos);
		CPos targetPos = pos + CPos(0, goalAddY);
		CBehaviorComponent* component =  new CInStepBehavior(pos, targetPos, 7 );
		enemy->SetBehaviorComponent(component);

		CBattleScene::m_enemyManager.Add(enemy);
		m_spawnerIndex++;
	}
}

//------------------------------------------------------------------------
// ノンストップゆっくり上から下に
SpawnerMediumTop_Down::SpawnerMediumTop_Down(EnemySize spawnerSize) : SpawnerBase(spawnerSize)
{
	SetNonStopSpawner(spawnerSize);
	m_maxCount = ToSecond(FormationSpawneFinishTiming);
	m_spawneTiming = m_maxCount / m_maxSpawneCount;

	int rand = CFunc::RandI(0, 3);
	if (rand == 0) { // 完全ランダム
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(CFunc::RandD(0.20, 0.80));
			double y = CGame::ToGamePosY(-0.1);
			m_posAry.push_back(CPos(x, y));
		}
	}
	else if (rand == 1) { // 連続0123
		double rx = CFunc::RandD(0.10, 0.50);
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(rx + ii * 0.10);
			double y = CGame::ToGamePosY(-0.1);
			m_posAry.push_back(CPos(x, y));
		}
	}
	else if (rand == 2) { // 連続3210
		double rx = CFunc::RandD(0.50, 0.90);
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(rx - ii * 0.10);
			double y = CGame::ToGamePosY(-0.1);
			m_posAry.push_back(CPos(x, y));
		}
	}
	else if (rand == 3) { // 縦
		double rx = CFunc::RandD(0.10, 0.90);
		for (int ii = 0; ii < m_maxSpawneCount; ii++) {
			double x = CGame::ToGamePosX(rx);
			double y = CGame::ToGamePosY(-0.1 - ii * 0.05);
			m_posAry.push_back(CPos(x, y));
		}
	}
}

SpawnerMediumTop_Down::~SpawnerMediumTop_Down() {}

void SpawnerMediumTop_Down::Spawne()
{
	if (m_count >= m_maxCount) {
		m_deleteFlg = true;
		return;
	}
	m_count++;

	if (m_count % m_spawneTiming == 0) {
		CPos pos = m_posAry[m_spawnerIndex];
		
		CBaseEnemy* enemy = GetNonStopEnemy(pos);
		CPos targetPos = pos + CPos(0, CGame::ToGameSizeY(0.3));
		CPos returnPos = pos + CPos(0, CGame::ToGameSizeY(1.3));

		CInOutBehavior* move = new CInOutBehavior(pos, targetPos, returnPos, 7, 1.0, 0);

		enemy->SetBehaviorComponent(move);
		enemy->SetMovingShot();

		CBattleScene::m_enemyManager.Add(enemy);
		m_spawnerIndex++;
	}
}


