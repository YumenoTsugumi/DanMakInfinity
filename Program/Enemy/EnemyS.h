#pragma once

#include "BaseEnemy.h"

// 時機狙い
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
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherS01_50 : public CBaseLauncher {
public:
	CLauncherS01_50(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS01_50() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

//----------------------------------------------------------------------------------------------------------
// CEnemyS02
//----------------------------------------------------------------------------------------------------------
// nWay弾
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
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

//----------------------------------------------------------------------------------------------------------
// CEnemyS03
//----------------------------------------------------------------------------------------------------------
// 2つから時期狙い
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
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};
class CLauncherS03 : public CBaseLauncher {
public:
	CLauncherS03(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS03() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

//----------------------------------------------------------------------------------------------------------
// CEnemyS04
//----------------------------------------------------------------------------------------------------------
// ナイトメア弾
class CLauncherS04;
class CEnemyS04 : public CBaseEnemy {
public:
	CLauncherS04* launcher;
	CEnemyS04(const CPos& pos);
	~CEnemyS04();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherS04 : public CBaseLauncher {
public:
	bool m_shotAngleRock; // ショット方向固定
	double m_shotAngle;
	double m_shotSpeed; 
	CLauncherS04(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS04() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


//----------------------------------------------------------------------------------------------------------
// CEnemyS05
//----------------------------------------------------------------------------------------------------------
// 角度制限弾
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
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};


//----------------------------------------------------------------------------------------------------------
// CEnemyS06
//----------------------------------------------------------------------------------------------------------
// 時期方向にバラまき弾
class CEnemyS06 : public CBaseEnemy {
public:
	CEnemyS06(const CPos& pos);
	~CEnemyS06();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherS06 : public CBaseLauncher {
public:
	CLauncherS06(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS06() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};

//----------------------------------------------------------------------------------------------------------
// CEnemyS07
//----------------------------------------------------------------------------------------------------------
// 時期狙い狭い3way　いわゆるワイバーン
class CEnemyS07 : public CBaseEnemy {
public:
	CEnemyS07(const CPos& pos);
	~CEnemyS07();
	virtual void Draw();
	virtual double GetFinalDirectionRad() override; // 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
};

class CLauncherS07 : public CBaseLauncher {
public:
	CLauncherS07(int rank, const CPos& enemyPos, const CPos& relativePos);
	virtual ~CLauncherS07() override;
	virtual bool Action(const CPos& newEnemyPos, const CPos& nowRelativePos) override;
};