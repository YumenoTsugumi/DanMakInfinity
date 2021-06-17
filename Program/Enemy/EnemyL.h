#pragma once

#include "BaseEnemy.h"

enum LauncherPos {
	LauncherLeft = 0,
	LauncherRight = 1,
};

//-------------------------------------------------
//
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


//-------------------------------------------------
//
class CEnemyL02 : public CBaseEnemy {
public:
	CEnemyL02(const CPos& pos);
	~CEnemyL02();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherL02a : public CBaseLauncher {
public:
	bool m_angleDir;
	double m_angle;
	CLauncherL02a(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherL02a() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherL02b : public CBaseLauncher {
public:
	LauncherPos m_lr;
	CLauncherL02b(int rank, const CPos& enemyPos, const CPos& relativePos, LauncherPos lr);
	virtual ~CLauncherL02b() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};



//-------------------------------------------------
//
class CEnemyL03 : public CBaseEnemy {
public:
	double m_shotAngleL;
	double m_shotAngleR;
	CEnemyL03(const CPos& pos);
	~CEnemyL03();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CBaseEffect;
class CLauncherL03a : public CBaseLauncher {
public:
	// エフェクトを追従
	double m_speed;
	double m_deltaSpeed;
	double m_shotAngle;
	CLauncherL03a(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherL03a() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherL03b : public CBaseLauncher {
public:
	CLauncherL03b(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherL03b() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


//-------------------------------------------------
//
class CEnemyL04 : public CBaseEnemy {
public:
	double m_shotAngleL;
	double m_shotAngleR;
	CEnemyL04(const CPos& pos);
	~CEnemyL04();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CBaseEffect;
class CLauncherL04a : public CBaseLauncher {
public:
	// エフェクトを追従
	double m_speed;
	double m_deltaSpeed;
	double m_shotAngle;
	CLauncherL04a(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherL04a() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherL04b : public CBaseLauncher {
public:
	CLauncherL04b(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherL04b() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


