#pragma once

#include "BaseEnemy.h"

// 上から画面上に移動して、弾をうって退場する敵
class CEnemyS01 : public CBaseEnemy {
public:
	CEnemyS01(const CPos& pos);
	~CEnemyS01();
	virtual void Draw();
};

enum ShotTypeS01 {
	Rapid=0,
	Slow
};

class CLauncherS01 : public CBaseLauncher {
public:
	ShotTypeS01 m_type;
	CLauncherS01(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type);
	virtual ~CLauncherS01() override;
	virtual void Action(const CPos& newEnemyPos) override;
}; 

