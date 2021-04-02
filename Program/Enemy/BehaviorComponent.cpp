
#include "MoveComponent.h"
#include "BehaviorComponent.h"
#include "BattleScene.h"



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
// A地点から出てきて、うって、B地点に去る
CInOutBehavior::CInOutBehavior(const CPos& initPos, const CPos& targetPos, const CPos& endTargetPos, double inSpeed, double outSpeed, int shotTime) :
	CBehaviorComponent(),
	m_shotTime(shotTime),
	m_shotCount(0),
	m_leaveDirToPlayer(Invalid),
	m_leaveStart(false),
	m_inMove(initPos, targetPos, inSpeed), // 入場時の設定
	m_outMove(targetPos, endTargetPos, outSpeed) // 退場時の設定
{
	m_initPos = initPos;
	m_targetPos = targetPos;
	m_endTargetPos = endTargetPos;
	m_inSpeed = inSpeed;
	m_outSpeed = outSpeed;

	m_moveStatus = BehaviorStatus::Admitting;
	m_inMove.SetArrivalAcce(-0.5, 0, 0.7); // 入場時(7割の位置)で目的地近くでは減速(0.5)する
	m_outMove.SetDepartureAcce(+0.3, outSpeed); // 退場するときの加速度
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

			if (!m_leaveStart) {
				if (m_leaveDirToPlayer == LeaveDirToPlayer::DirToPlayer || 
					m_leaveDirToPlayer == LeaveDirToPlayer::DirToPlayer2) {
					m_outMove.ResetTargetPos(updatePos, CBattleScene::GetPlayerPos(), true, m_inMove.m_speed);
					m_leaveStart = true;
				}
			}
		}
		m_shotCount++;
		return;
	}
	else {
		ArrivalStatus arrivalStatus = m_outMove.Action(updatePos);
		if (arrivalStatus == ArrivalStatus::Arrival) {
			m_moveStatus = BehaviorStatus::Finish;
		}
	}

	// 時機狙いに動きを変更
	if (m_moveStatus == BehaviorStatus::Leaving) {
		if (m_leaveDirToPlayer == LeaveDirToPlayer::DirToPlayer2) {
			m_outMove.ResetTargetPos(updatePos, CBattleScene::GetPlayerPos(), false, 0);
		}
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
// 小型機
//-------------------------------------------------------------------------------------
// 撃ちつつ止まらずに、A地点から出てきてB地点に向かう
CGoTargetBehavior::CGoTargetBehavior(const CPos& initPos, const CPos& targetPos, double inSpeed, int shotTime) :
	CBehaviorComponent(),
	m_shotTime(shotTime),
	m_shotCount(0),
	m_inMove(initPos, targetPos, inSpeed) // 入場時の設定
{
	m_initPos = initPos;
	m_targetPos = targetPos;
	m_inSpeed = inSpeed;

	m_moveStatus = BehaviorStatus::Admitting;
}

CGoTargetBehavior::~CGoTargetBehavior()
{

}

void CGoTargetBehavior::Action(CPos& updatePos)
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
			m_moveStatus = BehaviorStatus::Leaving;
		}
	}
}

BehaviorStatus CGoTargetBehavior::GetBehaviorStatus()
{
	return m_moveStatus;
}

double CGoTargetBehavior::GetDirection()
{
	if (m_moveStatus == BehaviorStatus::Admitting) {
		return m_inMove.GetDirection();
	}


	return CFunc::ToRad(90.0);
}

void CGoTargetBehavior::DebugPrint()
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


// A地点から出てきて、うって、B地点にいく(ベジエ版)
CBezierInOutBehavior::CBezierInOutBehavior(const std::vector<CPos>& inArray, const std::vector<CPos>& outArray, double inSpeed, double outSpeed, int shotTime) : CBehaviorComponent(),
m_inMove(inArray, inSpeed),
m_outMove(outArray, outSpeed),
m_leaveStart(false)
{
	m_moveStatus = BehaviorStatus::Admitting;
	m_inMove.SetArrivalAcce(-0.5, 0, 0.9); // 入場時(7割の位置)で目的地近くでは減速(0.5)する
	m_outMove.SetDepartureAcce(+0.1, outSpeed); // 退場するときの加速度
}

CBezierInOutBehavior::~CBezierInOutBehavior()
{

}
void CBezierInOutBehavior::Action(CPos& updatePos)
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

			if (!m_leaveStart) {
				m_outMove.ResetArray(m_inMove.GetNowPos());
				m_leaveStart = true;
			}
		}
		m_shotCount++;
		return;
	}
	else {
		ArrivalStatus arrivalStatus = m_outMove.Action(updatePos);
		if (arrivalStatus == ArrivalStatus::Arrival) {
			m_moveStatus = BehaviorStatus::Finish;
		}
	}
}
void CBezierInOutBehavior::DebugPrint()
{
	m_inMove.DebugPrint();
	m_outMove.DebugPrint();
}
double CBezierInOutBehavior::GetDirection()
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
BehaviorStatus CBezierInOutBehavior::GetBehaviorStatus()
{
	return BehaviorStatus::Shot;
}


