﻿#pragma once

#include "BaseBullet.h"
#include "BaseBeam.h"
#include "BaseEnemy.h"

class CBaseEnemy;
const int MAX_RANK = 100;

// ランクUPで　
//	発射間隔が早くなる	xxfr～xxfr
//	連弾になる			xx~xx弾
//	速度が速くなる		xx～xx

class CBaseLauncher {
public:
	CBaseLauncher(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CBaseLauncher();

	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos);

	//弾幕管理をしているマネージャ
	static CBulletManager* m_bulletManager;
	static void SetBulletManagerPointer(CBulletManager* Manager);
	static CBeamManager* m_beamManager;
	static void SetBeamManagerPointer(CBeamManager* Manager);

	CPos m_enemyPos; // 敵の座標
	CPos m_relativePos; // 敵の座標を中心としたときの発射口の座標

	int m_rank; // 1~100を想定

	void SetParent(CBaseEnemy* parent);
	CBaseEnemy* m_parent; // 親

	// ゲームシーンで毎フレーム時機の位置を更新してくれてる
	static CPos m_target;
	static void SetTarget(CPos target); 
protected:
	int m_count; // 発射のタイミングのカウンター


	// 各クラスでそれぞれ変数を定義するのがめんどくさいので、適当に定義しておく
	int iVal[10];
	double dVal[10];
	char cVal[10];
	bool bVal[10];

protected:

	double Rank10To15(); // ランクによる数値を返す 1.0-1.5
	double Rank10To20(); // ランクによる数値を返す 1.0-2.0
	double Rank10To30(); // ランクによる数値を返す 1.0-3.0
	double Rank10To40(); // ランクによる数値を返す 
	double Rank10To50(); // ランクによる数値を返す 
	double Rank10To60(); // ランクによる数値を返す 
	double Rank10To70(); // ランクによる数値を返す 
	double Rank10To80(); // ランクによる数値を返す 
	double Rank10To90(); // ランクによる数値を返す 
	double Rank10To100(); // ランクによる数値を返す 

	double RankSpeed(); // ランクによる速度変化を返す 1.0-2.0
	double RankBulletNum(); // ランクによる弾の総数を返す
	double RankSpan(); // ランクによる発射間隔を返す
	int RankWay(); // ランクによるnway弾の個数を返す
	int RankRapidA(); // ランクによる連弾の個数を返す
	int RankRapidB(); // ランクによる連弾の個数を返す

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
//	CLauncher001(int rank, const CPos& enemyPos, const CPos& relativePos);
//	virtual ~CLauncher001() override;
//	virtual void Action(const CPos& newEnemyPos) override;
//};
#define LANCHERHEADER(ClassName) \
class ClassName : public CBaseLauncher { \
public:  \
	ClassName(int rank, const CPos& enemyPos, const CPos& relativePos);  \
	virtual ~ClassName() override;  \
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;  \
};  \

////CLauncher001::CLauncher001(int rank, const CPos& enemyPos, const CPos& relativePos) : CBaseLauncher(rank, enemyPos, relativePos){}
////CLauncher001::~CLauncher001(){}
#define LANCHERCPP(ClassName) \
ClassName::ClassName(int rank, const CPos& enemyPos, const CPos& relativePos) : CBaseLauncher(rank, enemyPos, relativePos){}; \
ClassName::~ClassName(){}; \
