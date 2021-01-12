#pragma once

#include "BaseEnemy.h"

// 上から画面上に移動して、弾をうって退場する敵
class CEnemyS001 : public CBaseEnemy {
public:
	CEnemyS001(const CPos& pos);
	~CEnemyS001();
	virtual void Draw();
};

// 上から下に移動して、Xに弾を撃つ敵
class CEnemyS002 : public CBaseEnemy {
public:
	virtual void Action();
	CEnemyS002(const CPos& pos);
	~CEnemyS002();
	virtual void Draw();
};

class CEnemyS003 : public CBaseEnemy {
public:
	CEnemyS003(const CPos& pos);
	~CEnemyS003();
	virtual void Draw();
};

class CEnemyS004 : public CBaseEnemy {
public:
	CEnemyS004(const CPos& pos);
	~CEnemyS004();
	virtual void Draw();
};

class CEnemyS005 : public CBaseEnemy {
public:
	CEnemyS005(const CPos& pos);
	~CEnemyS005();
	virtual void Draw();
};

class CEnemyS006 : public CBaseEnemy {
public:
	CEnemyS006(const CPos& pos);
	~CEnemyS006();
	virtual void Draw();
};

class CEnemyS007 : public CBaseEnemy {
public:
	CEnemyS007(const CPos& pos);
	~CEnemyS007();
	virtual void Draw();
};