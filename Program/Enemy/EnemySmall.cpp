#include "EnemySmall.h"
#include "DXFunc.h"

#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"

//----------------------------------------------------------------------------------------------------------
// CEnemyS001
//----------------------------------------------------------------------------------------------------------
CEnemyS001::CEnemyS001(const CPos& pos) : CBaseEnemy(pos){
	image = (CImage*)CGame::GetResource("enemyS1");

	std::vector<Collision> collisions = { Collision(CPos(0,-18), 39.0) };
	Init(1000, Small, collisions);

	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(21, 31)));
	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(-21, 31)));
	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(0, 14)));
}
CEnemyS001::~CEnemyS001() {

}
void CEnemyS001::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}
//----------------------------------------------------------------------------------------------------------
// CEnemyS002
//----------------------------------------------------------------------------------------------------------
CEnemyS002::CEnemyS002(const CPos& pos) : CBaseEnemy(pos) {
	image = (CImage*)CGame::GetResource("enemyS2");

	std::vector<Collision> collisions = { Collision(CPos(0,0), 48.0) };
	Init(1000, Small, collisions);

	CPos targetPos = m_pos;
	targetPos.y += 200 + CFunc::RandI(0, 50);


	AddLauncher(CPos(0, 0), new CLauncher000(100, m_pos, CPos(0, 30)));
}
CEnemyS002::~CEnemyS002() {

}
void CEnemyS002::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}


//----------------------------------------------------------------------------------------------------------
// CEnemyS007
//----------------------------------------------------------------------------------------------------------
CEnemyS007::CEnemyS007(const CPos& pos) : CBaseEnemy(pos) {
	image = (CImage*)CGame::GetResource("enemyS7");

	std::vector<Collision> collisions = { Collision(CPos(0,5), 25.0) ,Collision(CPos(33,0), 9.0), Collision(CPos(-33,0), 9.0), Collision(CPos(45,-8), 5.0), Collision(CPos(-45,-8), 5.0) };
	Init(100000, Small, collisions);

	AddLauncher(CPos(0, 0), new CLauncher004(1, m_pos, CPos(0, 29)));
	AddLauncher(CPos(0, 0), new CLauncher004(1, m_pos, CPos(22, 22)));
	AddLauncher(CPos(0, 0), new CLauncher004(1, m_pos, CPos(-22, 22)));
}
CEnemyS007::~CEnemyS007() {

}
void CEnemyS007::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}
