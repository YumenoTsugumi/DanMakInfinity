//
// 単純な動きを定義する
//
#pragma once

#include "Func.h"

// 到着して停止したか
enum ArrivalStatus {
	Moving = 0, // まだ動作中
	Arrival, // 到着した
};

class CMoveComponent {
public:
	CMoveComponent() {}
	virtual ~CMoveComponent() {}
	virtual ArrivalStatus Action(CPos& updatePos) = 0;
	virtual void DebugPrint() {};

	virtual double GetDirection() = 0;

};

//-------------------------------------------------------------------------------------
// 直線移動
//-------------------------------------------------------------------------------------

class CConstantVelocityLinearMotion : public CMoveComponent {
public:
	CConstantVelocityLinearMotion(double angle, double speed);
	virtual ~CConstantVelocityLinearMotion();
	virtual ArrivalStatus Action(CPos& updatePos);
	virtual void DebugPrint();
	virtual double GetDirection();
protected:
	double m_angle;
	double m_speed;
	CPos m_vel;
private:
	double m_saveDirection; // 速度が0になった時に方向が安定しないので、0になる直前の値を覚えておく
};

// 直線移動(一定距離動いたら停止)
class CCVLM_DistanceStop : public CConstantVelocityLinearMotion {
public:
	CCVLM_DistanceStop(double angle, double speed, double maxDistance);
	virtual ~CCVLM_DistanceStop();
	virtual ArrivalStatus Action(CPos& updatePos);
	virtual void DebugPrint();
protected:
	double m_maxDistance;
	double m_moveDistance;
};

// 直線移動(特定の位置までいったら停止)
class CCVLM_CertainAmountStop : public CConstantVelocityLinearMotion {
public:

	CCVLM_CertainAmountStop(const CPos& initPos, const CPos& targetPos, double departureSpeed);
	virtual ~CCVLM_CertainAmountStop();
	virtual ArrivalStatus Action(CPos& updatePos);
	virtual void DebugPrint();
	void SetDepartureAcce(double departureAcce, double departureMaxSpeed);
	void SetArrivalAcce(double arrivalAcce, double arrivalMinSpeed, double arrivalAcceTimingRatio = 0.8);

protected:
	CPos m_initPos; // 初期位置
	CPos m_targetPos; // 目的地
	double m_maxDistance; // 目的地までの距離
	double m_moveDistance; // 現在動いた距離

	// 加速度
	bool m_departureAcceFlag; // これが有効だったら、最初加速する
	double m_departureAcce; // 加速度
	double m_departureMaxSpeed; // 加速度の最大値

	// 減速度
	bool m_arrivalAcceFlag; // これが有効だったら、目的地に到着する前に減速する
	double m_arrivalAcceDistance; // 減速し始める残りの距離　初期位置A　目的地Bの距離が100で、残り距離が10なら減速開始する
	double m_arrivalAcce; // 減速度
	double m_arrivalMinSpeed; // 減速の最低速度
};


//-------------------------------------------------------------------------------------
// ベジエ曲線
//-------------------------------------------------------------------------------------

class CBezierMotion : public CMoveComponent {
public:
	CBezierMotion(const CPos& st, const CPos& p1, const CPos& p2, const CPos& ed, double speed);
	virtual ~CBezierMotion();
	virtual ArrivalStatus Action(CPos& updatePos);
	virtual void DebugPrint();
	virtual double GetDirection();
protected:
	double m_speed;
	CPos m_vel;

	CPos m_nowPos; // 曲線上の現在位置
	int m_nextPosIndex;// 曲線上の次の位置のインデックス

	bool m_arrival; // 到着済みか
	std::vector<CPos> m_vezier;
private:
	double m_saveDirection; // 速度が0になった時に方向が安定しないので、0になる直前の値を覚えておく
};


