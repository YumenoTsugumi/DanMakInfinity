#pragma once

#include "resource.h"
#include "StageManager.h"

enum StartLRPos {
	Left = 0,
	Right = 1,
	Top = 2,
};
// ---------------------------
// 編隊バラバラパターン
// ---------------------------
class SpawnerMoveStopMove : public SpawnerBase {
public:
	SpawnerMoveStopMove(EnemySize spawnerSize);
	~SpawnerMoveStopMove();
	CInOutBehavior* GetReturnPettern(int spawnerIndex, const CPos& pos, const CPos& targetPos);

	double m_returnAngle;
	int m_returnPettern; // 帰りのパターン
	int m_random; // なんか乱数
};

// 上から真ん中まで動いて停止　その後撤退 
class SpawnerSmallTop_Stop : public SpawnerMoveStopMove {
public:								
	SpawnerSmallTop_Stop(EnemySize spawnerSize);
	virtual ~SpawnerSmallTop_Stop();					
	virtual void Spawne() override;				
	double m_appearancePosition; // 出現位置誤差
};
// 左右から真ん中まで動いて停止　その後撤退 
class SpawnerSmallLeftRight_Stop : public SpawnerMoveStopMove {
public:
	SpawnerSmallLeftRight_Stop(EnemySize spawnerSize);
	virtual ~SpawnerSmallLeftRight_Stop();
	virtual void Spawne() override;

	StartLRPos m_LRPettern; // 出現パターン
	double m_spawnerTargetPosX; // 出現後の移動先の位置
	double m_appearancePosition; // 出現位置誤差
};

// 上から真ん中まで動いて停止　その後撤退 (Line)
class SpawnerSmall_Line_Top_Stop : public SpawnerMoveStopMove {
public:
	SpawnerSmall_Line_Top_Stop(EnemySize spawnerSize);
	virtual ~SpawnerSmall_Line_Top_Stop();
	virtual void Spawne() override;
	int m_inPettern;// inパターン
	std::vector<CPos> m_inPos; // in位置

	int m_spawnerYPettern; //Yのパターン
	double m_goalY;

	double m_appearancePosition; // 出現位置誤差
};
// 左右から真ん中まで動いて停止　その後撤退 (Line)
class SpawnerSmall_Line_LeftRight_Stop : public SpawnerMoveStopMove {
public:
	SpawnerSmall_Line_LeftRight_Stop(EnemySize spawnerSize);
	virtual ~SpawnerSmall_Line_LeftRight_Stop();
	virtual void Spawne() override;
	StartLRPos m_LRPettern; // 出現パターン

	int m_inPettern;// inパターン
	std::vector<CPos> m_inPos; // in位置
	std::vector<CPos> m_targetPos; // in位置
	double m_haba; // Line編隊の幅

	int m_spawnerXPettern; //Xのパターン
	double m_goalX;

	double m_appearancePosition; // 出現位置誤差
};

// ------------------------------------------------------------------------------------------------------------
// 止まらず動き続けるパターン
// ------------------------------------------------------------------------------------------------------------
// 画面左から出てきて、右に行って左に帰ってくる つ
// 画面上から出てきて、下に行って上に帰ってくる U
// 画面右から出てきて、左に行って右に帰ってくる
class SpawnerSmallTop_NoStop_Uturn : public SpawnerBase {
public:
	SpawnerSmallTop_NoStop_Uturn(EnemySize spawnerSize);
	virtual ~SpawnerSmallTop_NoStop_Uturn();
	virtual void Spawne() override;
	std::vector<CPos> m_posAry;
	StartLRPos m_spawnerPosition; // 出現位置
	double m_appearancePosition; // 出現位置誤差
};

// 画面左から出てきて、上にいく
// 画面右から出てきて、上にいく
class SpawnerSmallTop_NoStop_LRTurn : public SpawnerBase {
public:
	SpawnerSmallTop_NoStop_LRTurn(EnemySize spawnerSize);
	virtual ~SpawnerSmallTop_NoStop_LRTurn();
	virtual void Spawne() override;
	std::vector<CPos> m_posAry;
	StartLRPos m_spawnerPosition; // 出現位置
};

// 画面左から出てきて、うねうねしながら
// 画面右から出てきて、うねうねしながら
class SpawnerSmallTop_NoStop_LRCos : public SpawnerBase {
public:
	SpawnerSmallTop_NoStop_LRCos(EnemySize spawnerSize);
	virtual ~SpawnerSmallTop_NoStop_LRCos();
	virtual void Spawne() override;
	std::vector<CPos> m_posAry;
	StartLRPos m_spawnerPosition; // 出現位置
};


// ずっとプレイヤーを時期狙い
class SpawnerSmallTop_TracePlayer : public SpawnerBase {
public:
	SpawnerSmallTop_TracePlayer(EnemySize spawnerSize);
	virtual ~SpawnerSmallTop_TracePlayer();
	virtual void Spawne() override;
	std::vector<CPos> m_posAry;
};



// ---------------------------
// 止まらず動き続けるパターン
// ---------------------------
// 上から出てきて、ゆっくり下がって、そのまま真下に行く
//class SpawnerSmallTop_NoStop : public SpawnerBase {
//public:
//	SpawnerSmallTop_NoStop(EnemySize spawnerSize);
//	virtual ~SpawnerSmallTop_NoStop();
//	virtual void Spawne() override;
//	double m_returnAngle;
//	int m_returnPettern; // 帰りのパターン
//	double m_appearancePosition; // 出現位置誤差
//};
//
//
//	
//class SpawnerSmallTop_Repeat_Stop : public SpawnerBase {
//public:
//	SpawnerSmallTop_Repeat_Stop(EnemySize spawnerSize);
//	virtual ~SpawnerSmallTop_Repeat_Stop();
//	virtual void Spawne() override;
//
//	int spawnerPettern; // 出現パターン
//	double spawnerTargetPosX; // 出現後の移動先の位置
//	double m_appearancePosition; // 出現位置誤差
//
//	int returnPettern; // 帰りのパターン
//};
//
//
//class SpawnerMedium : public SpawnerBase {	
//public:										
//	SpawnerMedium(EnemySize spawnerSize);
//	virtual ~SpawnerMedium();				
//	virtual void Spawne() override;		
//
//	int spawnerPettern; // 出現パターン
//	double m_appearancePosition; // 出現位置誤差
//
//	virtual CBaseEnemy* GetEnemy(const CPos& pos);
//};											
