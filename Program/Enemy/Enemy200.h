#pragma once

#include "BaseEnemy.h"

// 大型機

class CEnemy200 : public CBaseEnemy {
public:
	CEnemy200(const CPos& pos);
	~CEnemy200();
	virtual void Draw();

	CImage* image;
};

