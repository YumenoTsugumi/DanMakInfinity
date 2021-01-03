﻿#include "Enemy000.h"
#include "DXFunc.h"

#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"

//----------------------------------------------------------------------------------------------------------
// Enemy000
//----------------------------------------------------------------------------------------------------------
CEnemy000::CEnemy000(const CPos& pos) : CBaseEnemy(pos){
	image = (CImage*)CGame::GetResource("enemy10r");

	CPos targetPos = m_pos;
	targetPos.y += 200 + CFunc::RandI(0,50);

	CInOutMotion* move = new CInOutMotion(this->m_pos, targetPos, 7, 10, 180);
	SetBehaviorComponent(move);
	AddLauncher(CPos(0, 0), new CLauncher000(0, m_pos, CPos(0,30)));
}
CEnemy000::~CEnemy000() {

}

//void CEnemy000::Action() {
//	__super::Action();
//}
void CEnemy000::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), image->m_iamge, TRUE, FALSE);
}

//// ダメージを受けたとき
//void CEnemy000::Damaged() {
//	__super::Damaged();
//}
//
//// 死んだ時
//void CEnemy000::Die() {
//	__super::Die();
//}



//----------------------------------------------------------------------------------------------------------
// Enemy001
//----------------------------------------------------------------------------------------------------------
CEnemy001::CEnemy001(const CPos& pos, int formationIndex) : CBaseEnemy(pos) {
	image = (CImage*)CGame::GetResource("enemy03r");

	CPos targetPos = m_pos;
	targetPos.y += 200 + CFunc::RandI(0, 50);

	CBezierBehavior* move = new CBezierBehavior(CPos(-100, -100), CPos(600, 400), CPos(200, 100), CPos(200, 800), 5.0);
	SetBehaviorComponent(move, formationIndex*30);
	AddLauncher(CPos(0, 0), new CLauncher000(0, m_pos, CPos(0, 30)));
}
CEnemy001::~CEnemy001() {

}

void CEnemy001::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), image->m_iamge, TRUE, FALSE);
}

