#include "BaseEnemy.h"

BaseEnemy::BaseEnemy(const CPos& pos):
	m_pos(pos)
{

}
BaseEnemy::~BaseEnemy() {

}

void BaseEnemy::Action() {

}
void BaseEnemy::Draw() {

}

// ダメージを受けたとき
void BaseEnemy::Damaged() {

} 

// 死んだ時
void BaseEnemy::Die() {

}

void BaseEnemy::SetMoveComponent(MoveComponent* component)
{
	m_moveComponent = component;
}
void BaseEnemy::AddLauncher(const CPos& pos, BaseLauncher* launcher)
{
	Launcher l(pos, launcher);
	m_launchers.push_back(l);
}
