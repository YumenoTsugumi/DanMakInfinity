
#include "Func.h"
#include "BattleScene.h"

// 敵　時機の弾
void CBattleScene::Collision_Enemy_PulyerBullet() 
{

	for (int jj = 0; jj < m_enemyManager.m_enemyTotalNum; jj++) {
		if (m_enemyManager.m_enemy[jj] == nullptr) {
			continue;
		}
		CBaseEnemy* enemy = m_enemyManager.m_enemy[jj];

		for (int kk = 0; kk < enemy->m_collisions.size(); kk++) {
			const Collision& co = enemy->m_collisions[kk];
			CPos pos;
			double size;
			enemy->GetCollisionData(co, pos, size);

			CPos ep = enemy->m_pos + pos;

			for (int ii = 0; ii < m_playerBullet.m_bulletTotalNum; ii++) {
				if (m_playerBullet.m_bullet[ii] == nullptr) {
					continue;
				}
				CPlayerBullet* playerBullet = (CPlayerBullet*)m_playerBullet.m_bullet[ii];

				if (CFunc::CollisionCircleToCircle(playerBullet->m_pos - GetBackGroundscrollSmall(), playerBullet->m_hitSize, ep, size)) {
					enemy->Damaged(playerBullet->m_damage); // 爆発エフェクトはここ
					playerBullet->Hit(); // ヒットエフェクトはココ
					continue;
				}

			}
		}
	}

}

// アイテム　プレイヤー
void CBattleScene::Collision_Item_Player()
{

	for (int jj = 0; jj < m_itemManager.m_bulletTotalNum; jj++) {
		if (m_itemManager.m_bullet[jj] == nullptr) {
			continue;
		}
		CBaseItem* item = (CBaseItem*)m_itemManager.m_bullet[jj];
	
		// アイテム取得範囲
		if (item->IsTakeTime() && CFunc::CollisionCircleToCircle(GetPlayerPosByScroll(), 32, item->m_pos, 0)) {
			item->GetItemAddScore();
			item->SetRemove();
			continue;
		}
		if (item->GetRecoveryFlag())continue; // アイテム自動回収フラグが立っていると、この後の処理が不要

		// アイテム自動回収範囲
		if (CFunc::CollisionCircleToCircle(GetPlayerPosByScroll(), 256+128, item->m_pos, 0)) {
			if (!item->IsTakeTime()) {
				item->SetPreRecovery();
				continue;
			}
			item->SetRecovery();
			continue;
		}

		
	}

}