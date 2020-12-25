#include "BaseEnemy.h"

CBaseEnemy::CBaseEnemy(const CPos& pos):
	m_pos(pos)
{

}
CBaseEnemy::~CBaseEnemy() {

}

void CBaseEnemy::Action() {

}
void CBaseEnemy::Draw() {

}

// ダメージを受けたとき
void CBaseEnemy::Damaged() {

} 

// 死んだ時
void CBaseEnemy::Die() {

}

void CBaseEnemy::SetMoveComponent(CMoveComponent* component)
{
	m_moveComponent = component;
}
void CBaseEnemy::AddLauncher(const CPos& pos, CBaseLauncher* launcher)
{
	Launcher l(pos, launcher);
	m_launchers.push_back(l);
}
