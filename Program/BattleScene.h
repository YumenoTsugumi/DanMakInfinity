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
#include "StageManager.h"

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
	CBulletManager m_playerBullet; // プレイヤーの弾

	static CEnemyManager m_enemyManager; // 敵
	static CBulletManager m_bulletManager; // 敵の弾
	static CBeamManager m_beamManager; // 敵のビーム

	StageManager m_stageManager; // 敵のスポナー

	CBackGroundPatternA m_bg; // 背景

	CBattleSceneUI m_ui;

	
	// 取得アイテム合計数
	static int m_takeItemRankCount[3]; // それぞれのランク毎にとった個数
	static void AddItem(int itemRank);
	static void GetItemCount(int& total, int& rank3Item, int& rank2Item, int& rank1Item) {
		rank3Item = m_takeItemRankCount[2];
		rank2Item = m_takeItemRankCount[1];
		rank1Item = m_takeItemRankCount[0];
		total = rank3Item * 3 + rank2Item * 2 + rank1Item;
	}

	// スコア
	static long long m_hiScore;
	static long long m_score;
	static int m_rank;
	static long long GetHiScore() {	return m_hiScore;	}
	static long long GetScore() { return m_score; }
	static void AddScore(int addScore);
	static int GetRank() { return m_rank; }
	static void AddRank(int delta);
	
	static CPos m_playerPos;
	static CPos GetPlayerPos() {return m_playerPos;	}
	static void SetPlayerPos(const CPos pos) {m_playerPos = pos;}

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
	static void SetBulletRemoveTime(BulletRemoveType type, int time);
	static BulletRemoveType m_bulletRemoveType;
	static int m_bulletRemoveTime;
	static int m_bulletRemoveCount;

	// BattleSceneCollision.cppにて実装
	void Collision_Enemy_PulyerBullet(); // 敵　時機の弾
	void Collision_Item_Player(); // アイテム　時機


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