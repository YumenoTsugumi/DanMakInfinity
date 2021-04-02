﻿
#include "StageManager.h"
#include "BattleScene.h"

//#include "EnemySmall.h"
//#include "EnemyMedium.h"
//#include "EnemyLarge.h"

#include "Spawner.h"
#include "Game.h"

StageManager::StageManager()
{
	m_count = 0;
	m_maxCount = 60*120; // 120s
	m_spawneTiming = 60 * FormationSpawneTiming; // n秒毎に敵スポーン
	m_strengthCounter = 0;
}

StageManager::~StageManager() 
{
}


void StageManager::Main()
{
	if (m_count >= m_maxCount) {
		return;
	}

	static bool debugMode = false;
	static int debugOnce = 0;
	// 編隊の出現順番
	if (m_count % m_spawneTiming == 0 && (debugOnce == 0 || !debugMode)) {
		debugOnce++;
		AddSpawner();
	}

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


void StageManager::AddSpawner()
{
	//SpawnerBase* spawner = GetRandomSpawner();
	SpawnerBase* spawner = GetTestSpawner();
	m_spawners.push_back(spawner);
}

SpawnerBase* StageManager::GetRandomSpawner()
{
	return new SpawnerSmallLeftRight_Stop();
}

SpawnerBase* StageManager::GetTestSpawner()
{
	//return new SpawnerM02();
	int rand = CFunc::RandI(0, 1);
	rand = 1;
	switch (rand) {
		case 0:	return new SpawnerSmallTop_Stop();
		case 1: return new SpawnerSmallLeftRight_Stop();
	}
}



SpawnerBase::SpawnerBase() :
	m_count(0),
	m_maxCount(0),
	m_spawneTiming(0),
	m_deleteFlg(0),
	m_spawneCount(0)
{

}
SpawnerBase::~SpawnerBase(){}
void SpawnerBase::Spawne(){}
int SpawnerBase::GetPower() { return 1; }

int SpawnerBase::ToSecond(int millSecond)
{
	return millSecond * 60;
}
double SpawnerBase::ToSecond(double millSecond)
{
	return millSecond * 60.0;
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

#include "EnemyS01.h"
#include "EnemyM02.h"
CBaseEnemy* SpawnerBase::GetSmallEnemy(const CPos& pos)
{
	return new CEnemyS01(pos);
}
