#pragma once

#include "BaseEnemy.h"

class CLauncherM01;
// 上から画面上に移動して、弾をうって退場する敵
class CEnemyM01 : public CBaseEnemy {
public:
	CLauncherM01* launcher;
	CEnemyM01(const CPos& pos);
	~CEnemyM01();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM01 : public CBaseLauncher {
public:
	bool m_shotAngleRock; // ショット方向固定
	double m_shotAngle;
	double m_shotSpeed;
	CLauncherM01(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM01() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};



// 上から画面上に移動して、弾をうって退場する敵
class CEnemyM02 : public CBaseEnemy {
public:
	CEnemyM02(const CPos& pos);
	~CEnemyM02();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM02 : public CBaseLauncher {
public:
	double m_shotAngle;
	CLauncherM02(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM02() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
}; 

// ためうちの敵
class CLauncherM08;
class CEnemyM08 : public CBaseEnemy {
public:

	double m_nockbackLength;
	CLauncherM08* launcher;
	CEnemyM08(const CPos& pos);
	~CEnemyM08();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM08 : public CBaseLauncher {
public:
	int m_tempCount;
	bool m_shotAngleRock; // ショット方向固定
	double m_shotAngle;

	CLauncherM08(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM08() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};



