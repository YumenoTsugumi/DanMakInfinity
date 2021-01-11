#include "BaseEnemy.h"
#include "BattleScene.h"

CPos CBaseEnemy::m_target;

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
	m_removeFlg(false),
	m_size(EnemySize::Small),
	m_drawSizeRatio(1.0)
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
	if (m_removeFlg == true) {
		return;
	}
	m_removeFlg = true;

	// "FireballExplosion2", 20500
	// "FireballExplosion3", 20501
	// "FireballExplosion7", 20502
	// "FireballExplosion10", 20503
	// 爆発エフェクト
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

	// アイテム
	int itemCount = (m_size + 1) * 10;
	int itemImage[6] = { 20700 ,20701 ,20702 ,20703 ,20704 ,20705 };
	for (int ii = 0; ii < itemCount; ii++) {
		double ang = CFunc::RandI(180+60, 360-60);
		double speed = 1.0 + CFunc::RandF(100, 300) / 100.0;
		CPos addPos = CPos(CFunc::RandI(-50, 50), CFunc::RandI(-50, 50));
		CBaseItem* eff = new CBaseItem(EDirType::Abs, m_pos + addPos, speed, ang, 0, 0, -0.1, 0, itemImage[CFunc::RandI(0, 5)]);
		eff->SetSize(0.0, +0.033, 0.125);
		CBattleScene::m_itemManager.Add(eff);
	}

	// 大型機の場合、全部弾消し
	if (m_size == EnemySize::Large) {
		CBattleScene::SetBulletRemoveTime(CBattleScene::BulletRemoveType::Item, 30);
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
	launcher->SetParent(this);
	Launcher l(pos, launcher);
	m_launchers.push_back(l);
}

// 時機の位置の設定
void CBaseEnemy::SetTarget(CPos target) {
	m_target = target;
}

// 共通の初期化
void CBaseEnemy::Init(int life, EnemySize size, const std::vector<Collision>& collisions)
{
	m_life = life;
	m_size = size;
	m_collisions = collisions;
}

// 敵が向いている方向
double CBaseEnemy::GetDirectionDeg()
{
	return CFunc::ToDeg(m_behaviorComponent->GetDirection());
}
double CBaseEnemy::GetDirectionRad()
{
	return m_behaviorComponent->GetDirection();
}

// 角度と大きさを考慮した、相対座標と角度を返してくれる
void CBaseEnemy::GetCollisionData(const Collision& co, CPos& p, double& size)
{
	CPos p2 = co.m_relationPos * m_drawSizeRatio; // 回転中心からの相対位置
	CPos p3 = CPos(0, 0); // 回転中心
	CFunc::RotatingMatrix(&p, p2, p3, -GetDirectionRad() + CFunc::ToRad(90.0));

	size = co.m_rad * m_drawSizeRatio;
	return;
}
CPos CBaseEnemy::GetCollisionData(const CPos& launcherPos)
{
	CPos returnPos;
	CPos p2 = launcherPos * m_drawSizeRatio; // 回転中心からの相対位置
	CPos p3 = CPos(0, 0); // 回転中心
	CFunc::RotatingMatrix(&returnPos, p2, p3, -GetDirectionRad() + CFunc::ToRad(90.0));
	return returnPos;
}



// 当たり判定の表示
void CBaseEnemy::DebugCollisionDraw()
{
	
	for (const Collision& co : m_collisions) {
		CPos pos; // 結果
		double size; // 大きさ
		GetCollisionData(co, pos, size);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		CDxFunc::DrawCircle(m_pos + pos, size, 32, GetColor(0, 0, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		CDxFunc::DrawCircle(m_pos + pos, size, 32, GetColor(0, 0, 255), FALSE);
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

// 表示サイズの割合手
void CBaseEnemy::SetDrawSize(double size)
{
	m_drawSizeRatio = size;
}
