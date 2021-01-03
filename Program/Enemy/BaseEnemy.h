#pragma once

#include "BaseLauncher.h"
#include "MoveComponent.h"
#include "BehaviorComponent.h"

#include <vector>
#include "Game.h"

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
	virtual void Damaged(); // ダメージを受けたとき
	virtual void Die(); // 死んだ時
	virtual void DebugPrint();

	void SetBehaviorComponent(CBehaviorComponent* component, int waitTime = 0);
	void AddLauncher(const CPos& pos, CBaseLauncher* launcher);
protected:
	std::vector<Launcher> m_launchers; // 発射口
	CBehaviorComponent* m_behaviorComponent; // 移動コンポーネント
	int m_life; // ライフ

	EnemyType m_type; // 地上～空　小型～大型
	CPos m_pos; // 座標
	bool m_shotTiming; // CBehaviorComponent以外で撃つタイミングを制御したい場合(trueならうつ)
	int m_count;
};

/*
Enemyメモ
　赤の敵　時期狙いが多い
　青の敵　時期に依存しないバラマキ
　緑の敵　どうしよう？
*/