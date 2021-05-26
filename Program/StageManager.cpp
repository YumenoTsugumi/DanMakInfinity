
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

constexpr int TotalSpawneTime = toSecond * 10; // 敵をスポーンしつづける時間（秒
constexpr int TotalSpawneTime_EndTime = TotalSpawneTime + toSecond * 7; // 最後敵をスポーンしてからちょっと待つ時間（秒

constexpr int RankUpTotalCount = 20; // 1回のステージでランクアップする回数
constexpr int RankUpTiming = TotalSpawneTime / RankUpTotalCount;

StageManager::StageManager()
{
	StageReset();

	m_spawneEnemySize = 0;
	m_spawneEnemyIndex = 1;
	m_spawneEnemyMoveType = 1;
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

#if 1
	m_count = 0;
	DebugSpawner();
#elif 1
	if (m_count % FormationSpawneMediumATiming == 0) {
		m_spawners.push_back(GetTestSpawner());
	}
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
	int rand = CFunc::RandI(0, 3);
	switch (rand) {
		case 0:	return new SpawnerSmallTop_Stop(size);
		case 1: return new SpawnerSmallLeftRight_Stop(size);
		case 2: return new SpawnerSmall_Line_Top_Stop(size);
		case 3: return new SpawnerSmall_Line_LeftRight_Stop(size);
	}
	return nullptr;
}

SpawnerBase* StageManager::GetRandomSpawner_LargeA()
{
	EnemySize size = EnemySize::Large;
	int rand = CFunc::RandI(0, 3);
	switch (rand) {
		case 0:	return new SpawnerSmallTop_Stop(size);
		case 1: return new SpawnerSmallLeftRight_Stop(size);
		case 2: return new SpawnerSmall_Line_Top_Stop(size);
		case 3: return new SpawnerSmall_Line_LeftRight_Stop(size);
	}
	return nullptr;
}

SpawnerBase* StageManager::GetTestSpawner()
{
	return new SpawnerSmallTop_Stop(EnemySize::Medium);
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


void StageManager::DebugSpawner()
{
	// m_spawneEnemySize
	// 小中大
	static int uc = 0;
	if (CheckHitKey(KEY_INPUT_U) != 0) {
		if (uc++ % 10 == 0) {
			m_spawneEnemySize++;
			if (m_spawneEnemySize > (int)EnemySize::Large) {
				m_spawneEnemySize = 0;
			}
			m_spawneEnemyIndex = 1;
		};
	}


	static int ic = 0;
	if (CheckHitKey(KEY_INPUT_I) != 0) {
		if (ic++ % 10 == 0) {
			m_spawneEnemyIndex++;
			if (m_spawneEnemySize == EnemySize::Small && m_spawneEnemyIndex > DebugGetSmallMax()) {
				m_spawneEnemyIndex = 1;
			}
			if (m_spawneEnemySize == EnemySize::Medium && m_spawneEnemyIndex > DebugGetMediumMax()) {
				m_spawneEnemyIndex = 1;
			}
			if (m_spawneEnemySize == EnemySize::Large && m_spawneEnemyIndex > DebugGetLargeMax()) {
				m_spawneEnemyIndex = 1;
			}
		}
	}

	struct Func {
		static SpawnerBase* getMove(int index, EnemySize size) {
			if (index == 0)return new SpawnerSmallTop_Stop(size);
			if (index == 1)return new SpawnerSmallLeftRight_Stop(size);
			if (index == 2)return new SpawnerSmall_Line_Top_Stop(size);
			if (index == 3)return new SpawnerSmall_Line_LeftRight_Stop(size);
			if (index == 4)return new SpawnerSmallTop_NoStop_Uturn(size);
			if (index == 5)return new SpawnerSmallTop_NoStop_LRTurn(size);
			if (index == 6)return new SpawnerSmallTop_NoStop_LRCos(size);
			if (index == 7)return new SpawnerSmallTop_TracePlayer(size);
			return nullptr;
		};
		static std::string getMoveName(int index) {
			if (index == 0)return "SpawnerSmallTop_Stop";
			if (index == 1)return "SpawnerSmallLeftRight_Stop";
			if (index == 2)return "SpawnerSmall_Line_Top_Stop";
			if (index == 3)return "SpawnerSmall_Line_LeftRight_Stop";
			if (index == 4)return "SpawnerSmallTop_NoStop_Uturn";
			if (index == 5)return "SpawnerSmallTop_NoStop_LRTurn";
			if (index == 6)return "SpawnerSmallTop_NoStop_LRCos";
			if (index == 7)return "SpawnerSmallTop_TracePlayer";
			return nullptr;
		};
	};

	static int oc = 0;
	if (CheckHitKey(KEY_INPUT_O) != 0) {
		if (oc++ % 10 == 0) {
			m_spawneEnemyMoveType++;
			if (m_spawneEnemyMoveType > 7) {
				m_spawneEnemyMoveType = 1;
			}
		}
	}


	std::vector<std::string> sizeStr = { "小型機","中型機","大型機" };
	printfDx("%s:%d \n", sizeStr[m_spawneEnemySize].c_str(), m_spawneEnemyIndex);
	printfDx("%s \n", Func::getMoveName(m_spawneEnemyMoveType).c_str());

	static int pc = 0;
	if (CheckHitKey(KEY_INPUT_P) != 0) {
		if (pc++ % 10 == 0) {
			SpawnerBase* sb = Func::getMove(m_spawneEnemyMoveType, (EnemySize)m_spawneEnemySize);
			sb->SetEnemyIndex(m_spawneEnemyIndex);
			m_spawners.push_back(sb);
		}
	}
}

//---------------------------------------------------

//---------------------------------------------------

SpawnerBase::SpawnerBase(EnemySize spawnerSize) :
	m_count(0),
	m_spawneTiming(0),
	m_deleteFlg(0),
	m_spawnerIndex(0)
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
		m_maxSpawneCount = 12;
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		m_index = GetMediumStopEnemyIndex();
		m_maxSpawneCount = 4;
	}
	else if (m_spawnerSize == EnemySize::Large) {
		m_index = GetLargeStopEnemyIndex();
		m_maxSpawneCount = 2;
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
	ep->SetMoveType(CBaseEnemy::MoveType::InShotOut);
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
int StageManager::DebugGetMediumMax() { return 4; }
int SpawnerBase::GetMediumStopEnemyIndex(){
	return CFunc::RandI(1, 4);
}
CBaseEnemy* SpawnerBase::GetMediumStopEnemy(int index, const CPos& pos){
	switch (index) {
		case 1: return new CEnemyM01(pos);
		case 2: return new CEnemyM02(pos);
		case 3: return new CEnemyM03(pos);
		case 4: return new CEnemyM08(pos);
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
		m_maxSpawneCount = 12;
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		m_index = GetMediumNonStopEnemyIndex();
		m_maxSpawneCount = 4;
	}
	else if (m_spawnerSize == EnemySize::Large) {
		m_index = GetLargeNonStopEnemyIndex();
		m_maxSpawneCount = 2;
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
	return CFunc::RandI(1, 4);
}
CBaseEnemy* SpawnerBase::GetMediumNonStopEnemy(int index, const CPos& pos) {
	switch (index) {
		case 1: return new CEnemyM01(pos);
		case 2: return new CEnemyM02(pos);
		case 3: return new CEnemyM03(pos);
	}
	assert(0);
	return nullptr;
}

int SpawnerBase::GetLargeNonStopEnemyIndex()
{
	return 0;
}
CBaseEnemy* SpawnerBase::GetLargeNonStopEnemy(int index, const CPos& pos)
{
	return nullptr;
}






// 退場せずにステップ動作する
int SpawnerBase::GetMediumStepEnemyIndex()
{
	return CFunc::RandI(1, 1);
}
CBaseEnemy* SpawnerBase::GetMediumStepEnemy(int index, const CPos& pos)
{
	switch (index) {
		case 1: return new CEnemyM04(pos);
	}
	return nullptr;
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
		m_speed = CFunc::RandD(1.0, 1.3);
	}
}
