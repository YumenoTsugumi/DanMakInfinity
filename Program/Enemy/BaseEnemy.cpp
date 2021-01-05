#include "BaseEnemy.h"
#include "BattleScene.h"


//---------------------------------------------------------------------------------
//	CEnemyManager
//---------------------------------------------------------------------------------
CEnemyManager::CEnemyManager(int num/* = 64*/) {
	m_enemyTotalNum = num;

	m_enemy = new CBaseEnemy * [m_enemyTotalNum];
	for (int i = 0; i < m_enemyTotalNum; i++) {
		m_enemy[i] = nullptr;
	}

	m_order = 0;
}

CEnemyManager::~CEnemyManager() {

	delete[] m_enemy;
}

void CEnemyManager::Action() {
	for (int i = 0; i < m_enemyTotalNum; i++) {
		if (m_enemy[i] == nullptr) {
			continue;
		}
		m_enemy[i]->Action();

		if (m_enemy[i]->m_removeFlg == true) {
			delete m_enemy[i];
			m_enemy[i] = nullptr;
		}
	}
}

void CEnemyManager::Draw() {
	for (int i = 0; i < m_enemyTotalNum; i++) {
		if (m_enemy[i] == nullptr) {
			continue;
		}
		m_enemy[i]->Draw();
		m_enemy[i]->DebugPrint();
	}
}

//配列の空きを探し代入
int CEnemyManager::Add(CBaseEnemy* Bullet) {
	for (int i = m_order; i < m_enemyTotalNum; i++) {
		//挿入位置が最大値なら挿入位置を0に戻す
		if (m_enemy[i] == nullptr) {
			//挿入位置をインクリメントし
			m_order++;
			if (m_order >= m_enemyTotalNum) {
				m_order = 0;
			}

			m_enemy[i] = Bullet;
			return i;
		}
		if (m_order >= m_enemyTotalNum - 1) {
			m_order = 0;
		}
	}

	//見つからない場合最初から再びチャレンジ
	for (int i = m_order; i < m_enemyTotalNum; i++) {
		//挿入位置が最大値なら挿入位置を0に戻す
		if (m_enemy[i] == nullptr) {
			//挿入位置をインクリメントし
			m_order++;
			if (m_order >= m_enemyTotalNum) {
				m_order = 0;
			}

			m_enemy[i] = Bullet;
			return i;
		}
		if (m_order >= m_enemyTotalNum - 1) {
			m_order = 0;
		}
	}

	m_order = 0;
	delete Bullet;
	return -1;
}


//------------------------------------------------------
// 本体
//------------------------------------------------------

CBaseEnemy::CBaseEnemy(const CPos& pos) :
	m_pos(pos),
	m_shotTiming(false),
	m_life(1),
	m_hitSize(0),
	m_removeFlg(false)
{

}
CBaseEnemy::~CBaseEnemy() {

}

void CBaseEnemy::Action() {
	m_behaviorComponent->Action(m_pos);

	BehaviorStatus behaviorStatus = m_behaviorComponent->GetBehaviorStatus();
	if (BehaviorStatus::Shot == behaviorStatus || m_shotTiming) {
		Shot();
	}
}
void CBaseEnemy::Draw() {

}

void CBaseEnemy::Shot()
{
	for (Launcher& l : m_launchers) {
		l.m_launcher->Action(m_pos);
	}
}

// ダメージを受けたとき
void CBaseEnemy::Damaged(int damage) {
	m_life -= damage;

	if (m_life <= 0) {
		Die();
	}
} 

// 死んだ時
void CBaseEnemy::Die() {
	m_removeFlg = true;

	// "FireballExplosion2", 20500
	// "FireballExplosion3", 20501
	// "FireballExplosion7", 20502
	// "FireballExplosion10", 20503

	int image4[4] = { 20503 ,20501 ,20501 ,20501 };
	for (int ii = 0; ii < 4; ii++) {
		CPos pp(CFunc::RandF(0, 800), CFunc::RandF(0, 400));
		double ang = (double)ii * 120 + CFunc::RandF(0, 90);
		double speed = CFunc::RandF(200, 400) / 100.0;
		CBaseEffect* eff = new CBaseEffect(0, EDirType::Abs, m_pos, speed, ang, 0, -0.2, 0.5, 0, image4[ii]);
		double size = 0.8 - (double)ii * 0.1;
		eff->SetSize(size, +0.0);
		eff->SetWaitTime(CFunc::RandI(1, 15));
		eff->SetBlend(255, +0.0);
		eff->SetBlendType(DX_BLENDMODE_NOBLEND);
		eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
		eff->SetRemoveCount(60);	//60frで削除

		CBattleScene::m_effectManager.Add(eff);
	}

	
}

void CBaseEnemy::DebugPrint()
{
	m_behaviorComponent->DebugPrint();
}


// 移動制御を設定
void CBaseEnemy::SetBehaviorComponent(CBehaviorComponent* component, int waitTime/*=0*/)
{
	m_behaviorComponent = component;
	if (waitTime > 0) {
		m_behaviorComponent->SetWaitTime(waitTime);
	}
}
// 砲台を設定
void CBaseEnemy::AddLauncher(const CPos& pos, CBaseLauncher* launcher)
{
	Launcher l(pos, launcher);
	m_launchers.push_back(l);
}
