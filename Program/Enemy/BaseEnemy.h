#pragma once

#include "BaseLauncher.h"
#include "MoveComponent.h"
#include "BehaviorComponent.h"

#include <vector>
#include "Game.h"

class CBaseEnemy;
//敵を管理するクラス
class CEnemyManager {
public:

	//コンストラクタ
	//敵の最大数を設定する
	//デフォルト1024
	CEnemyManager(int m_num = 64);
	virtual ~CEnemyManager();

	//最大数	64ぐらいあれば弾幕できるんじゃないか
	//コンストラクタで設定後は変更不可
	int m_enemyTotalNum;

	//弾の挿入位置	徐々にずらし弾が重ならないようにするため
	int m_order;

	//弾データ	弾クラスを代入する
	//ポインタを入れる箱
	CBaseEnemy** m_enemy;

	virtual void Action();
	virtual void Draw();

	int Add(CBaseEnemy* enemy);
};


enum EnemyType {
	GroundS = 0, // 地上小型機
	GroundM, // 地上中型機
	GroundL, // 地上大型機
	SkyS, // 空中小型機
	SkyM, // 空中中型機
	SkyL, // 空中大型機
	Boss
};


struct Launcher {
	Launcher(const CPos& pos, CBaseLauncher* launcher) {
		m_relativePos = pos;
		m_launcher = launcher;
	}
	CBaseLauncher* m_launcher; // 発射口の実態
	CPos m_relativePos; // 座標(敵本体の座標からの相対座標)
};

class CBaseEnemy {
public:
	CBaseEnemy(const CPos& pos);
	virtual ~CBaseEnemy();

	virtual void Action();
	virtual void Draw();
	virtual void Shot();
	virtual void Damaged(int damage); // ダメージを受けたとき
	virtual void Die(); // 死んだ時
	virtual void DebugPrint();

	void SetBehaviorComponent(CBehaviorComponent* component, int waitTime = 0);
	void AddLauncher(const CPos& pos, CBaseLauncher* launcher);


	std::vector<Launcher> m_launchers; // 発射口
	CBehaviorComponent* m_behaviorComponent; // 移動コンポーネント
	int m_life; // ライフ
	bool m_removeFlg; // trueになると勝手に削除しｔくれる

	EnemyType m_type; // 地上～空　小型～大型
	CPos m_pos; // 座標
	bool m_shotTiming; // CBehaviorComponent以外で撃つタイミングを制御したい場合(trueならうつ)
	int m_count;

	double m_hitSize; // 当たり判定の大きさ（半径）継承したクラスで設定する
};

/*
Enemyメモ
　赤の敵　時期狙いが多い
　青の敵　時期に依存しないバラマキ
　緑の敵　どうしよう？
*/