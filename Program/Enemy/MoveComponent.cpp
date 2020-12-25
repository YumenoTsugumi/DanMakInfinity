
#include "MoveComponent.h"

//-------------------------------------------------------------------------------------
// 直線移動
//-------------------------------------------------------------------------------------
CConstantVelocityLinearMotion::CConstantVelocityLinearMotion(double angle, double speed) :
	CMoveComponent(),
	m_angle(angle/CFunc::RAD),
	m_speed(speed)
{
	m_vel.x = m_speed * cos(m_angle);
	m_vel.y = m_speed * sin(m_angle);
}
CConstantVelocityLinearMotion::~CConstantVelocityLinearMotion() {}
void CConstantVelocityLinearMotion::DebugPrint() {

};

//--------------------------------------
// 直線移動(停止しない)
CCVLM_NonStop::CCVLM_NonStop(double angle, double speed) :
	CConstantVelocityLinearMotion(angle, speed)
{
}
CCVLM_NonStop::~CCVLM_NonStop() {}
void CCVLM_NonStop::Action(CPos& updatePos) {
	updatePos += m_vel;
}
void CCVLM_NonStop::DebugPrint() {

};

//--------------------------------------
// 直線移動(一定距離動いたら停止)
CCVLM_DistanceStop::CCVLM_DistanceStop(double angle, double speed, double distance) :
	CConstantVelocityLinearMotion(angle, speed),
	m_maxDistance(distance),
	m_moveDistance(0.0)
{
}
CCVLM_DistanceStop::~CCVLM_DistanceStop() {}
void CCVLM_DistanceStop::Action(CPos& updatePos) {
	if (m_moveDistance <= m_maxDistance) {
		updatePos += m_vel;
		m_moveDistance += m_vel.Length();
	}
}
void CCVLM_DistanceStop::DebugPrint() {

}

//--------------------------------------
// 直線移動(特定の位置までいったら停止)
CCVLM_CertainAmountStop::CCVLM_CertainAmountStop(const CPos& targetPos, double speed) :
	CConstantVelocityLinearMotion(0.0, speed),
	m_targetPos(targetPos),
	m_init(false)
{

}
CCVLM_CertainAmountStop::~CCVLM_CertainAmountStop() {

}
void CCVLM_CertainAmountStop::Action(CPos& updatePos) {
	if (m_init) {
		m_angle = (CFunc::GetTwoPointAngle(updatePos, m_targetPos) + (180.0 / CFunc::RAD));
		m_maxDistance = CFunc::GetDistanceSqrt(updatePos, m_targetPos);
	}
	m_vel.x = m_speed * cos(m_angle);
	m_vel.y = m_speed * sin(m_angle);
	if (m_moveDistance >= m_maxDistance) {
		updatePos += m_vel;
		m_moveDistance += m_vel.Length();
	}
}
void CCVLM_CertainAmountStop::DebugPrint() {

}


//-------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------