
#include "Func.h"
#include "BattleScene.h"


void CBattleScene::Collision_Enemy_BulyerBullet() 
{

	for (int jj = 0; jj < m_enemyManager.m_enemyTotalNum; jj++) {
		if (m_enemyManager.m_enemy[jj] == nullptr) {
			continue;
		}
		CBaseEnemy* enemy = m_enemyManager.m_enemy[jj];

		for (int ii = 0; ii < m_playerBullet.m_bulletTotalNum; ii++) {
			if (m_playerBullet.m_bullet[ii] == nullptr) {
				continue;
			}
			CPlayerBullet* playerBullet = (CPlayerBullet*)m_playerBullet.m_bullet[ii];


			if (CFunc::CollisionCircleToCircle(playerBullet->m_pos, playerBullet->m_hitSize, enemy->m_pos, enemy->m_hitSize)) {
				enemy->Damaged(playerBullet->m_damage); // 爆発エフェクトはここ
				playerBullet->Hit(); // ヒットエフェクトはココ
				continue;
			}

		}
	}

}