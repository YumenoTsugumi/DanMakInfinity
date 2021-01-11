#pragma once

#include "BaseEnemy.h"

// 大型機

class CEnemyL001 : public CBaseEnemy {
public:
	CEnemyL001(const CPos& pos);
	~CEnemyL001();
	virtual void Draw();

	CImage* image;
};

