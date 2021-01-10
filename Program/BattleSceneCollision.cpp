
#include "Func.h"
#include "BattleScene.h"


void CBattleScene::Collision_Enemy_PulyerBullet() 
{

	for (int jj = 0; jj < m_enemyManager.m_enemyTotalNum; jj++) {
		if (m_enemyManager.m_enemy[jj] == nullptr) {
			continue;
		}
		CBaseEnemy* enemy = m_enemyManager.m_enemy[jj];

		for (int kk = 0; kk < enemy->m_collisions.size(); kk++) {
			Collision* co = &enemy->m_collisions[kk];
			CPos ep = enemy->m_pos + co->m_relationPos;

			for (int ii = 0; ii < m_playerBullet.m_bulletTotalNum; ii++) {
				if (m_playerBullet.m_bullet[ii] == nullptr) {
					continue;
				}
				CPlayerBullet* playerBullet = (CPlayerBullet*)m_playerBullet.m_bullet[ii];


				if (CFunc::CollisionCircleToCircle(playerBullet->m_pos, playerBullet->m_hitSize, ep, co->m_rad)) {
					enemy->Damaged(playerBullet->m_damage); // 爆発エフェクトはここ
					playerBullet->Hit(); // ヒットエフェクトはココ
					continue;
				}

			}
		}
	}

}


void CBattleScene::Collision_Item_Player()
{

	for (int jj = 0; jj < m_itemManager.m_bulletTotalNum; jj++) {
		if (m_itemManager.m_bullet[jj] == nullptr) {
			continue;
		}
		CBaseItem* item = (CBaseItem*)m_itemManager.m_bullet[jj];

		if (!item->IsTakeTime()) {
			continue;
		}
	
		// アイテム取得範囲
		if (CFunc::CollisionCircleToCircle(m_player.m_pos, 32, item->m_pos, 0)) {
			item->SetRemove();
			continue;
		}

		// アイテム自動回収範囲
		if (CFunc::CollisionCircleToCircle(m_player.m_pos, 256+128, item->m_pos, 0)) {
			item->SetRecovery();
			continue;
		}

		
	}

}