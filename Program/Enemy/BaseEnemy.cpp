﻿#include "BaseEnemy.h"
#include "BattleScene.h"

CPos CBaseEnemy::m_target;
int CBaseEnemy::m_globalEnemyId = 0; // 発射ID（中型機の弾消し用のID）
CBattleScene* CBaseEnemy::m_scene;
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
void CEnemyManager::AllRemove(){	
	for (int i = 0; i < m_enemyTotalNum; i++) {
		delete m_enemy[i];
		m_enemy[i] = nullptr;
	}
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
	if (Bullet->m_size == EnemySize::Small) {
		CBattleScene::AddSpawneSmallCount();
	}
	else if (Bullet->m_size == EnemySize::Medium) {
		CBattleScene::AddSpawneMediumCount();
	}
	else if (Bullet->m_size == EnemySize::Large) {
		CBattleScene::AddSpawneLargeCount();
	}

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

void CEnemyManager::DestoryAllEnemyNothingItemDrop()
{
	for (int i = 0; i < m_enemyTotalNum; i++) {
		if (m_enemy[i] == nullptr) {
			continue;
		}
		m_enemy[i]->Die(false); // アイテムを落とさない
	}
}
void CEnemyManager::DamageAllEnemy(int damage)
{
	for (int i = 0; i < m_enemyTotalNum; i++) {
		if (m_enemy[i] == nullptr) {
			continue;
		}
		m_enemy[i]->Damaged(damage);
	}
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
	m_rank = CBattleScene::GetRank();
	m_enemyId = m_globalEnemyId++;
	m_waitShotTime = -CFunc::RandI(0, 60);
	m_SpawneWaitCount = 60*1; // 2秒間スポーン待つ
}

CBaseEnemy::~CBaseEnemy() {

}

void CBaseEnemy::Action() {
	if (m_SpawneWaitCount-- > 0) {
		return;
	}

	m_waitShotTime++;
	m_behaviorComponent->Action(m_pos);

	BehaviorStatus behaviorStatus = m_behaviorComponent->GetBehaviorStatus();
	if (BehaviorStatus::Shot == behaviorStatus || m_shotTiming) {
		Shot();
	}
	if (BehaviorStatus::Finish == behaviorStatus) {
		m_removeFlg = true;
	}
}
void CBaseEnemy::Draw() {

}

void CBaseEnemy::BaseDraw(CPos p, double ExtRate, double Angle, int GrHandle, int TransFlag, int TurnFlag)
{
	CPos subPos = CBattleScene::GetBattleScene()->GetBackGroundscrollSmall();
	CDxFunc::MyDrawRotaGraph(p + subPos, ExtRate, Angle, GrHandle, TransFlag, TurnFlag);
}

void CBaseEnemy::Shot()
{
	for (CBaseLauncher* l : m_launchers) {
		CPos nowRelativePos = GetCollisionData(l->m_relativePos);
		l->Action(m_pos, nowRelativePos);
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
void CBaseEnemy::Die(bool dropItem) {
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
	if (m_size == EnemySize::Large) { // 大型
		int loopCp = 12;
		for (int ii = 0; ii < loopCp; ii++) {
			CPos pp(CFunc::RandI(0, 30), CFunc::RandI(0, 30));
			double ang = (double)ii * 120 + CFunc::RandF(0, 90);
			double speed = 1.0 + CFunc::RandF(200, 400) / 100.0;
			CBaseEffect* eff = new CBaseEffect(0, EDirType::Abs, m_pos + pp, speed, ang, 0, -0.2, 0.5, 0, image4[ii % 4]);
			double size = 1.8 - (double)ii * 0.1;
			eff->SetSize(size, +0.0);
			eff->SetWaitTime(ii * 3);
			eff->SetBlend(255, +0.0);
			eff->SetBlendType(DX_BLENDMODE_NOBLEND);
			eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
			eff->SetRemoveCount(60);	//60frで削除

			CBattleScene::m_effectManager.Add(eff);
		}

	} else { // 小型中型
		int loopCp = 4;
		if (m_size == EnemySize::Medium)loopCp = 6;

		for (int ii = 0; ii < loopCp; ii++) {
			CPos pp(CFunc::RandI(0, 30), CFunc::RandI(0, 30));
			double ang = (double)ii * 120 + CFunc::RandF(0, 90);
			double speed = CFunc::RandF(200, 400) / 100.0;
			CBaseEffect* eff = new CBaseEffect(0, EDirType::Abs, m_pos + pp, speed, ang, 0, -0.2, 0.5, 0, image4[ii % 4]);
			double size = 0.8 - (double)ii * 0.1;
			eff->SetSize(size, +0.0);
			eff->SetWaitTime(ii * 4);
			eff->SetBlend(255, +0.0);
			eff->SetBlendType(DX_BLENDMODE_NOBLEND);
			eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
			eff->SetRemoveCount(60);	//60frで削除

			CBattleScene::m_effectManager.Add(eff);
		}
	}

	if (dropItem) {
		// dropItemする＝倒したということなのでカウントする
		if (m_size == EnemySize::Small) {
			CBattleScene::AddDestorySpawneSmallCount();
		}
		else if (m_size == EnemySize::Medium) {
			CBattleScene::AddDestorySpawneMediumCount();
		}
		else if (m_size == EnemySize::Large) {
			CBattleScene::AddDestorySpawneLargeCount();
		}


		// アイテム
		int itemCount = (m_size + 1) * 10; // 大30　中20　小10
		for (int ii = 0; ii < itemCount; ii++) {
			double ang = CFunc::RandI(180 + 60, 360 - 60);
			double speed = 1.0 + CFunc::RandF(100, 300) / 100.0;
			CPos addPos = CPos(CFunc::RandI(-50, 50), CFunc::RandI(-50, 50));
			CBaseItem* eff = new CBaseItem(EDirType::Abs, m_pos + addPos, speed, ang, 0, 0, -0.1, 0, 20720);
			eff->SetSize(1.0, 0, 1.0);
			CBattleScene::m_itemManager.Add(eff);
			CBattleScene::AddDropItemCount();
		}
		

		// 大型機の場合、全部弾消し
		if (m_size == EnemySize::Large) {
			CBattleScene::SetBulletRemoveTime(CBattleScene::BulletRemoveType::Item, 120);
		}
		else if (m_size == EnemySize::Medium) { // 中型機の場合、その機体が出した弾をすべて消す
			CBattleScene::RemoveBulletByMidiumEnemy(GetEnemyId());
		}
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
void CBaseEnemy::AddLauncher(CBaseLauncher* launcher)
{
	launcher->SetParent(this);
	m_launchers.push_back(launcher);
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
// 敵の最終的な向き（これにより発射口や当たり判定の位置が決まる）
double CBaseEnemy::GetFinalDirectionRad()
{
	return m_behaviorComponent->GetDirection();
}


// 角度と大きさを考慮した、相対座標と角度を返してくれる
void CBaseEnemy::GetCollisionData(const Collision& co, CPos& p, double& size)
{
	CPos p2 = co.m_relationPos * m_drawSizeRatio; // 回転中心からの相対位置
	CPos p3 = CPos(0, 0); // 回転中心
	CFunc::RotatingMatrix(&p, p2, p3, -GetFinalDirectionRad() + CFunc::ToRad(90.0));

	size = co.m_rad * m_drawSizeRatio;
	return;
}
CPos CBaseEnemy::GetCollisionData(const CPos& launcherPos)
{
	CPos returnPos;
	CPos p2 = launcherPos * m_drawSizeRatio; // 回転中心からの相対位置
	CPos p3 = CPos(0, 0); // 回転中心
	CFunc::RotatingMatrix(&returnPos, p2, p3, -GetFinalDirectionRad() + CFunc::ToRad(90.0));
	return returnPos;
}



// 当たり判定の表示
void CBaseEnemy::DebugCollisionDraw()
{
	if(!CBattleScene::GetEnemyHitSizeDraw())return;
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
void CBaseEnemy::DebugLauncherDraw()
{
	if (!CBattleScene::GetEnemyLauncherDraw())return;

	for (CBaseLauncher* laun : m_launchers) {
		CPos pos = GetCollisionData(laun->m_relativePos);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
		CDxFunc::DrawCircle(m_pos + pos, 8, 32, GetColor(255, 0, 255), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		CDxFunc::DrawCircle(m_pos + pos, 8, 32, GetColor(255, 0, 255), FALSE);
	}
}

// 表示サイズの割合手
void CBaseEnemy::SetDrawSize(double size)
{
	m_drawSizeRatio = size;
}


double CBaseEnemy::GetToPlayerAngle()
{
	return CFunc::GetTwoPointAngle(m_pos, m_target) * CFunc::RAD + 180.0;
}