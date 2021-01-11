﻿#include "EnemyLarge.h"
#include "DXFunc.h"

#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"

// 大型機

//----------------------------------------------------------------------------------------------------------
// CEnemyL001
//----------------------------------------------------------------------------------------------------------
CEnemyL001::CEnemyL001(const CPos& pos) : CBaseEnemy(pos){
	image = (CImage*)CGame::GetResource("enemy24b");

	std::vector<Collision> collisions = { Collision(CPos(0,0), 128.0) };
	Init(5000, Large, collisions);

	CPos targetPos = pos - CPos(-1000, 100);

	AddLauncher(CPos(0, 0), new CLauncher020(0, m_pos, CPos(-30, 0), false));
	AddLauncher(CPos(0, 0), new CLauncher020(0, m_pos, CPos(+30, 0), true));
	AddLauncher(CPos(0, 0), new CLauncher002(0, m_pos, CPos(+0, 50)));
}
CEnemyL001::~CEnemyL001() {
}
void CEnemyL001::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0, CFunc::ToRad(0.0), image->m_iamge, TRUE, FALSE);
}