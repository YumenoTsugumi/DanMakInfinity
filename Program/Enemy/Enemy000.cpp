#include "Enemy000.h"
#include "DXFunc.h"

Enemy000::Enemy000(const CPos& pos) : BaseEnemy(pos){

}
Enemy000::~Enemy000() {

}

void Enemy000::Action() {
	if (m_moveComponent) {
		m_moveComponent->Action(m_pos);
	}

	for (Launcher& l : m_launchers) {
		l.m_launcher->Action(m_pos);
	}
}
void Enemy000::Draw() {
	if (m_moveComponent) {
		//m_moveComponent->DebugPrint();
	}
	int cr = GetColor(0, 0, 255);    // 青色の値を取得
	CDxFunc::DrawCircle(m_pos, 32, 64, cr, TRUE);
}

// ダメージを受けたとき
void Enemy000::Damaged() {

}

// 死んだ時
void Enemy000::Die() {

}
