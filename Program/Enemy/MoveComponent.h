#pragma once

#include "Func.h"

class MoveComponent {
public:
	MoveComponent() {}
	virtual ~MoveComponent() {}
	virtual void Action(CPos& updatePos) = 0;
	virtual void DebugPrint() {};
};

//-------------------------------------------------------------------------------------
// 直線移動
//-------------------------------------------------------------------------------------

class ConstantVelocityLinearMotion : public MoveComponent {
public:
	ConstantVelocityLinearMotion(double angle, double speed);
	virtual ~ConstantVelocityLinearMotion();
	virtual void Action(CPos& updatePos) = 0;
	virtual void DebugPrint();
protected:
	double m_angle;
	double m_speed;
	CPos m_vel;
};

// 直線移動(停止しない)
class CVLM_NonStop : public ConstantVelocityLinearMotion {
public:
	CVLM_NonStop(double angle, double speed);
	virtual ~CVLM_NonStop();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
};

// 直線移動(一定距離動いたら停止)
class CVLM_DistanceStop : public ConstantVelocityLinearMotion {
public:
	CVLM_DistanceStop(double angle, double speed, double maxDistance);
	virtual ~CVLM_DistanceStop();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
protected:
	double m_maxDistance;
	double m_moveDistance;
};

// 直線移動(特定の位置までいったら停止)
class CVLM_CertainAmountStop : public ConstantVelocityLinearMotion {
public:
	CVLM_CertainAmountStop(const CPos& targetPos, double speed);
	virtual ~CVLM_CertainAmountStop();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
protected:
	CPos m_targetPos;
	double m_maxDistance;
	double m_moveDistance;
	bool m_init;
};

//-------------------------------------------------------------------------------------
// 
//-------------------------------------------------------------------------------------



