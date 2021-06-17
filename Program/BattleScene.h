#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"
#include "BaseBeam.h"

#include "Launcher.h"
#include "BaseEnemy.h"
#include "BaseItem.h"
#include "BackGround.h"
#include "GameDefine.h"
#include "BattleSceneUI.h"
#include "BattleResultUI.h"
#include "StageManager.h"

constexpr int RankBasedDigit = 10000;

class CBattleScene : public CScene{
public:
	//------------------
	//	おまじない
	//------------------
	CBattleScene(int InTime);
	virtual ~CBattleScene();

	//行動	引数isTopSceneは一番上の描画かどうかである。上から渡されるので気にしなくて良い
	virtual void Main(CInputAllStatus *input);

	//------------------
	//	ここから下は自由に
	//------------------
	void Init(CGame* gameP);

	CGame *m_game; // 上位のゲームクラス
	CPlayer m_player; // プレイヤー自身
	CPos m_initPlayerPos; // プレイヤーの初期位置
	CPos GetBackGroundscrollSmall() {
		CPos subPos; // 背景とプレイヤーの位置の位相
		subPos.x = (m_initPlayerPos.x - m_player.m_pos.x) / 5.0;
		subPos.y = 0;
		return subPos;
	}
	CPos GetBackGroundscrollLarge() {
		CPos subPos; // 背景とプレイヤーの位置の位相
		subPos.x = (m_initPlayerPos.x - m_player.m_pos.x) / 10.0;
		subPos.y = 0;
		return subPos;
	}
	CPos GetPlayerPosByScroll() {
		return m_player.m_pos - GetBackGroundscrollSmall();
	}

	static CBattleScene* m_scene;
	static void SetBattleScene(CBattleScene* scene) {
		m_scene = scene;
	}
	static CBattleScene* GetBattleScene() {
		return m_scene;
	}

	CBulletManager m_playerBullet; // プレイヤーの弾

	static CEnemyManager m_enemyManager; // 敵
	static CBulletManager m_bulletManager; // 敵の弾
	static CBeamManager m_beamManager; // 敵のビーム

	StageManager m_stageManager; // 敵のスポナー
	bool m_battleResultUIReset;
	CBattleResultUI m_battleResultUI; // 勝利画面

	int m_feedinCount;
	int m_feedoutCount;
	CBackGroundPatternA* m_activeBg;
	CBackGroundPatternA* m_nonActiveBg;
	CBackGroundPatternA m_bgA; // 背景1
	CBackGroundPatternA m_bgB; // 背景2

	CBattleSceneUI m_ui;

	//-------------------------
	// ステージ毎
	static int m_spawneSmallCount;
	static int m_spawneMediumCount;
	static int m_spawneLargeCount;
	static int m_spawneDropItemCount;
	static void AddSpawneSmallCount() { m_spawneSmallCount++; }
	static void AddSpawneMediumCount() { m_spawneMediumCount++; }
	static void AddSpawneLargeCount() { m_spawneLargeCount++; }
	static void AddDropItemCount() { m_spawneDropItemCount++; }

	static int m_destroySpawneSmallCount;
	static int m_destroySpawneMediumCount;
	static int m_destroySpawneLargeCount;
	static int m_getSpawneDropItemCount;
	static void AddDestorySpawneSmallCount() { m_destroySpawneSmallCount++; }
	static void AddDestorySpawneMediumCount() { m_destroySpawneMediumCount++; }
	static void AddDestorySpawneLargeCount() { m_destroySpawneLargeCount++; }
	static void AddGetDropItemCount() { m_getSpawneDropItemCount++; }

	static int m_usedBomb;
	static int m_missCount;
	static void AddUsedBomb() { m_usedBomb++; }
	static void AddMissCount() { m_missCount++; }

	static void StageCountReset();

	enum ClearRank {
		Rank_S = 0, Rank_A = 1, Rank_B = 2, Rank_C = 3, Rank_D = 4, Rank_E = 5,
	};
	void StageClearResult();
	std::tuple<ClearRank, int ,int> CalcClearBounus(double destoryLargeEnemyRatio,double destoryMediumEnemyRatio,double destorySmallEnemyRatio,
		double usedBomb,double missCount,double getItemRatio);

	// 取得アイテム合計数
	static int m_takeItemRankCount[3]; // それぞれのランク毎にとった個数
	static void AddItem(int itemRank);
	static void GetItemCount(int& total, int& rank3Item, int& rank2Item, int& rank1Item) {
		rank3Item = m_takeItemRankCount[2];
		rank2Item = m_takeItemRankCount[1];
		rank1Item = m_takeItemRankCount[0];
		total = rank3Item * 3 + rank2Item * 2 + rank1Item;
	}
	// ランク

	// プレイヤーの情報
	static int m_haveBomb;
	static int m_haveLife;
	static int GetHaveBomb() { return m_haveBomb; }
	static int GetHaveLife() { return m_haveLife; }
	static void BombDecrement() { m_haveBomb--; }
	static void LifeDecrement() { m_haveLife--; }

	// スコア
	static long long m_hiScore;
	static long long m_score;
	static int m_rank; // rankは10000で1を管理する
	static double m_rankRatio; // 1.0 0.5～10.0
	static long long GetHiScore() {	return m_hiScore;	}
	static long long GetScore() { return m_score; }
	static void AddScore(int addScore);
	static int GetRank() { return m_rank / RankBasedDigit; }
	static void AddRank(int delta);
	static double GetRankRatio() { return m_rankRatio; }
	static void SetRankRatio(double ratio);
	static void AddRankRatio(double delta);
	static void AddRankRatioByStageSpan(); // ステージ中のspan
	static void AddRankRatioByStageClear(int resultrank); // S0 E5
	
	static CPos m_playerPos;
	static CPos GetPlayerPos() {return m_playerPos;	}
	static void SetPlayerPos(const CPos pos) {m_playerPos = pos;}

	// 現在ステージ
	static int m_nowStage;
	static int GetNowStage() { return m_nowStage; }
	static void AddNowStage() { m_nowStage++; };

	// その他
	//CPos testLauncherPos; // テスト砲台位置
	//CLauncher999* testLauncher; // テスト砲台

	// 参照する範囲が広いのでグローバルにした
	static CEffectManager m_effectManager; // エフェクト
	static CItemManager m_itemManager; // アイテムマネージャー

	// 弾消し
	enum BulletRemoveType {
		Nothing = 0,
		Item = 1
	};
	void RemoveBullet(); // 弾消し処理
	static void RemoveBulletByMidiumEnemy(int id); // 弾消し処理
	static void SetBulletRemoveTime(BulletRemoveType type, int time); // 弾消し
	static BulletRemoveType m_bulletRemoveType;
	static int m_bulletRemoveTime;
	static int m_bulletRemoveCount;

	void DestoryAllEnemyNothingItemDrop(); // 全敵破壊
	void DamageAllEnemy(int damage);

	// BattleSceneCollision.cppにて実装
	void Collision_Enemy_PulyerBullet(); // 敵　時機の弾
	void Collision_Item_Player(); // アイテム　時機
	void Collision_EnemyBullet_Pulyer(); // 敵の弾　時機

	// 敵出現ルーチン
	void DebugAllEnemyDirection();

	// デバッグコマンド
	void DebugCommand();

	static bool m_enemyHitSizeDraw; // 敵の当たり判定を表示する
	static bool GetEnemyHitSizeDraw() {
		return m_enemyHitSizeDraw;
	}
	static bool m_enemyLauncherDraw; // 敵の砲台の位置を表示する
	static bool GetEnemyLauncherDraw() {
		return m_enemyLauncherDraw;
	}
	static bool m_enableDebugCommand; // デバッグコマンドを有効にする
};