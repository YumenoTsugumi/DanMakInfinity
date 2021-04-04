#pragma once

#include "BaseEnemy.h"

class CEnemyS01 : public CBaseEnemy {
public:
	CEnemyS01(const CPos& pos);
	~CEnemyS01();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};
class CLauncherS01 : public CBaseLauncher {
public:
	CLauncherS01(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS01() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherS01_50 : public CBaseLauncher {
public:
	CLauncherS01_50(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS01_50() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

//----------------------------------------------------------------------------------------------------------
// CEnemyS02
//----------------------------------------------------------------------------------------------------------
class CEnemyS02 : public CBaseEnemy {
public:
	CEnemyS02(const CPos& pos);
	~CEnemyS02();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherS02 : public CBaseLauncher {
public:
	CLauncherS02(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS02() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

//----------------------------------------------------------------------------------------------------------
// CEnemyS03
//----------------------------------------------------------------------------------------------------------

class CEnemyS03 : public CBaseEnemy {
public:
	CEnemyS03(const CPos& pos);
	~CEnemyS03();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherS03_50 : public CBaseLauncher {
public:
	CLauncherS03_50(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS03_50() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherS03 : public CBaseLauncher {
public:
	CLauncherS03(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS03() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};



//----------------------------------------------------------------------------------------------------------
// CEnemyS05
//----------------------------------------------------------------------------------------------------------


class CEnemyS05 : public CBaseEnemy {
public:
	double m_drawAngle;
	CEnemyS05(const CPos& pos);
	~CEnemyS05();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherS05 : public CBaseLauncher {
public:
	CLauncherS05(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS05() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};