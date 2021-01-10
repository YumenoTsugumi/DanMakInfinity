
#include "MoveComponent.h"
#include "BehaviorComponent.h"




CBehaviorComponent::CBehaviorComponent() :
	m_waitTime(0), m_waitCount(0) 
{
}
CBehaviorComponent::~CBehaviorComponent() {
};

void CBehaviorComponent::Action(CPos& updatePos) {
}
void CBehaviorComponent::DebugPrint() {
};
void CBehaviorComponent::SetWaitTime(int waitTime) {
	m_waitTime = waitTime;
}


//-------------------------------------------------------------------------------------
// 小型機
//-------------------------------------------------------------------------------------
// A地点から出てきて、うって、A地点に去る
CInOutBehavior::CInOutBehavior(const CPos& initPos, const CPos& targetPos, double inSpeed, double outSpeed, int shotTime) :
	CBehaviorComponent(),
	m_shotTime(shotTime),
	m_shotCount(0),
	m_inMove(initPos, targetPos, inSpeed), // 入場時の設定
	m_outMove(targetPos, initPos, outSpeed) // 退場時の設定
{
	m_initPos = initPos;
	m_targetPos = targetPos;
	m_inSpeed = inSpeed;
	m_outSpeed = outSpeed;

	m_moveStatus = BehaviorStatus::Admitting;
	m_inMove.SetArrivalAcce(-0.5, 0, 0.7); // 入場時で目的地近くでは減速する
	m_outMove.SetDepartureAcce(+0.1, outSpeed); // 退場するときの加速度
}

CInOutBehavior::~CInOutBehavior()
{

}

void CInOutBehavior::Action(CPos& updatePos)
{
	if (m_waitTime > 0) {
		if (m_waitCount <= m_waitTime) {
			m_waitCount++;
			return;
		}
	}

	if (m_moveStatus == BehaviorStatus::Admitting) {
		ArrivalStatus arrivalStatus = m_inMove.Action(updatePos);
		if (arrivalStatus == ArrivalStatus::Arrival) {
			m_moveStatus = BehaviorStatus::Shot;
		}
	}
	else if (m_moveStatus == BehaviorStatus::Shot) {
		if (m_shotCount > m_shotTime) {
			m_moveStatus = BehaviorStatus::Leaving;
		}
		m_shotCount++;
		return;
	}
	else {
		ArrivalStatus arrivalStatus = m_outMove.Action(updatePos);
	}
}

BehaviorStatus CInOutBehavior::GetBehaviorStatus()
{
	return m_moveStatus;
}

double CInOutBehavior::GetDirection()
{

	if (m_moveStatus == BehaviorStatus::Admitting) {
		return m_inMove.GetDirection();
	}
	else if (m_moveStatus == BehaviorStatus::Shot) {
		
	}
	else {
		return m_outMove.GetDirection();
	}

	return CFunc::ToRad(90.0);
}

void CInOutBehavior::DebugPrint()
{

}

//-------------------------------------------------------------------------------------
// ベジエ曲線
//-------------------------------------------------------------------------------------
// ベジエ曲線のテスト
CBezierBehavior::CBezierBehavior(const CPos& st, const CPos& p1, const CPos& p2, const CPos& ed, double speed) :
	CBehaviorComponent(),
	m_move(st, p1, p2, ed, speed)
{

}
CBezierBehavior::CBezierBehavior(const std::vector<CPos>& posArray, double speed) : CBehaviorComponent(),
m_move(posArray, speed)
{
}

CBezierBehavior::~CBezierBehavior()
{

}
void CBezierBehavior::Action(CPos& updatePos)
{
	if (m_waitTime > 0) {
		if (m_waitCount <= m_waitTime) {
			m_waitCount++;
			return;
		}
	}

	m_move.Action(updatePos);
}
void CBezierBehavior::DebugPrint()
{
	m_move.DebugPrint();
}
double CBezierBehavior::GetDirection()
{
	return m_move.GetDirection();
}
BehaviorStatus CBezierBehavior::GetBehaviorStatus()
{
	return BehaviorStatus::Shot;
}


