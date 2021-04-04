#pragma once

#include "BaseEnemy.h"

class CEnemyS01 : public CBaseEnemy {
public:
	CEnemyS01(const CPos& pos);
	~CEnemyS01();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
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
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
}; 



class CEnemyS02 : public CBaseEnemy {
public:
	CEnemyS02(const CPos& pos);
	~CEnemyS02();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherS02 : public CBaseLauncher {
public:
	CLauncherS02(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type);
	virtual ~CLauncherS02() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};



class CEnemyS03 : public CBaseEnemy {
public:
	CEnemyS03(const CPos& pos);
	~CEnemyS03();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherS03a : public CBaseLauncher {
public:
	CLauncherS03a(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type);
	virtual ~CLauncherS03a() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherS03b : public CBaseLauncher {
public:
	CLauncherS03b(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type);
	virtual ~CLauncherS03b() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};





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
	CLauncherS05(int rank, const CPos& enemyPos, const CPos& relativePos, ShotTypeS01 type);
	virtual ~CLauncherS05() override;
	virtual void Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};