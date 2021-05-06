#pragma once

#include "BaseEnemy.h"

enum LauncherPos {
	LauncherLeft = 0,
	LauncherRight = 1,
};

// 上から画面上に移動して、弾をうって退場する敵
class CEnemyL01 : public CBaseEnemy {
public:
	double m_shotAngleL;
	double m_shotAngleR;
	CEnemyL01(const CPos& pos);
	~CEnemyL01();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherL01a : public CBaseLauncher {
public:
	CLauncherL01a(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherL01a() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherL01b : public CBaseLauncher {
public:
	LauncherPos m_lr;
	CLauncherL01b(int rank, const CPos& enemyPos, const CPos& relativePos, LauncherPos lr);
	virtual ~CLauncherL01b() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

