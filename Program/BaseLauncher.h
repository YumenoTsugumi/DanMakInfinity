#pragma once

#include "BaseBullet.h"
#include "BaseBeam.h"

const int MAX_RANK = 100;

// ランクUPで　
//	発射間隔が早くなる	xxfr～xxfr
//	連弾になる			xx~xx弾
//	速度が速くなる		xx～xx

class CBaseLauncher {
public:
	CBaseLauncher(int rank, const CPos& pos);
	virtual ~CBaseLauncher();

	virtual void Action(const CPos& newPos);

	//弾幕管理をしているマネージャ
	static CBulletManager* m_bulletManager;
	static void SetBulletManagerPointer(CBulletManager* Manager);
	static CBeamManager* m_beamManager;
	static void SetBeamManagerPointer(CBeamManager* Manager);
	CPos m_pos; // 発射口の座標
	int m_rank; // 1~100を想定
protected:
	int m_count; // 発射のタイミングのカウンター


	// 各クラスでそれぞれ変数を定義するのがめんどくさいので、適当に定義しておく
	int iVal[10];
	double dVal[10];
	char cVal[10];
	bool bVal[10];

protected:
	// 現在のランクの割合を返す 0.0～1.0
	double RankRatio();
	// 連弾の速度差を生成する(speedの10％までをindexで等分割する)
	// 10連弾なら　indexは0～9 maxIndexは10
	double SpeedRange(double speed, int index, int max);
};

//--------------------------------------------------
// 禁じ手のdefine
//--------------------------------------------------

//class CLauncher001 : public CBaseLauncher {
//public:
//	CLauncher001(int rank, const CPos& pos);
//	virtual ~CLauncher001() override;
//	virtual void Action(const CPos& newPos) override;
//};
#define LANCHERHEADER(ClassName) \
class ClassName : public CBaseLauncher { \
public:  \
	ClassName(int rank, const CPos& pos);  \
	virtual ~ClassName() override;  \
	virtual void Action(const CPos& newPos) override;  \
};  \

////CLauncher001::CLauncher001(int rank, const CPos& pos) : CBaseLauncher(rank, pos){}
////CLauncher001::~CLauncher001(){}
#define LANCHERCPP(ClassName) \
ClassName::ClassName(int rank, const CPos& pos) : CBaseLauncher(rank, pos){}; \
ClassName::~ClassName(){}; \
