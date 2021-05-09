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
	Leaving, // 退場中（不要かも）
	Finish, // 退場済
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
// A地点から出てきて、うって、B地点にいく
class CInOutBehavior : public CBehaviorComponent {
public:
	CInOutBehavior(const CPos& initPos, const CPos& targetPos, const CPos& endTargetPos, double inSpeed, double outSpeed, int shotTime);
	virtual ~CInOutBehavior();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
	virtual double GetDirection();
	virtual BehaviorStatus GetBehaviorStatus();

	enum LeaveDirToPlayer {
		Invalid = 0,
		DirToPlayer,
		DirToPlayer2,
	};
	void SetLeaveDirToPlayer() { m_leaveDirToPlayer = DirToPlayer; }; // endTargetPosを無視して、離れる時に、プレイヤーが居た位置を対象とする
	void SetLeaveDirToPlayer2() { 
		m_leaveDirToPlayer = DirToPlayer2;
		m_outMove.SetNoArrival(true);
	}; // endTargetPosを無視して、離れる時に、プレイヤーが居る場所を参照し続ける（時期狙い）
protected:
	int m_shotTime; // 撃つ時間
	int m_shotCount;  // 撃つ時間の計測

	CPos m_initPos;
	CPos m_targetPos;
	CPos m_endTargetPos;

	double m_inSpeed;
	double m_outSpeed;

	BehaviorStatus m_moveStatus; // 移動状態
	CCVLM_CertainAmountStop m_inMove;
	CCVLM_CertainAmountStop m_outMove;

	LeaveDirToPlayer m_leaveDirToPlayer;// endTargetPosを無視して、離れる時に、プレイヤーが居た位置を対象とする

	bool m_leaveStart; // 動き始めた瞬間に座標を補正するよう
};


//-------------------------------------------------------------------------------------
// 小型機
//-------------------------------------------------------------------------------------
// 撃ちつつ、A地点から出てきてB地点に向かう(ノンストップ)
class CGoTargetBehavior : public CBehaviorComponent {
public:
	CGoTargetBehavior(const CPos& initPos, const CPos& targetPos, double inSpeed, int shotTime);
	virtual ~CGoTargetBehavior();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
	virtual double GetDirection();
	virtual BehaviorStatus GetBehaviorStatus();
protected:

	int m_shotTime; // 撃つ時間
	int m_shotCount;  // 撃つ時間の計測

	CPos m_initPos;
	CPos m_targetPos;
	CPos m_endTargetPos;

	double m_inSpeed;
	double m_outSpeed;

	BehaviorStatus m_moveStatus; // 移動状態
	CCVLM_CertainAmountStop m_inMove;
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
	BehaviorStatus m_moveStatus; // 移動状態
};


// A地点から出てきて、うって止まって、B地点にいく(ベジエ版)
//std::vector<CPos> poss = {
//CPos(-100, -100), CPos(100, 100), CPos(200, 100), CPos(300, 100),
//CPos(300, 100), CPos(600, 100), CPos(600, 200), CPos(300, 200),
//CPos(300, 200), CPos(100, 200), CPos(0, 200), CPos(-100, 200), };
class CBezierInOutBehavior : public CBehaviorComponent {
public:
	CBezierInOutBehavior(const std::vector<CPos>& inArray, const std::vector<CPos>& outArray, double inSpeed, double outSpeed, int shotTime);

	virtual ~CBezierInOutBehavior();
	virtual void Action(CPos& updatePos);
	virtual void DebugPrint();
	virtual double GetDirection();
	virtual BehaviorStatus GetBehaviorStatus();
protected:

	int m_shotTime; // 撃つ時間
	int m_shotCount;  // 撃つ時間の計測

	CPos m_initPos;
	CPos m_targetPos;
	CPos m_endTargetPos;

	double m_inSpeed;
	double m_outSpeed;

	BehaviorStatus m_moveStatus; // 移動状態

	CBezierMotion m_inMove;
	CBezierMotion m_outMove;
	bool m_leaveStart; // 動き始めた瞬間に座標を補正するよう
};
