
#include "MoveComponent.h"

// デバッグ用(本来ここには不要なヘッダー)
#include "DxLib.h"

//-------------------------------------------------------------------------------------
// 直線移動
//-------------------------------------------------------------------------------------
CConstantVelocityLinearMotion::CConstantVelocityLinearMotion(double angle, double speed) :
	CMoveComponent(),
	m_angle(0.0),
	m_speed(0.0),
	m_saveDirection(0.0)
{
	m_angle = angle / CFunc::RAD;
	m_speed = speed;
	m_vel.x = m_speed * cos(m_angle);
	m_vel.y = m_speed * sin(m_angle);
}
CConstantVelocityLinearMotion::~CConstantVelocityLinearMotion() {
}
ArrivalStatus CConstantVelocityLinearMotion::Action(CPos& updatePos) {
	updatePos += m_vel;
	return ArrivalStatus::Moving;
}
void CConstantVelocityLinearMotion::DebugPrint() {

};

double CConstantVelocityLinearMotion::GetDirection()
{
	if (m_vel.Length() <= CFunc::TOLZERO) {
		return m_saveDirection;
	}
	m_saveDirection = CFunc::GetOneVectorAngle(m_vel);
	return m_saveDirection;
}

//--------------------------------------
// 直線移動(一定距離動いたら停止)
CCVLM_DistanceStop::CCVLM_DistanceStop(double angle, double speed, double distance) :
	CConstantVelocityLinearMotion(angle, speed),
	m_maxDistance(distance),
	m_moveDistance(0.0)
{
}
CCVLM_DistanceStop::~CCVLM_DistanceStop() {}
ArrivalStatus CCVLM_DistanceStop::Action(CPos& updatePos) {
	if (m_moveDistance <= m_maxDistance) {
		updatePos += m_vel;
		m_moveDistance += m_vel.Length();
		return ArrivalStatus::Moving;
	}
	return ArrivalStatus::Arrival;
}
void CCVLM_DistanceStop::DebugPrint() {

}

//--------------------------------------
// 直線移動(特定の位置までいったら停止)
CCVLM_CertainAmountStop::CCVLM_CertainAmountStop(const CPos& initPos, const CPos& targetPos, double speed) :
	CConstantVelocityLinearMotion(0.0, speed),
	m_moveDistance(0),
	m_initPos(initPos),
	m_targetPos(targetPos),
	m_departureAcceFlag(false),
	m_departureAcce(0.0),
	m_departureMaxSpeed(0.0),
	m_arrivalAcceFlag(false),
	m_arrivalAcceDistance(0.0),
	m_arrivalAcce(0.0),
	m_arrivalMinSpeed(0.0)
{
	m_angle = (CFunc::GetTwoPointAngle(m_initPos, m_targetPos) + (180.0 / CFunc::RAD));
	m_maxDistance = CFunc::GetDistanceSqrt(m_initPos, m_targetPos);
	m_vel.x = m_speed * cos(m_angle);
	m_vel.y = m_speed * sin(m_angle);
}
CCVLM_CertainAmountStop::~CCVLM_CertainAmountStop() {

}
ArrivalStatus CCVLM_CertainAmountStop::Action(CPos& updatePos) {

	if (m_moveDistance >= m_maxDistance) {
		return ArrivalStatus::Arrival; // 目的地までの距離を移動したら、これ以上移動しない
	}

	if (m_departureAcceFlag) {
		// 加速度未満なら、加速する
		if (m_speed < m_departureMaxSpeed) {
			m_speed += m_departureAcce;
		}
		else {
			m_speed = m_departureMaxSpeed;
			m_departureAcceFlag = false;
		}
	}
	if (!m_departureAcceFlag && m_arrivalAcceFlag && m_moveDistance >= m_arrivalAcceDistance) {
		// 減速距離に到達していて、減速していれば
		if (m_speed > m_arrivalMinSpeed) {
			m_speed += m_arrivalAcce;
			if (m_departureMaxSpeed == 0.0) {
				if (m_speed < 0.0) {
					m_moveDistance = DBL_MAX;
					m_speed = 0.0;
				}
			}
		}
		else {
			m_speed = m_departureMaxSpeed;
			m_moveDistance = DBL_MAX;
			m_arrivalAcceFlag = false;
		}
	}

	m_vel.x = m_speed * cos(m_angle);
	m_vel.y = m_speed * sin(m_angle);
	updatePos += m_vel;
	m_moveDistance += m_vel.Length();
	return ArrivalStatus::Moving;
}
void CCVLM_CertainAmountStop::DebugPrint() {

}
void CCVLM_CertainAmountStop::SetDepartureAcce(double inAcce, double inMaxSpeed) {
	m_departureAcceFlag = true;
	m_departureAcce = inAcce;
	m_departureMaxSpeed = inMaxSpeed;
}
void CCVLM_CertainAmountStop::SetArrivalAcce(double outAcce, double outMinSpeed, double outAcceTimingRatio/*=0.1*/)
{
	m_arrivalAcceFlag = true;
	m_arrivalAcceDistance = m_maxDistance * outAcceTimingRatio;
	m_arrivalAcce = outAcce;
	m_arrivalMinSpeed = outMinSpeed;
}

//-------------------------------------------------------------------------------------
// ベジエ曲線
//-------------------------------------------------------------------------------------
CBezierMotion::CBezierMotion(const CPos& st, const CPos& p1, const CPos& p2, const CPos& ed, double speed) : CMoveComponent(),
m_speed(speed)
{
	m_vezier = CFunc::GetBezierCurvePointList(st, p1, p2, ed, 32);

	m_nowPos = m_vezier[0];
	m_nextPosIndex = 1;
}

CBezierMotion::~CBezierMotion(){}

ArrivalStatus CBezierMotion::Action(CPos& updatePos)
{
	if (m_arrival) {
		return ArrivalStatus::Arrival;
	}
	//①現在地から、speedを超える値を探す
	// speed6なら
	//  12345|
	// p---p---p---p---p
	double tempSpeed = m_speed;
	ArrivalStatus arrivalStatus = ArrivalStatus::Moving;
	do {
		CPos nextPos = m_vezier[m_nextPosIndex];
		double dist = CFunc::GetDistanceSqrt(m_nowPos, nextPos);
		if (dist > tempSpeed) {
			// 次の点が移動量より遠くにあれば
			double angle = (CFunc::GetTwoPointAngle(m_nowPos, nextPos) + CFunc::ToRad(180.0));
			m_vel.x = m_speed * cos(angle);
			m_vel.y = m_speed * sin(angle);
			m_nowPos += m_vel;
			break;
		}
		else {
			// 次の点が移動量以内にあれば、同じことを次の点で行う
			// ただしspeed値を減算して、その分移動したことにする
			tempSpeed -= dist;
			m_nextPosIndex++;

			if (m_nextPosIndex >= m_vezier.size()) { // 最後の点を超えてしまった
				m_nowPos = m_vezier[m_vezier.size() - 1];
				arrivalStatus = ArrivalStatus::Arrival; // 到着！
				m_arrival = true;
				break;
			}
		}
	} while (1);

	updatePos = m_nowPos;


	return arrivalStatus;
}

double CBezierMotion::GetDirection()
{
	if (m_vel.Length() <= CFunc::TOLZERO) {
		return m_saveDirection;
	}
	m_saveDirection = CFunc::GetOneVectorAngle(m_vel);
	return m_saveDirection;
}

void CBezierMotion::DebugPrint()
{
	unsigned int cr = GetColor(255, 0, 0);    // 白色の値を取得
	for (int ii = 0; ii < m_vezier.size()-1; ii++) {
		CPos p1 = m_vezier[ii];
		CPos p2 = m_vezier[ii + 1];
		DrawLine(p1.x, p1.y, p2.x, p2.y, cr);    // 線を描画
	}
}