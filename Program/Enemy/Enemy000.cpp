#include "Enemy000.h"
#include "DXFunc.h"

#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"

//----------------------------------------------------------------------------------------------------------
// Enemy000
//----------------------------------------------------------------------------------------------------------
CEnemy000::CEnemy000(const CPos& pos) : CBaseEnemy(pos){
	image = (CImage*)CGame::GetResource("enemy10r");

	std::vector<Collision> collisions = { Collision(CPos(0,0), 48.0) };
	Init(1000, Small, collisions);

	AddLauncher(CPos(0, 0), new CLauncher000(0, m_pos, CPos(0,30)));
}
CEnemy000::~CEnemy000() {

}

void CEnemy000::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), image->m_iamge, TRUE, FALSE);
}

//----------------------------------------------------------------------------------------------------------
// Enemy001
//----------------------------------------------------------------------------------------------------------
CEnemy001::CEnemy001(const CPos& pos) : CBaseEnemy(pos) {
	image = (CImage*)CGame::GetResource("enemy03r");

	std::vector<Collision> collisions = { Collision(CPos(0,0), 48.0) };
	Init(1000, Small, collisions);

	CPos targetPos = m_pos;
	targetPos.y += 200 + CFunc::RandI(0, 50);


	AddLauncher(CPos(0, 0), new CLauncher000(100, m_pos, CPos(0, 30)));
}
CEnemy001::~CEnemy001() {

}

void CEnemy001::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), image->m_iamge, TRUE, FALSE);
}

