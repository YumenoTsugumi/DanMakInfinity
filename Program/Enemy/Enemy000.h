#pragma once

#include "BaseEnemy.h"

class CEnemy000 : public CBaseEnemy {
public:
	CEnemy000(const CPos& pos);
	~CEnemy000();
	virtual void Draw();

	CImage* image;
};


class CEnemy001 : public CBaseEnemy {
public:
	CEnemy001(const CPos& pos, int formationIndex);
	~CEnemy001();

	virtual void Draw();

	CImage* image;
};