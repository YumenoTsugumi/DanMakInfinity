#pragma once

#include "BaseLauncher.h"


//LANCHERHEADER(CLauncher020) // 蛾
class CLauncher020 : public CBaseLauncher {
public:
	CLauncher020(int rank, const CPos& enemyPos, const CPos& relativePos, bool left);
	virtual ~CLauncher020() override;
	virtual void Action(const CPos& newEnemyPos) override;
};


