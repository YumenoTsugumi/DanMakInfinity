#include "BaseEnemy.h"

CBaseEnemy::CBaseEnemy(const CPos& pos):
	m_pos(pos),
	m_shotTiming(false)
{

}
CBaseEnemy::~CBaseEnemy() {

}

void CBaseEnemy::Action() {
	m_behaviorComponent->Action(m_pos);

	BehaviorStatus behaviorStatus = m_behaviorComponent->GetBehaviorStatus();
	if (BehaviorStatus::Shot == behaviorStatus || m_shotTiming) {
		Shot();
	}
}
void CBaseEnemy::Draw() {

}

void CBaseEnemy::Shot()
{
	for (Launcher& l : m_launchers) {
		l.m_launcher->Action(m_pos);
	}
}

// ダメージを受けたとき
void CBaseEnemy::Damaged() {

} 

// 死んだ時
void CBaseEnemy::Die() {

}
void CBaseEnemy::DebugPrint()
{
	m_behaviorComponent->DebugPrint();
}


// 移動制御を設定
void CBaseEnemy::SetBehaviorComponent(CBehaviorComponent* component, int waitTime/*=0*/)
{
	m_behaviorComponent = component;
	if (waitTime > 0) {
		m_behaviorComponent->SetWaitTime(waitTime);
	}
}
// 砲台を設定
void CBaseEnemy::AddLauncher(const CPos& pos, CBaseLauncher* launcher)
{
	Launcher l(pos, launcher);
	m_launchers.push_back(l);
}
