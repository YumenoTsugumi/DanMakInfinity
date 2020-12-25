#include "Enemy000.h"
#include "DXFunc.h"

CEnemy000::CEnemy000(const CPos& pos) : CBaseEnemy(pos){

}
CEnemy000::~CEnemy000() {

}

void CEnemy000::Action() {
	if (m_moveComponent) {
		m_moveComponent->Action(m_pos);
	}

	for (Launcher& l : m_launchers) {
		l.m_launcher->Action(m_pos);
	}
}
void CEnemy000::Draw() {
	if (m_moveComponent) {
		//m_moveComponent->DebugPrint();
	}
	int cr = GetColor(0, 0, 255);    // 青色の値を取得
	CDxFunc::DrawCircle(m_pos, 32, 64, cr, TRUE);
}

// ダメージを受けたとき
void CEnemy000::Damaged() {

}

// 死んだ時
void CEnemy000::Die() {

}
