
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

const int toSecond = 60;
const int FormationSpawneFinishTiming = toSecond * 1.2; // 1編隊の全敵をスポーンする時間(秒)

const int FormationSpawneSmallATiming = toSecond * 3; // 3秒ごとに編隊をスポーンする間隔(秒)
const int FormationSpawneSmallBTiming = toSecond * 5; // 5秒ごとに編隊をスポーンする間隔(秒)
const int FormationSpawneMediumATiming = toSecond * 8; // 8秒ごとに編隊をスポーンする間隔(秒)
const int FormationSpawneLargeATiming = toSecond * 10; // 10秒ごとに編隊をスポーンする間隔(秒)

const int TotalSpawneTime = toSecond * 11; // 敵をスポーンしつづける時間（秒
const int TotalSpawneTime_EndTime = TotalSpawneTime + toSecond * 7; // 最後敵をスポーンしてからちょっと待つ時間（秒

StageManager::StageManager()
{
	StageReset();
}

StageManager::~StageManager() 
{
}

void StageManager::StageReset()
{
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


#if 0
	//if (m_count  == 1) {
	//	SpawnerBase* spawner = GetTestSpawner();
	//	m_spawners.push_back(spawner);
	//}

	//if (m_count % FormationSpawneSmallATiming == 0) {
	//	SpawnerBase* spawner = GetTestSpawner();
	//	m_spawners.push_back(spawner);
	//}

	//// 編隊の出現順番
	//if (m_count % FormationSpawneSmallATiming == 0) {
	//	m_spawners.push_back(GetRandomSpawner_SmallA());
	//}
	//if (m_count % FormationSpawneSmallBTiming == 0) {
	//	m_spawners.push_back(GetRandomSpawner_SmallB());
	//}
	//if (m_count % FormationSpawneMediumATiming == 0) {
	//	m_spawners.push_back(GetRandomSpawner_MediumA());
	//}
	//if (m_count % FormationSpawneLargeATiming == 0) {
	//	m_spawners.push_back(GetRandomSpawner_LargeA());
	//}
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
	int rand = CFunc::RandI(0, 2);
	switch (rand) {
		case 0: return new SpawnerSmallTop_NoStop_Uturn(size);
		case 1: return new SpawnerSmallTop_NoStop_LRTurn(size);
		case 2: return new SpawnerSmallTop_NoStop_LRCos(size);
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
	int rand = CFunc::RandI(0, 3);

	int ary[] = { 1,1,2,0,0,2 };
	static int count = 0;
	EnemySize ssss = (EnemySize)ary[count];
	count++;
	if (count >= 6)ssss = EnemySize::Medium;
	ssss = EnemySize::Medium;

	switch (rand) {
		case 0:	return new SpawnerSmallTop_Stop(ssss);
		case 1: return new SpawnerSmallLeftRight_Stop(ssss);
		case 2: return new SpawnerSmall_Line_Top_Stop(ssss);
		case 3: return new SpawnerSmall_Line_LeftRight_Stop(ssss);

		case 4: return new SpawnerSmallTop_NoStop_Uturn(ssss);
		case 5: return new SpawnerSmallTop_NoStop_LRTurn(ssss);
		case 6: return new SpawnerSmallTop_NoStop_LRCos(ssss);
	}
	return nullptr;
}

//---------------------------------------------------

//---------------------------------------------------

SpawnerBase::SpawnerBase(EnemySize spawnerSize) :
	m_count(0),
	m_spawneTiming(0),
	m_deleteFlg(0),
	m_spawnerIndex(0)
{
	SetSpawner(spawnerSize);
	SetSpeedBySize();
}
SpawnerBase::~SpawnerBase(){}
void SpawnerBase::Spawne(){}
int SpawnerBase::GetPower() { return 1; }


void SpawnerBase::SetSpawner(EnemySize spawnerSize)
{
	m_spawnerSize = spawnerSize;
	if (m_spawnerSize == EnemySize::Small) {
		m_index = GetSmallEnemyIndex();
		m_maxSpawneCount = 12;
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		m_index = GetMediumEnemyIndex();
		m_maxSpawneCount = 4;
	}
	else if (m_spawnerSize == EnemySize::Large) {
		m_index = GetLargeEnemyIndex();
		m_maxSpawneCount = 2;
	}
}
CBaseEnemy* SpawnerBase::GetEnemy(const CPos& pos)
{
	if (m_spawnerSize == EnemySize::Small) {
		return GetSmallEnemy(m_index, pos);
	}
	else if (m_spawnerSize == EnemySize::Medium) {
		return GetMediumEnemy(m_index, pos);
	}
	else if (m_spawnerSize == EnemySize::Large) {
		return GetLargeEnemy(m_index, pos);
	}
	return nullptr;
}

int SpawnerBase::GetSmallEnemyIndex() {
	//return 7;
	static int count = 0;
	//return count++ % 4;
	return CFunc::RandI(1, 7);
}
CBaseEnemy* SpawnerBase::GetSmallEnemy(int index, const CPos& pos)
{
	switch (index) {
		case 1: return new CEnemyS01(pos);
		case 2: return new CEnemyS02(pos);
		case 3: return new CEnemyS03(pos);
		case 4: return new CEnemyS04(pos);
		case 5: return new CEnemyS05(pos);
		case 6: return new CEnemyS06(pos);
		case 7: return new CEnemyS07(pos);
	}
	return new CEnemyS01(pos);
}
int SpawnerBase::GetMediumEnemyIndex()
{
	return CFunc::RandI(1, 1);
}
CBaseEnemy* SpawnerBase::GetMediumEnemy(int index, const CPos& pos)
{
	int rand = CFunc::RandI(1, 3);
	switch (rand) {
		case 1: return new CEnemyM01(pos);
		case 2: return new CEnemyM02(pos);
		case 3: return new CEnemyM08(pos);
	}
	return nullptr;
}
int SpawnerBase::GetLargeEnemyIndex()
{
	return CFunc::RandI(1, 1);
}
CBaseEnemy* SpawnerBase::GetLargeEnemy(int index, const CPos& pos)
{
	switch (index) {
		case 1: return new CEnemyL01(pos);
	}
	return nullptr;
}

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

// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
// FulHDなら (480,20)(1440,1060)の範囲
CPos SpawnerBase::ToGamePos(CPos ratioPos)
{
	return ToGamePos(ratioPos.x, ratioPos.y);
}
CPos SpawnerBase::ToGamePos(double ratioPosX, double ratioPosY)
{
	return CPos(ToGamePosX(ratioPosX), ToGamePosY(ratioPosY));
}
// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
// FulHDなら (480～1440)の範囲
double SpawnerBase::ToGamePosX(double ratioPosX)
{
	int min = GameWindowAreaLeft * CGame::GetWindowRatio();
	int max = GameWindowAreaRight * CGame::GetWindowRatio();
	return 	min + (max - min) * (ratioPosX);
}
// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
// FulHDなら (20～1060)の範囲
double SpawnerBase::ToGamePosY(double ratioPosY)
{
	int min = GameWindowAreaTop * CGame::GetWindowRatio();
	int max = GameWindowAreaBottom * CGame::GetWindowRatio();
	return 	min + (max - min) * (ratioPosY);
}

// 0.0～1.0で画面サイズに比例する大きさが返ってくる
double SpawnerBase::ToGameSizeX(double ratioPosX)
{
	int min = GameWindowAreaLeft * CGame::GetWindowRatio();
	int max = GameWindowAreaRight * CGame::GetWindowRatio();
	return (max - min) * (ratioPosX);
}
// 0.0～1.0で画面サイズに比例する大きさが返ってくる
double SpawnerBase::ToGameSizeY(double ratioPosY)
{
	int min = GameWindowAreaTop * CGame::GetWindowRatio();
	int max = GameWindowAreaBottom * CGame::GetWindowRatio();
	return 	(max - min) * (ratioPosY);
}
