#pragma once

#include "resource.h"
#include "BaseEffect.h"
#include "BaseEnemy.h"

class SpawnerBase;
class CBaseEnemy;



class StageManager {
public:
	enum StageManageStatus {
		Now = 0,
		WaitingEnd = 1,
		ResultDrawWait = 2
	};

	StageManager();
	virtual ~StageManager();
	int m_count;
	void StageReset();

	StageManageStatus GetStatus() {
		return status;
	}
	StageManageStatus status;

	int m_spawneTimingSmallA;
	int m_spawneTimingSmallB;
	int m_spawneTimingMediumA;
	int m_spawneTimingLargeA;
	std::vector<SpawnerBase*> m_spawners;

	void Main();

	SpawnerBase* GetTestSpawner();

	SpawnerBase* GetRandomSpawner_SmallA(); // 動き回る+停止も含む
	SpawnerBase* GetRandomSpawner_SmallB(); // 動き回る

	SpawnerBase* GetRandomSpawner_MediumA(); // 停止のみ
	SpawnerBase* GetRandomSpawner_LargeA(); // 停止のみ

	void SetBattleScene(CBattleScene* scene) {
		m_scene = scene;
	}
	CBattleScene* m_scene;


	//------------------------------------------------------------
	// デバッガー
	int m_debugSpawneEnemySize;
	int m_debugSpawneEnemyIndex;
	int m_debugSpawneEnemyMoveType;
	void DebugIndexControl();
	int m_debugSpawneCount;
	void DebugContinueSpawner();
	SpawnerBase* DebugGetMove(int index, EnemySize size);
	std::string DebugGetMoveName(int index);
	// デバッグ
	int DebugGetSmallMax();
	int DebugGetMediumMax();
	int DebugGetLargeMax();
};

class SpawnerBase {
public:
	int m_count;
	int m_maxCount;
	int m_spawneTiming;
	bool m_deleteFlg;
	int m_spawnerIndex;

	double m_speed;

	SpawnerBase(EnemySize spawnerSize);
	virtual ~SpawnerBase();
	virtual void Spawne();

	// 1-10段階ぐらいにしとく
	// ランクで参照して、特定ランク未満だと出てこないようにするため
	// たぶん未使用
	virtual int GetPower(); // 編隊の強さ（ざっくり）

	int ToSecond(int millSecond);
	double ToSecond(double millSecond);

	// 入場する際の交点にマーカーを表示する(右側左側の順番でチェックする場合rightFirstをtrueに)
	void SetCrossMaker(const CPos& pos, const CPos& targetPos, bool topCheck = true, bool rightFirst = true);

	//// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	//// FulHDなら (480,20)(1440,1060)の範囲
	//CPos ToGamePos(CPos ratioPos);
	//CPos ToGamePos(double ratioPosX, double ratioPosY);
	//// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	//double CGame::ToGamePosX(double ratioPosX); 
	//// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	//double CGame::ToGamePosY(double ratioPosY);

	//// 0.0～1.0で画面サイズに比例する大きさが返ってくる
	//double ToGameSizeX(double ratioPosX);
	//// 0.0～1.0で画面サイズに比例する大きさが返ってくる
	//double ToGameSizeY(double ratioPosY);

	int m_maxSpawneCount; // スポーン数　小12体　中4体　大2体

	EnemySize m_spawnerSize; // 出現する敵のサイズ
	void SetSpeedBySize();

	int m_index; // 出現する敵（子クラスで使用）(コンストラクタで設定)
	void SetEnemyIndex(int index) {m_index	= index;};// (コンストラクタの値を後から上書き)




	// 入場して、攻撃して、退場する
	CBaseEnemy* GetStopEnemy(const CPos& pos);
	void SetStopSpawner(EnemySize spawnerSize);

	int GetSmallStopEnemyIndex();
	CBaseEnemy* GetSmallStopEnemy(int index, const CPos& pos);
	int GetMediumStopEnemyIndex();
	CBaseEnemy* GetMediumStopEnemy(int index, const CPos& pos);
	int GetLargeStopEnemyIndex();
	CBaseEnemy* GetLargeStopEnemy(int index, const CPos& pos);

	// 動き続ける敵
	CBaseEnemy* GetNonStopEnemy(const CPos& pos);
	void SetNonStopSpawner(EnemySize spawnerSize);

	int GetSmallNonStopEnemyIndex(); 
	CBaseEnemy* GetSmallNonStopEnemy(int index, const CPos& pos);
	int GetMediumNonStopEnemyIndex();
	CBaseEnemy* GetMediumNonStopEnemy(int index, const CPos& pos);
	int GetLargeNonStopEnemyIndex();
	CBaseEnemy* GetLargeNonStopEnemy(int index, const CPos& pos);

	// 入場して、ずっと滞在してステップ動作する
	CBaseEnemy* GetStepEnemy(const CPos& pos);
	void SetStepSpawner(EnemySize spawnerSize);

	int GetMediumStepEnemyIndex();
	CBaseEnemy* GetMediumStepEnemy(int index, const CPos& pos);
	int GetLargeStepEnemyIndex();
	CBaseEnemy* GetLargeStepEnemy(int index, const CPos& pos);
};

