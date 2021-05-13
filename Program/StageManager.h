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
	int m_maxCount;
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


	// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	// FulHDなら (480,20)(1440,1060)の範囲
	CPos ToGamePos(CPos ratioPos);
	CPos ToGamePos(double ratioPosX, double ratioPosY);
	// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	double ToGamePosX(double ratioPosX); 
	// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
	double ToGamePosY(double ratioPosY);

	// 0.0～1.0で画面サイズに比例する大きさが返ってくる
	double ToGameSizeX(double ratioPosX);
	// 0.0～1.0で画面サイズに比例する大きさが返ってくる
	double ToGameSizeY(double ratioPosY);

	int m_maxSpawneCount; // スポーン数　小12体　中4体　大2体
	void SetSpawner(EnemySize spawnerSize);
	EnemySize m_spawnerSize; // 出現する敵のサイズ
	CBaseEnemy* GetEnemy(const CPos& pos);

	void SetSpeedBySize();

	int m_index; // 出現する敵（子クラスで使用）
private:
	int GetSmallEnemyIndex();
	CBaseEnemy* GetSmallEnemy(int index, const CPos& pos);
	int GetMediumEnemyIndex();
	CBaseEnemy* GetMediumEnemy(int index, const CPos& pos);
	int GetLargeEnemyIndex();
	CBaseEnemy* GetLargeEnemy(int index, const CPos& pos);
};

