#pragma once

#include "BaseEnemy.h"

class CEnemy000 : public CBaseEnemy {
public:
	CEnemy000(const CPos& pos);
	~CEnemy000();

	virtual void Action();
	virtual void Draw();
	virtual void Damaged(); // ダメージを受けたとき
	virtual void Die(); // 死んだ時


};