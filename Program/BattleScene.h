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

	//std::vector<CBaseEnemy*> m_enemys; // 敵
	CEnemyManager m_enemyManager; // 敵
	CBulletManager m_bulletManeger; // 敵の弾
	CBeamManager m_beamManeger; // 敵のビーム

	CBackGroundPatternA m_bg; // 背景

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

};