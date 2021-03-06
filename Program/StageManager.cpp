﻿
#include "StageManager.h"
#include "BattleScene.h"

//#include "EnemySmall.h"
//#include "EnemyMedium.h"
//#include "EnemyLarge.h"

#include "EnemyS.h"
#include "EnemyM.h"
#include "EnemyL.h"

#include "Spawner.h"
#include "Game.h"

constexpr int toSecond = 60;
constexpr int FormationSpawneFinishTiming = toSecond * 1.2; // 1編隊の全敵をスポーンする時間(秒)

constexpr int FormationSpawneSmallATiming = toSecond * 3; // 3秒ごとに編隊をスポーンする間隔(秒)
constexpr int FormationSpawneSmallBTiming = toSecond * 5; // 5秒ごとに編隊をスポーンする間隔(秒)
constexpr int FormationSpawneMediumATiming = toSecond * 8; // 8秒ごとに編隊をスポーンする間隔(秒)
constexpr int FormationSpawneLargeATiming = toSecond * 10; // 10秒ごとに編隊をスポーンする間隔(秒)

constexpr int TotalSpawneTime = toSecond * 120; // 敵をスポーンしつづける時間（秒
constexpr int TotalSpawneTime_EndTime = TotalSpawneTime + toSecond * 7; // 最後敵をスポーンしてからちょっと待つ時間（秒

constexpr int RankUpTotalCount = 20; // 1回のステージでランクアップする回数
constexpr int RankUpTiming = TotalSpawneTime / RankUpTotalCount;

StageManager::StageManager()
{
	StageReset();

	m_debugSpawneEnemySize = 0;
	m_debugSpawneEnemyIndex = 1;
	m_debugSpawneEnemyMoveType = 1;
	m_debugSpawneCount = 0;
}

StageManager::~StageManager() 
{
}

void StageManager::StageReset()
{
	m_count = 0;
	status = Now;
}

void StageManager::Main()
{
	if (m_scene->m_isEscMenu) {
		return;
	}

	if (m_count >= TotalSpawneTime) {
		// 全敵のスポーンが終わってから一定時間まつ
		if (m_count == TotalSpawneTime) {
			status = WaitingEnd;
		}
		
		// すべての敵を破壊して、UIを表示する
		if (m_count == TotalSpawneTime_EndTime) {
			CBattleScene::SetBulletRemoveTime(CBattleScene::BulletRemoveType::Nothing, 0);
			m_scene->DestoryAllEnemyNothingItemDrop();
			status = ResultDrawWait;
		}
		m_count++;
		return;
	}

	if (m_count % RankUpTiming == 0 && m_count != 0) {
		CBattleScene::AddRankRatioByStageSpan();
	}

#if 0
	if (m_count % (60*5) == 1) {
		m_spawners.push_back(StageManager::GetRandomSpawner_LargeA());
	}
#elif 1

	DebugIndexControl();
	DebugContinueSpawner();
#else
	// 編隊の出現順番
	if (m_count % FormationSpawneSmallATiming == 0 && m_count != 0) {
		m_spawners.push_back(GetRandomSpawner_SmallA());
	}
	if (m_count % FormationSpawneSmallBTiming == 0) {
		m_spawners.push_back(GetRandomSpawner_SmallB());
	}
	if (m_count % FormationSpawneMediumATiming == 0 && m_count != 0) {
		m_spawners.push_back(GetRandomSpawner_MediumA());
	}
	if (m_count % FormationSpawneLargeATiming == 0 && m_count != 0) {
		m_spawners.push_back(GetRandomSpawner_LargeA());
	}
#endif

	for (auto it = m_spawners.begin(); it != m_spawners.end();) {
		SpawnerBase* spawner = *it;
		if (spawner->m_deleteFlg) {
			it = m_spawners.erase(it);
		}
		else {
			spawner->Spawne();
			++it;
		}
	}

	m_count++;
}

SpawnerBase* StageManager::GetRandomSpawner_SmallA()
{
	EnemySize size = EnemySize::Small;
	int rand = CFunc::RandI(0, 3);
	switch (rand) {
		case 0:	return new SpawnerSmallTop_Stop(size);
		case 1: return new SpawnerSmallLeftRight_Stop(size);
		case 2: return new SpawnerSmall_Line_Top_Stop(size);
		case 3: return new SpawnerSmall_Line_LeftRight_Stop(size);
	}
	return nullptr;
}
SpawnerBase* StageManager::GetRandomSpawner_SmallB()
{
	EnemySize size = EnemySize::Small;
	int rand = CFunc::RandI(0, 3);
	switch (rand) {
		case 0: return new SpawnerSmallTop_NoStop_Uturn(size);
		case 1: return new SpawnerSmallTop_NoStop_LRTurn(size);
		case 2: return new SpawnerSmallTop_NoStop_LRCos(size);
		case 3: return new SpawnerSmallTop_TracePlayer(size);
	}
	return nullptr;
}
SpawnerBase* StageManager::GetRandomSpawner_MediumA() 
{
	EnemySize size = EnemySize::Medium;
	int rand = CFunc::RandI(0, 5);
	switch (rand) {
		case 0:	return new SpawnerSmallTop_Stop(size);
		case 1: return new SpawnerSmallLeftRight_Stop(size);
		case 2: return new SpawnerSmall_Line_Top_Stop(size);
		case 3: return new SpawnerSmall_Line_LeftRight_Stop(size);
		case 4: return new SpawnerMediumTop_Step(size);
		case 5: return new SpawnerMediumTop_Down(size);
	}
	return nullptr;
}

SpawnerBase* StageManager::GetRandomSpawner_LargeA()
{
	EnemySize size = EnemySize::Large;
	int rand = CFunc::RandI(0, 3);
	switch (rand) {
		case 0: return new SpawnerSmallTop_NoStop_Uturn(size);
		case 1: return new SpawnerSmallTop_NoStop_LRTurn(size);
		case 2: return new SpawnerSmallTop_NoStop_LRCos(size);
		case 3: return new SpawnerMediumTop_Down(size);
	}
	return nullptr;
}

SpawnerBase* StageManager::GetTestSpawner()
{
	int aaa = CFunc::RandI(0, 3);
	if (aaa == 0)return new SpawnerSmallTop_NoStop_Uturn(EnemySize::Large);
	if (aaa == 1)return new SpawnerSmallTop_NoStop_LRTurn(EnemySize::Large);
	if (aaa == 2)return new SpawnerSmallTop_NoStop_LRCos(EnemySize::Large);
	if (aaa == 3)return new SpawnerMediumTop_Down(EnemySize::Large);

#if 0
	EnemySize ssss;
	ssss = EnemySize::Small;
	return new SpawnerSmallTop_TracePlayer(ssss);
#else 0
	int rand = CFunc::RandI(0, 3);

	int ary[] = { 1,1,2,0,0,2 };
	static int count = 0;
	EnemySize ssss = (EnemySize)ary[count];
	count++;
	if (count >= 6)ssss = EnemySize::Medium;
	ssss = EnemySize::Medium;

	switch (rand) {
		//case 0: return new SpawnerSmallTop_Stop(ssss);
		//case 1: return new SpawnerSmallLeftRight_Stop(ssss);
		//case 2: return new SpawnerSmall_Line_Top_Stop(ssss);
		//case 3: return new SpawnerSmall_Line_LeftRight_Stop(ssss);

		//case 4: return new SpawnerSmallTop_NoStop_Uturn(ssss);
		//case 5: return new SpawnerSmallTop_NoStop_LRTurn(ssss);
		//case 6: return new SpawnerSmallTop_NoStop_LRCos(ssss);
	}
#endif
	return nullptr;
}

SpawnerBase* StageManager::DebugGetMove(int index, EnemySize size) {
	if (index == 0)return new SpawnerSmallTop_Stop(size);
	if (index == 1)return new SpawnerSmallLeftRight_Stop(size);
	if (index == 2)return new SpawnerSmall_Line_Top_Stop(size);
	if (index == 3)return new SpawnerSmall_Line_LeftRight_Stop(size);

	if (index == 4)return new SpawnerSmallTop_NoStop_Uturn(size);
	if (index == 5)return new SpawnerSmallTop_NoStop_LRTurn(size);
	if (index == 6)return new SpawnerSmallTop_NoStop_LRCos(size);
	if (index == 7)return new SpawnerSmallTop_TracePlayer(size);
	if (index == 8)return new SpawnerMediumTop_Step(size);
	if (index == 9)return new SpawnerMediumTop_Down(size);
	return nullptr;
};
std::string StageManager::DebugGetMoveName(int index) {
	if (index == 0)return "SpawnerSmallTop_Stop";
	if (index == 1)return "SpawnerSmallLeftRight_Stop";
	if (index == 2)return "SpawnerSmall_Line_Top_Stop";
	if (index == 3)return "SpawnerSmall_Line_LeftRight_Stop";

	if (index == 4)return "SpawnerSmallTop_NoStop_Uturn";
	if (index == 5)return "SpawnerSmallTop_NoStop_LRTurn";
	if (index == 6)return "SpawnerSmallTop_NoStop_LRCos";
	if (index == 7)return "SpawnerSmallTop_TracePlayer";
	if (index == 8)return "SpawnerMediumTop_Step";
	if (index == 9)return "SpawnerMediumTop_Down";
	return "null";
};
void StageManager::DebugIndexControl()
{
	m_count = 0; // 普通のスポーンはしない
	// 小中大
	static int uc = 0;
	if (CheckHitKey(KEY_INPUT_U) != 0) {
		if (uc++ % 10 == 0) {
			m_debugSpawneEnemySize++;
			if (m_debugSpawneEnemySize > (int)EnemySize::Large) {
				m_debugSpawneEnemySize = 0;
			}
			m_debugSpawneEnemyIndex = 1;
		};
	}
	static int ic = 0;
	if (CheckHitKey(KEY_INPUT_I) != 0) {
		if (ic++ % 10 == 0) {
			m_debugSpawneEnemyIndex++;
			if (m_debugSpawneEnemySize == EnemySize::Small && m_debugSpawneEnemyIndex > DebugGetSmallMax()) {
				m_debugSpawneEnemyIndex = 1;
			}
			if (m_debugSpawneEnemySize == EnemySize::Medium && m_debugSpawneEnemyIndex > DebugGetMediumMax()) {
				m_debugSpawneEnemyIndex = 1;
			}
			if (m_debugSpawneEnemySize == EnemySize::Large && m_debugSpawneEnemyIndex > DebugGetLargeMax()) {
				m_debugSpawneEnemyIndex = 1;
			}
		}
	}
	static int oc = 0;
	if (CheckHitKey(KEY_INPUT_O) != 0) {
		if (oc++ % 10 == 0) {
			m_debugSpawneEnemyMoveType++;
			if (m_debugSpawneEnemyMoveType > 7) {
				m_debugSpawneEnemyMoveType = 1;
			}
		}
	}

	std::vector<std::string> sizeStr = { "小型機","中型機","大型機" };
	printfDx("%s(u):%d(i) \n", sizeStr[m_debugSpawneEnemySize].c_str(), m_debugSpawneEnemyIndex);
	printfDx("%s(o):スポーンはp \n", DebugGetMoveName(m_debugSpawneEnemyMoveType).c_str());
}
void StageManager::DebugContinueSpawner()
{
	static int debug = 0;
	if (debug == 0) {
		m_debugSpawneEnemySize = 0;
		m_debugSpawneEnemyIndex = 1;
		m_debugSpawneEnemyMoveType = 0;
		debug++;
	}

	if (m_debugSpawneCount % (60*3) == 0) {
		SpawnerBase* sb = DebugGetMove(m_debugSpawneEnemyMoveType, (EnemySize)m_debugSpawneEnemySize);
		sb->SetEnemyIndex(m_debugSpawneEnemyIndex);
		m_spawners.push_back(sb);

	}
	m_debugSpawneCount++;
}


//---------------------------------------------------

SpawnerBase::SpawnerBase(EnemySize spawnerSize) :
	m_count(0),
	m_spawneTiming(0),
	m_deleteFlg(0),
	m_spawnerIndex(0),
	m_spawnerSize(spawnerSize)
{
	SetSpeedBySize();
}
SpawnerBase::~SpawnerBase(){}
void SpawnerBase::Spawne(){}
int SpawnerBase::GetPower() { return 1; }

//-------------------------------------------------------------------------
// ストップして攻撃する
void SpawnerBase::SetStopSpawner(EnemySize spawnerSize)
{
	m_spawnerSize = spawnerSize;
	if (m_spawnerSize == EnemySize::Small) {
		m_index = GetSmallStopEnemyIndex();
		m_maxSpawneCount = 9;
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		m_index = GetMediumStopEnemyIndex();
		m_maxSpawneCount = 3;
	}
	else if (m_spawnerSize == EnemySize::Large) {
		m_index = GetLargeStopEnemyIndex();
		m_maxSpawneCount = 1;
	}
}
CBaseEnemy* SpawnerBase::GetStopEnemy(const CPos& pos){
	CBaseEnemy* ep = nullptr;
	if (m_spawnerSize == EnemySize::Small) {
		ep = GetSmallStopEnemy(m_index, pos);
	} else if (m_spawnerSize == EnemySize::Medium) {
		ep = GetMediumStopEnemy(m_index, pos);
	} else if (m_spawnerSize == EnemySize::Large) {
		ep = GetLargeStopEnemy(m_index, pos);
	}
	ep->SetMoveType(CBaseEnemy::MoveType::StopShot);
	return ep;
}

//-------------------------------------------------------------
// デバッグ
int StageManager::DebugGetSmallMax() { return 7; }
int SpawnerBase::GetSmallStopEnemyIndex() {
	return CFunc::RandI(1, 7);
}
CBaseEnemy* SpawnerBase::GetSmallStopEnemy(int index, const CPos& pos){
	switch (index) {
		case 1: return new CEnemyS01(pos);
		case 2: return new CEnemyS02(pos);
		case 3: return new CEnemyS03(pos);
		case 4: return new CEnemyS04(pos);
		case 5: return new CEnemyS05(pos);
		case 6: return new CEnemyS06(pos);
		case 7: return new CEnemyS07(pos);
	}
	assert(0);
	return nullptr;
}
int StageManager::DebugGetMediumMax() { return 10; }
int SpawnerBase::GetMediumStopEnemyIndex(){
	return CFunc::RandI(1, 10);
}
CBaseEnemy* SpawnerBase::GetMediumStopEnemy(int index, const CPos& pos){
	switch (index) {
		case 1: return new CEnemyM01(pos); // (STOP)固定角度5WAY　4～5回
		case 2: return new CEnemyM02(pos); // (STOP,NONSTOP)エヴァッカニア第二の扇
		case 3: return new CEnemyM03(pos); // (STOP,NONSTOP)ドルフィンみたいな　２WAY時期狙い
		case 4: return new CEnemyM04(pos); // (STOP,NONSTOP)ランダム弾5発
		case 5: return new CEnemyM05(pos); // (STOP,NONSTOP)V弾
		case 6: return new CEnemyM06(pos); // (STOP,NONSTOP)芋虫連なり弾
		case 7: return new CEnemyM07(pos); // (STOP,NONSTOP)双顎
		case 8: return new CEnemyM08(pos); // (STOP)ためうちの敵
		case 9: return new CEnemyM09(pos); // (STOP,NONSTOP)幅広7WAY
		case 10: return new CEnemyM10(pos); // (STOP,NONSTOP)345WAY
	}
	return nullptr;
}
int StageManager::DebugGetLargeMax() { return 1; }
int SpawnerBase::GetLargeStopEnemyIndex(){
	return CFunc::RandI(1, 1);
}
CBaseEnemy* SpawnerBase::GetLargeStopEnemy(int index, const CPos& pos){
	switch (index) {
		case 1: return new CEnemyL01(pos);
	}
	return nullptr;
}
//-------------------------------------------------------------------------
// ノンストップ攻撃する 動き続ける敵
void SpawnerBase::SetNonStopSpawner(EnemySize spawnerSize)
{
	m_spawnerSize = spawnerSize;
	if (m_spawnerSize == EnemySize::Small) {
		m_index = GetSmallNonStopEnemyIndex();
		m_maxSpawneCount = 9;
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		m_index = GetMediumNonStopEnemyIndex();
		m_maxSpawneCount = 3;
	}
	else if (m_spawnerSize == EnemySize::Large) {
		m_index = GetLargeNonStopEnemyIndex();
		m_maxSpawneCount = 1;
	}
}
CBaseEnemy* SpawnerBase::GetNonStopEnemy(const CPos& pos) {
	CBaseEnemy* ep = nullptr;
	if (m_spawnerSize == EnemySize::Small) {
		ep = GetSmallNonStopEnemy(m_index, pos);
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		ep = GetMediumNonStopEnemy(m_index, pos);
	}
	else if (m_spawnerSize == EnemySize::Large) {
		ep = GetLargeNonStopEnemy(m_index, pos);
	}
	ep->SetMoveType(CBaseEnemy::MoveType::MoveingShot);
	return ep;
}

int SpawnerBase::GetSmallNonStopEnemyIndex() {
	return CFunc::RandI(1, 7);
}
CBaseEnemy* SpawnerBase::GetSmallNonStopEnemy(int index, const CPos& pos) {
	switch (index) {
		case 1: return new CEnemyS01(pos);
		case 2: return new CEnemyS02(pos);
		case 3: return new CEnemyS03(pos);
		case 4: return new CEnemyS04(pos);
		case 5: return new CEnemyS05(pos);
		case 6: return new CEnemyS06(pos);
		case 7: return new CEnemyS07(pos);
	}
	assert(0);
	return nullptr;
}
int SpawnerBase::GetMediumNonStopEnemyIndex() {
	return CFunc::RandI(1, 8);
}
CBaseEnemy* SpawnerBase::GetMediumNonStopEnemy(int index, const CPos& pos) {
	switch (index) {
		case 1: return new CEnemyM02(pos); // (STOP,NONSTOP)エヴァッカニア第二の扇
		case 2: return new CEnemyM03(pos); // (STOP,NONSTOP)ドルフィンみたいな　２WAY時期狙い
		case 3: return new CEnemyM04(pos); // (STOP,NONSTOP)ランダム弾5発
		case 4: return new CEnemyM05(pos); // (STOP,NONSTOP)V弾
		case 5: return new CEnemyM06(pos); // (STOP,NONSTOP)芋虫連なり弾
		case 6: return new CEnemyM07(pos); // (STOP,NONSTOP)双顎
		case 7: return new CEnemyM09(pos); // (STOP,NONSTOP)幅広7WAY
		case 8: return new CEnemyM10(pos); // (STOP,NONSTOP)345WAY
	}
	assert(0);
	return nullptr;
}

int SpawnerBase::GetLargeNonStopEnemyIndex()
{
	return CFunc::RandI(1, 4);
}
CBaseEnemy* SpawnerBase::GetLargeNonStopEnemy(int index, const CPos& pos)
{
	switch (index) {
		case 1: return new CEnemyL01(pos);
		case 2: return new CEnemyL02(pos);
		case 3: return new CEnemyL03(pos);
		case 4: return new CEnemyL04(pos);
	}
	return nullptr;
}


// 退場せずにステップ動作する
// ストップして攻撃する
void SpawnerBase::SetStepSpawner(EnemySize spawnerSize)
{
	m_spawnerSize = spawnerSize;
	if (m_spawnerSize == EnemySize::Small) {
		assert(0);
		//m_index = GetSmallStepEnemyIndex();
		//m_maxSpawneCount = 9;
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		m_index = GetMediumStepEnemyIndex();
		m_maxSpawneCount = 3;
	}
	else if (m_spawnerSize == EnemySize::Large) {
		//m_index = GetLargeStepEnemyIndex();
		//m_maxSpawneCount = 1;
	}
}
CBaseEnemy* SpawnerBase::GetStepEnemy(const CPos& pos) {
	CBaseEnemy* ep = nullptr;
	if (m_spawnerSize == EnemySize::Small) {
		//ep = GetSmallStopEnemy(m_index, pos);
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		ep = GetMediumStepEnemy(m_index, pos);
	}
	else if (m_spawnerSize == EnemySize::Large) {
		//ep = GetLargeStepEnemy(m_index, pos);
	}
	ep->SetMoveType(CBaseEnemy::MoveType::MoveingShot);
	return ep;
}
int SpawnerBase::GetMediumStepEnemyIndex()
{
	return CFunc::RandI(1, 8);
}
CBaseEnemy* SpawnerBase::GetMediumStepEnemy(int index, const CPos& pos)
{
	switch (index) {
		case 1: return new CEnemyM02(pos); // (STOP,NONSTOP)エヴァッカニア第二の扇
		case 2: return new CEnemyM03(pos); // (STOP,NONSTOP)ドルフィンみたいな　２WAY時期狙い
		case 3: return new CEnemyM04(pos); // (STOP,NONSTOP)ランダム弾5発
		case 4: return new CEnemyM05(pos); // (STOP,NONSTOP)V弾
		case 5: return new CEnemyM06(pos); // (STOP,NONSTOP)芋虫連なり弾
		case 6: return new CEnemyM07(pos); // (STOP,NONSTOP)双顎
		case 7: return new CEnemyM09(pos); // (STOP,NONSTOP)幅広7WAY
		case 8: return new CEnemyM10(pos); // (STOP,NONSTOP)345WAY
	}
	return nullptr;
}

int SpawnerBase::GetLargeStepEnemyIndex()
{
	return CFunc::RandI(1, 1);
}
CBaseEnemy* SpawnerBase::GetLargeStepEnemy(int index, const CPos& pos)
{
	switch (index) {
		case 1: return new CEnemyL01(pos); 
	}
	return nullptr;
}

// 入場する際の交点にマーカーを表示する
void SpawnerBase::SetCrossMaker(const CPos& pos, const CPos& targetPos, bool topCheck /*= true*/, bool rightFirst /*= true*/)
{
	double l, r;
	CPos xp;
	bool chk;

	double size;
	if (m_spawnerSize == EnemySize::Small) {
		size = 0.50;
	}else if (m_spawnerSize == EnemySize::Medium) {
		size = 0.75;
	}else if (m_spawnerSize == EnemySize::Large) {
		size = 1.00;
	}

	double angle;
	bool chk1 = false, chk2 = false, chk3 = false;
	do {
		if (topCheck) {
			// 上側
			CPos tl2(CGame::ToGamePosX(-1), CGame::ToGamePosY(0.05));
			CPos tr2(CGame::ToGamePosX(2), CGame::ToGamePosY(0.05));
			chk2 = CFunc::CalcIntersectionPoint(pos, targetPos, tl2, tr2, xp, l, r);
			if (chk2) {
				angle = 90.0;
				if (xp.x < CGame::ToGamePosX(0)) {
					xp.x = CGame::ToGamePosX(0);
					angle = 45.0;
				}
				if (xp.x > CGame::ToGamePosX(1)) {
					xp.x = CGame::ToGamePosX(1);
					angle = 135.0;
				}
				break;
			}
		}

		if (!rightFirst) {
			// 左側面
			CPos tl1(CGame::ToGamePosX(0), CGame::ToGamePosY(-1));
			CPos dl1(CGame::ToGamePosX(0), CGame::ToGamePosY(1));
			chk1 = CFunc::CalcIntersectionPoint(pos, targetPos, tl1, dl1, xp, l, r);
			if (chk1) {
				angle = 0.0;
				if (xp.y < CGame::ToGamePosY(0)) {
					xp.y = CGame::ToGamePosY(0);
					angle = 45.0;
				}
				break;
			}
		}

		// 右側面
		CPos tr3(CGame::ToGamePosX(1), CGame::ToGamePosY(-1));
		CPos dr3(CGame::ToGamePosX(1), CGame::ToGamePosY(1));
		chk3 = CFunc::CalcIntersectionPoint(pos, targetPos, tr3, dr3, xp, l, r);
		if (chk3) {
			angle = 180.0;
			if (xp.y < CGame::ToGamePosY(0)) {
				xp.y = CGame::ToGamePosY(0);
				angle = 135.0;
			}
			break;
		}

		if (rightFirst) {
			// 左側面
			CPos tl1(CGame::ToGamePosX(0), CGame::ToGamePosY(-1));
			CPos dl1(CGame::ToGamePosX(0), CGame::ToGamePosY(1));
			chk1 = CFunc::CalcIntersectionPoint(pos, targetPos, tl1, dl1, xp, l, r);
			if (chk1) {
				angle = 0.0;
				if (xp.y < CGame::ToGamePosY(0)) {
					xp.y = CGame::ToGamePosY(0);
					angle = 45.0;
				}
				break;
			}
		}
	} while (0);

	if (chk1 || chk2 || chk3) {
		CBaseEffect* eff = new CBaseEffect(60, EDirType::Abs, xp, 0, angle, 0, 0, 0, 0, 1600);
		eff->SetSize(size, 0);
		eff->SetBlend(255, -5, 0);
		eff->SetBlendType(DX_BLENDMODE_ALPHA);
		eff->SetScroll(false);
		eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
		eff->SetRemoveCount(255 / 5);	//60frで削除
		CBattleScene::m_effectManager.Add(eff);
	}

}


//-------------------------------------------------------------
int SpawnerBase::ToSecond(int millSecond)
{
	return millSecond * 60;
}
double SpawnerBase::ToSecond(double millSecond)
{
	return millSecond * 60.0;
}

void SpawnerBase::SetSpeedBySize()
{
	if (m_spawnerSize == EnemySize::Small) {
		m_speed = CFunc::RandD(4.0, 5.5);
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		m_speed = CFunc::RandD(1.5, 2.5);
	}
	else if (m_spawnerSize == EnemySize::Large) {
		m_speed = CFunc::RandD(0.8, 1.2);
	}
}
