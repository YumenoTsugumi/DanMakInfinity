#include "EnemySmall.h"
#include "DXFunc.h"

#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "Launcher.h"

//----------------------------------------------------------------------------------------------------------
// CEnemyS001
//----------------------------------------------------------------------------------------------------------
CEnemyS001::CEnemyS001(const CPos& pos) : CBaseEnemy(pos){
	m_image = (CImage*)CGame::GetResource("enemyS1");

	std::vector<Collision> collisions = { Collision(CPos(0,-18), 39.0) };
	Init(100, Small, collisions);

	AddLauncher(CPos(0, 0), new CLauncher000(0, m_pos, CPos(21, 31)));
	AddLauncher(CPos(0, 0), new CLauncher000(0, m_pos, CPos(-21, 31)));
	AddLauncher(CPos(0, 0), new CLauncher000(0, m_pos, CPos(0, 14)));
}
CEnemyS001::~CEnemyS001() {
}
void CEnemyS001::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}


//----------------------------------------------------------------------------------------------------------
// CEnemyS002
//----------------------------------------------------------------------------------------------------------
CEnemyS002::CEnemyS002(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS2");

	std::vector<Collision> collisions = { Collision(CPos(0,0), 28.0) };
	Init(100, Small, collisions);

	this->m_shotTiming = true; // 動いていても弾をうつ

	AddLauncher(CPos(0, 0), new CLauncher005(0, m_pos, CPos(0, 26)));
}
CEnemyS002::~CEnemyS002() {
}
void CEnemyS002::Action() {
	m_behaviorComponent->Action(m_pos);

	if (m_pos.y < 300.0) {
		BehaviorStatus behaviorStatus = m_behaviorComponent->GetBehaviorStatus();
		if (BehaviorStatus::Shot == behaviorStatus || m_shotTiming) {
			Shot();
		}
	}
}
void CEnemyS002::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}
//----------------------------------------------------------------------------------------------------------
// CEnemyS003
//----------------------------------------------------------------------------------------------------------
CEnemyS003::CEnemyS003(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS3");

	std::vector<Collision> collisions = { Collision(CPos(0,-12), 18.0), Collision(CPos(24,5), 16.0), Collision(CPos(-24,5), 16.0) };
	Init(100, Small, collisions);

	this->m_shotTiming = true; // 動いていても弾をうつ
	//AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(0, 7)));
	AddLauncher(CPos(0, 0), new CLauncher001(0, m_pos, CPos(22, 30)));
	AddLauncher(CPos(0, 0), new CLauncher001(0, m_pos, CPos(-22, 30)));
}
CEnemyS003::~CEnemyS003() {
}
void CEnemyS003::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	double angle = CFunc::GetTwoPointAngle(m_target, m_pos) - CFunc::ToRad(90.0);

	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, angle, m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}

//----------------------------------------------------------------------------------------------------------
// CEnemyS004
//----------------------------------------------------------------------------------------------------------
CEnemyS004::CEnemyS004(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS4");

	std::vector<Collision> collisions = { Collision(CPos(14,2), 24.0), Collision(CPos(-14,0), 24.0) };
	Init(1000, Small, collisions);

	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(0, 13)));
	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(20, 41)));
	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(-20, 41)));
}
CEnemyS004::~CEnemyS004() {
}
void CEnemyS004::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}

//----------------------------------------------------------------------------------------------------------
// CEnemyS005
//----------------------------------------------------------------------------------------------------------
CEnemyS005::CEnemyS005(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS5");

	std::vector<Collision> collisions = { Collision(CPos(0,8), 18.0), Collision(CPos(26,21), 16.0), Collision(CPos(-26,21), 16.0) };
	Init(1000, Small, collisions);

	int xAry[4] = { -7,-3,3,7 };
	int yAry[3] = { 7,13,19 };
	for (int ii = 0; ii < 4; ii++) {
		for (int jj = 0; jj < 3; jj++) {
			AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(ii, jj)));
		}
	}

}
CEnemyS005::~CEnemyS005() {
}
void CEnemyS005::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}

//----------------------------------------------------------------------------------------------------------
// CEnemyS006
//----------------------------------------------------------------------------------------------------------
CEnemyS006::CEnemyS006(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS6");

	std::vector<Collision> collisions = { Collision(CPos(0,-2), 18.0), Collision(CPos(36,24), 12.0), Collision(CPos(-36,24), 12.0) };
	Init(1000, Small, collisions);

	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(0, 13)));
	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(20, 41)));
	AddLauncher(CPos(0, 0), new CLauncher004(0, m_pos, CPos(-20, 41)));
}
CEnemyS006::~CEnemyS006() {
}
void CEnemyS006::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}



//----------------------------------------------------------------------------------------------------------
// CEnemyS007
//----------------------------------------------------------------------------------------------------------
CEnemyS007::CEnemyS007(const CPos& pos) : CBaseEnemy(pos) {
	m_image = (CImage*)CGame::GetResource("enemyS7");

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
	CDxFunc::MyDrawRotaGraph(m_pos, m_drawSizeRatio, m_behaviorComponent->GetDirection() - CFunc::ToRad(90.0), m_image->m_iamge, TRUE, FALSE);

	DebugCollisionDraw();
}
