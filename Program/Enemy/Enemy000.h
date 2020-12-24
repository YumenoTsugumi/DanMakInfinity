#pragma once

#include "BaseEnemy.h"

class Enemy000 : public BaseEnemy {
public:
	Enemy000(const CPos& pos);
	~Enemy000();

	virtual void Action();
	virtual void Draw();
	virtual void Damaged(); // ダメージを受けたとき
	virtual void Die(); // 死んだ時


};