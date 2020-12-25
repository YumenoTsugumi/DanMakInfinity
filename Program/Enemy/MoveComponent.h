#pragma once

#include "Func.h"

class CMoveComponent {
public:
	CMoveComponent() {}
	virtual ~CMoveComponent() {}
	virtual void Action(CPos& updatePos) = 0;
	virtual void DebugPrint() {};
};

//-------------------------------------------------------------------------------------
// 直線移動
//-------------------------------------------------------------------------------------

class CConstantVelocityLinearMotion : public CMoveComponent {
public:
	CConstantVelocityLinearMotion(double angle, double speed);
	virtual ~CConstantVelocityLinearMotion();
	virtual void Action(CPos& updatePos) = 0;
	virtual void DebugPrint();
protected:
	double m_angle;
	double m_speed;
	CPos m_vel;
};

// 直線移動(停止しない)
class CCVLM_NonStop : public CConstantVelocityLinearMotion {
public:
	CCVLM_NonStop(double angle, double speed);
	virtual ~CCVLM_NonStop();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
};

// 直線移動(一定距離動いたら停止)
class CCVLM_DistanceStop : public CConstantVelocityLinearMotion {
public:
	CCVLM_DistanceStop(double angle, double speed, double maxDistance);
	virtual ~CCVLM_DistanceStop();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
protected:
	double m_maxDistance;
	double m_moveDistance;
};

// 直線移動(特定の位置までいったら停止)
class CCVLM_CertainAmountStop : public CConstantVelocityLinearMotion {
public:
	CCVLM_CertainAmountStop(const CPos& targetPos, double speed);
	virtual ~CCVLM_CertainAmountStop();
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



