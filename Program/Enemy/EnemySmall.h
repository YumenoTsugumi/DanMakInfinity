#pragma once

#include "BaseEnemy.h"

class CEnemyS001 : public CBaseEnemy {
public:
	CEnemyS001(const CPos& pos);
	~CEnemyS001();
	virtual void Draw();
	CImage* image;
};


class CEnemyS002 : public CBaseEnemy {
public:
	CEnemyS002(const CPos& pos);
	~CEnemyS002();

	virtual void Draw();

	CImage* image;
};




class CEnemyS007 : public CBaseEnemy {
public:
	CEnemyS007(const CPos& pos);
	~CEnemyS007();
	virtual void Draw();
	CImage* image;
};