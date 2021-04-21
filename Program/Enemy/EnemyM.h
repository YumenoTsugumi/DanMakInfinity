#pragma once

#include "BaseEnemy.h"

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
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
}; 

