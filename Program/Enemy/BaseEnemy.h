#pragma once

#include "BaseLauncher.h"
#include "MoveComponent.h"
#include <vector>

enum EnemyType {
	GroundS = 0, // 地上小型機
	GroundM, // 地上中型機
	GroundL, // 地上大型機
	SkyS, // 空中小型機
	SkyM, // 空中中型機
	SkyL, // 空中大型機
	Boss
};

enum MoveStatus {
	Admitting = 0, // 入場中
	Admitted, // 入場済み
	leaving, // 退場（不要かも）
};

struct Launcher {
	Launcher(const CPos& pos, BaseLauncher* launcher) {
		m_relativePos = pos;
		m_launcher = launcher;
	}
	BaseLauncher* m_launcher; // 発射口の実態
	CPos m_relativePos; // 座標(敵本体の座標からの相対座標)
};

class BaseEnemy {
public:
	BaseEnemy(const CPos& pos);
	virtual ~BaseEnemy();

	virtual void Action();
	virtual void Draw();
	virtual void Damaged(); // ダメージを受けたとき
	virtual void Die(); // 死んだ時

	void SetMoveComponent(MoveComponent* component);
	void AddLauncher(const CPos& pos, BaseLauncher* launcher);
protected:
	std::vector<Launcher> m_launchers; // 発射口
	MoveComponent* m_moveComponent; // 移動コンポーネント
	int m_life; // ライフ
	MoveStatus m_moveStatus; // 入退場ステータス
	EnemyType m_type; // 地上～空　小型～大型
	CPos m_pos; // 座標
	int m_count;
};

