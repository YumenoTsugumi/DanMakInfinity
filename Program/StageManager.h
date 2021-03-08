#pragma once

#include "resource.h"
#include "BaseEffect.h"

class SpawnerBase;

class StageManager {
public:
	StageManager();
	virtual ~StageManager();
	int m_count;
	int m_maxCount;
	int m_spawneTiming;
	std::vector<SpawnerBase*> m_spawners;

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
	bool m_deleteFlg;
	SpawnerBase();
	virtual ~SpawnerBase();
	virtual void Spawne();

	// 1-10段階ぐらいにしとく
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

