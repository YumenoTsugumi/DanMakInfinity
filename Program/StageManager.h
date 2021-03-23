#pragma once

#include "resource.h"
#include "BaseEffect.h"

class SpawnerBase;

const int FormationSpawneTiming = 3; // 3秒ごとに編隊をスポーンする間隔
const int FormationSpawneFinishTiming = 2; // 1編隊の全敵をスポーンする時間


class StageManager {
public:
	StageManager();
	virtual ~StageManager();
	int m_count;
	int m_maxCount;
	int m_spawneTiming;
	std::vector<SpawnerBase*> m_spawners;

	// 編隊を強さに振り分けるやつ
	int m_strengthCounter;

	void Main();
	void AddSpawner();
	SpawnerBase* GetRandomSpawner();
	SpawnerBase* GetTestSpawner();
};

class SpawnerBase {
public:
	int m_count;
	int m_maxCount;
	int m_spawneTiming;
	int m_spawneCount;
	bool m_deleteFlg;

	SpawnerBase();
	virtual ~SpawnerBase();
	virtual void Spawne();

	// 1-10段階ぐらいにしとく
	// ランクで参照して、特定ランク未満だと出てこないようにするため
	virtual int GetPower(); // 編隊の強さ（ざっくり）


	// 各クラスでそれぞれ変数を定義するのがめんどくさいので、適当に定義しておく
	int iVal[10];
	double dVal[10];
	char cVal[10];
	bool bVal[10];

	int ToSecond(int millSecond);
	double ToSecond(double millSecond);


	// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	// FulHDなら (480,20)(1440,1060)の範囲
	CPos ToGamePos(CPos ratioPos);
	CPos ToGamePos(double ratioPosX, double ratioPosY);
	// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	double ToGamePosX(double ratioPosX); 
	// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	double ToGamePosY(double ratioPosY);

	// 0.0～1.0で画面サイズに比例する大きさが返ってくる
	double ToGameSizeX(double ratioPosX);
	// 0.0～1.0で画面サイズに比例する大きさが返ってくる
	double ToGameSizeY(double ratioPosY);
};



#define SPAWNEHEADER(ClassName)					\
class ClassName : public SpawnerBase {			\
public:											\
	ClassName();								\
	virtual ~ClassName();						\
	virtual void Spawne() override;				\
	virtual int GetPower() override;			\
};												\

#define SPAWNECPP(ClassName)					\
ClassName::ClassName() : SpawnerBase(){}		\
ClassName::~ClassName() {}						\

