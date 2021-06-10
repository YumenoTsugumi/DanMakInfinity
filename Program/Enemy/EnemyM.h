#pragma once

#include "BaseEnemy.h"

class CLauncherM01;
// 固定5WAY
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


//-------------------------------------------------
// エヴァッカニア第二の扇
class CEnemyM02 : public CBaseEnemy {
public:
	CEnemyM02(const CPos& pos);
	~CEnemyM02();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM02 : public CBaseLauncher {
public:
	CLauncherM02(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM02() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
}; 

//-------------------------------------------------
// ドルフィンみたいな　２WAY時期狙い
class CEnemyM03 : public CBaseEnemy {
public:
	CEnemyM03(const CPos& pos);
	~CEnemyM03();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM03 : public CBaseLauncher {
public:
	CLauncherM03(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM03() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


//-------------------------------------------------
// ランダム弾5発
class CEnemyM04 : public CBaseEnemy {
public:
	CEnemyM04(const CPos& pos);
	~CEnemyM04();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM04 : public CBaseLauncher {
public:
	CLauncherM04(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM04() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


//-------------------------------------------------
// V弾
class CEnemyM05 : public CBaseEnemy {
public:
	CEnemyM05(const CPos& pos);
	~CEnemyM05();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM05 : public CBaseLauncher {
public:
	CLauncherM05(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM05() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


//-------------------------------------------------
// 芋虫連なり弾
class CEnemyM06 : public CBaseEnemy {
public:
	CEnemyM06(const CPos& pos);
	~CEnemyM06();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM06 : public CBaseLauncher {
public:
	double m_speed;
	double m_deltaSpeed;
	CLauncherM06(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM06() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

//-----------------------------------------------
// 双顎
class CLauncherM07;
class CEnemyM07 : public CBaseEnemy {
public:
	CLauncherM07* launcher;
	CEnemyM07(const CPos& pos);
	~CEnemyM07();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM07 : public CBaseLauncher {
public:
	CLauncherM07(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM07() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


//-------------------------------------------------
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

//-------------------------------------------------
// 幅広7WAY
class CEnemyM09 : public CBaseEnemy {
public:
	CEnemyM09(const CPos& pos);
	~CEnemyM09();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM09 : public CBaseLauncher {
public:
	double m_shotCount;
	CLauncherM09(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM09() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


//-------------------------------------------------
// 345WAY
class CEnemyM10 : public CBaseEnemy {
public:
	CEnemyM10(const CPos& pos);
	~CEnemyM10();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherM10 : public CBaseLauncher {
public:
	double m_shotCount;
	CLauncherM10(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherM10() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

