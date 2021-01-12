#pragma once

#include "BaseLauncher.h"
#include "MoveComponent.h"
#include "BehaviorComponent.h"

#include <vector>
#include "Game.h"

class CBaseLauncher;

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


enum EnemySize {
	Small = 0, // 地上小型機
	Medium = 1, // 地上中型機
	Large = 2, // 地上大型機
	Boss = 3
};


struct Launcher {
	Launcher(const CPos& pos, CBaseLauncher* launcher) {
		m_relativePos = pos;
		m_launcher = launcher;
	}
	CBaseLauncher* m_launcher; // 発射口の実態
	CPos m_relativePos; // 座標(敵本体の座標からの相対座標)
};

struct Collision {
	Collision(const CPos& pos, double r) {
		m_relationPos = pos;
		m_rad = r;
	}
	CPos m_relationPos; // 当たり判定
	double m_rad; // 当たり半径
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

	void Init(int life, EnemySize size, const std::vector<Collision>& collisions);

	// 当たり判定
	std::vector<Collision> m_collisions;
	// 角度と大きさを考慮した、相対座標と角度を返してくれる
	void GetCollisionData(const Collision& co, CPos& p, double& size);
	CPos GetCollisionData(const CPos& launcherPos);

	std::vector<Launcher> m_launchers; // 発射口
	CBehaviorComponent* m_behaviorComponent; // 移動コンポーネント
	int m_life; // ライフ
	bool m_removeFlg; // trueになると勝手に削除しｔくれる

	EnemySize m_size; // 大中小
	
	bool m_shotTiming; // CBehaviorComponent以外で撃つタイミングを制御したい場合(trueならうつ)
	int m_count;

	CImage* m_image;

	// 表示サイズの割合手
	void SetDrawSize(double size);
	double m_drawSizeRatio;

	double m_hitSize; // 当たり判定の大きさ（半径）継承したクラスで設定する

	CPos m_pos; // 座標
	CPos GetPos() { return m_pos; }

	// 敵が向いている方向
	double GetDirectionDeg();
	double GetDirectionRad();

	// 時機の位置の設定
	static CPos m_target;
	static void SetTarget(CPos target);

	// 当たり判定を可視化
	void DebugCollisionDraw();
};

/*
Enemyメモ
　赤の敵　時期狙いが多い
　青の敵　時期に依存しないバラマキ
　緑の敵　どうしよう？
*/