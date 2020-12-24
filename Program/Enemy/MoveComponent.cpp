
#include "MoveComponent.h"

//-------------------------------------------------------------------------------------
// 直線移動
//-------------------------------------------------------------------------------------
ConstantVelocityLinearMotion::ConstantVelocityLinearMotion(double angle, double speed) :
	MoveComponent(),
	m_angle(angle/CFunc::RAD),
	m_speed(speed)
{
	m_vel.x = m_speed * cos(m_angle);
	m_vel.y = m_speed * sin(m_angle);
}
ConstantVelocityLinearMotion::~ConstantVelocityLinearMotion() {}
void ConstantVelocityLinearMotion::DebugPrint() {

};

//--------------------------------------
// 直線移動(停止しない)
CVLM_NonStop::CVLM_NonStop(double angle, double speed) :
	ConstantVelocityLinearMotion(angle, speed)
{
}
CVLM_NonStop::~CVLM_NonStop() {}
void CVLM_NonStop::Action(CPos& updatePos) {
	updatePos += m_vel;
}
void CVLM_NonStop::DebugPrint() {

};

//--------------------------------------
// 直線移動(一定距離動いたら停止)
CVLM_DistanceStop::CVLM_DistanceStop(double angle, double speed, double distance) :
	ConstantVelocityLinearMotion(angle, speed),
	m_maxDistance(distance),
	m_moveDistance(0.0)
{
}
CVLM_DistanceStop::~CVLM_DistanceStop() {}
void CVLM_DistanceStop::Action(CPos& updatePos) {
	if (m_moveDistance <= m_maxDistance) {
		updatePos += m_vel;
		m_moveDistance += m_vel.Length();
	}
}
void CVLM_DistanceStop::DebugPrint() {

}

//--------------------------------------
// 直線移動(特定の位置までいったら停止)
CVLM_CertainAmountStop::CVLM_CertainAmountStop(const CPos& targetPos, double speed) :
	ConstantVelocityLinearMotion(0.0, speed),
	m_targetPos(targetPos),
	m_init(false)
{

}
CVLM_CertainAmountStop::~CVLM_CertainAmountStop() {

}
void CVLM_CertainAmountStop::Action(CPos& updatePos) {
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
void CVLM_CertainAmountStop::DebugPrint() {

}


//-------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------