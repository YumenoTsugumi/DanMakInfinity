// 
// MoveComponentを使って、複雑な動きを定義する
//

#pragma once

#include "Func.h"
#include "MoveComponent.h"


// １つの移動制御用
enum BehaviorStatus {
	Wait = 0, // 待機
	Admitting , // 入場中
	Shot, // 発射準備OK
	Leaving, // 退場（不要かも）
};

class CBehaviorComponent {
public:
	CBehaviorComponent();
	virtual ~CBehaviorComponent() ;
	virtual void Action(CPos& updatePos) ;
	virtual void DebugPrint() ;
	virtual double GetDirection() = 0;
	virtual BehaviorStatus GetBehaviorStatus() = 0;
	void SetWaitTime(int waitTime);
protected:
	int m_waitTime;
	int m_waitCount;
};


//-------------------------------------------------------------------------------------
// 小型機
//-------------------------------------------------------------------------------------
// A地点から出てきて、うって、A地点に去る
class CInOutMotion : public CBehaviorComponent {
public:
	CInOutMotion(const CPos& initPos, const CPos& targetPos, double inSpeed, double outSpeed, int shotTime);
	virtual ~CInOutMotion();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
	virtual double GetDirection();
	virtual BehaviorStatus GetBehaviorStatus();
protected:

	int m_shotTime; // 撃つ時間
	int m_shotCount;  // 撃つ時間の計測

	CPos m_initPos;
	CPos m_targetPos;

	double m_inSpeed;
	double m_outSpeed;

	BehaviorStatus m_moveStatus; // 移動状態
	CCVLM_CertainAmountStop m_inMove;
	CCVLM_CertainAmountStop m_outMove;
};

//-------------------------------------------------------------------------------------
// ベジエ曲線
//-------------------------------------------------------------------------------------
// ベジエ曲線のテスト
class CBezierBehavior : public CBehaviorComponent {
public:
	CBezierBehavior(const CPos& st, const CPos& p1, const CPos& p2, const CPos& ed, double speed);
	CBezierBehavior(const std::vector<CPos>& posArray, double speed);
	virtual ~CBezierBehavior();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
	virtual double GetDirection();
	virtual BehaviorStatus GetBehaviorStatus();
protected:
	CBezierMotion m_move;
};
