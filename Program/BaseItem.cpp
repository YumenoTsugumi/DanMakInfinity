#include "DxLib.h"
#include "DXFunc.h"
#include "BaseItem.h"
#include "BaseBullet.h"
#include "BaseBullet.h"
#include "Game.h"
#include <float.h>
#include "BattleScene.h"
CBulletImage* CBaseItem::m_shineImage = nullptr;

// プレイヤーがアイテム回収範囲に入った
const int noTakeCount = 45; // フレーム
const int fastGetItemCount = 0; // アイテムが取れるようになって0.5秒以内にアイテム取得範囲に入った
const int nomalGetItemCount = 60; // アイテムが取れるようになって0.5+1秒以内にアイテム取得範囲に入った

//---------------------------------------------------------------------------------
//	CItemManager
//---------------------------------------------------------------------------------
CItemManager::CItemManager(int m_num/* = 512*/){
	m_bulletTotalNum = m_num;

	m_bullet = new CBaseBullet*[m_bulletTotalNum];
	for(int i=0;i<m_bulletTotalNum;i++){
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}

CItemManager::~CItemManager()
{
	// 親のデストラクタで開放される
}

void CItemManager::Draw() {
	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] == nullptr) {
			continue;
		}
		m_bullet[i]->Draw();
	}
}

//---------------------------------------------------------------------------------
//	CBaseItem
//---------------------------------------------------------------------------------

CBaseItem::CBaseItem(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
						CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, image),
	m_gravity(0.0)
{
	if (m_shineImage == nullptr) {
		m_shineImage = (CBulletImage*)CGame::GetResource(20721);
	}
	Set();
	SetImage(image);
}
//CBaseItem::CBaseItem(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName) :
//						CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, ImageName),
//	m_gravity(0.0)
//{
//	Set();
//	SetImage(ImageName);
//}

CBaseItem::~CBaseItem(){

}
void CBaseItem::Set(){
	funcP = nullptr;
	m_removeNow = false;

	m_waitCount = 0;
	m_waitTime = 0;
	m_survivalTime = 0;
	m_itemRank = 0;
	m_preRecoveryFlag = false;
}

void CBaseItem::Action(){
	if (m_waitTime > 0) {
		if (m_waitCount <= m_waitTime) {
			m_waitCount++;
			return;
		}
	}

	if (m_plusSize > 0 && m_size < m_maxSize) {
		m_size += m_plusSize;
		if (m_size > m_maxSize) {
			m_size = m_maxSize;
		}
	}
	else if (m_plusSize < 0 && m_size > m_maxSize) {
		m_size += m_plusSize;
		if (m_size < m_maxSize) {
			m_size = m_maxSize;
		}
	}

	if (m_recoveryFlag) {
		m_speed = 12.0;
		m_angle = (CFunc::GetTwoPointAngle(m_pos, m_target) + (180.0 / CFunc::RAD));
		//速度
		m_vel.x = cos(m_angle) * m_speed;
		m_vel.y = sin(m_angle) * m_speed;
		m_pos += m_vel;
	} else {

		//移動
		Move();

		m_pos.y += m_gravity;
		m_gravity += 0.1;
	}

	if(funcP != nullptr){
		funcP(this);
	}

	//範囲外判定
	RectOut();

	m_count++;
}


void CBaseItem::Draw(){
	if (m_waitTime > 0) {
		if (m_waitCount <= m_waitTime) {
			m_waitCount++;
			return;
		}
	}

	//回転フラグがONなら
	if(m_imageInfo.m_rotationFlg == true){
		m_imageInfo.m_rotationAngle += m_imageInfo.m_rotationSpeed;
	}

	//描画
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CPos subPos = CBattleScene::GetBattleScene()->GetBackGroundscrollSmall();
	int itemRank = GetSurvivalTimeRank();
	if (m_recoveryFlag) {
		itemRank = m_itemRank; // 自動取得モードになってるなら、その時時点の値にする
	}
	int shineImage = m_shineImage->m_images[0];
	if (itemRank == 3 || itemRank == 2) {
		double blend = 255.0 - 255.0 * ((double)(m_count) / (noTakeCount + fastGetItemCount + nomalGetItemCount)); // アイテムが取得できる時間 / アイテムが輝いている時間
		
		SetDrawBlendMode(DX_BLENDMODE_ADD, blend);
		CDxFunc::MyDrawRotaGraph(m_pos+ subPos, m_size * 2.0, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, shineImage);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		CDxFunc::MyDrawRotaGraph(m_pos + subPos, m_size, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, m_image[m_imageInfo.m_animePos]);
	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		CDxFunc::MyDrawRotaGraph(m_pos + subPos, m_size, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, m_image[m_imageInfo.m_animePos]);
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 ) ;
}

void CBaseItem::RectOut(){
	//領域外に出たら削除
	if (//m_pos.x <	CGame::GetGameRect().leftUp.x - m_imageInfo.m_sizeX / 2 ||
		//m_pos.x >	CGame::GetGameRect().rightDown.x + m_imageInfo.m_sizeX / 2 ||
		//m_pos.y <	CGame::GetGameRect().leftUp.y - m_imageInfo.m_sizeY / 2 ||
		m_pos.y >	CGame::GetGameRect().rightDown.y + m_imageInfo.m_sizeY / 2) {
		m_removeFlg = true;
	}
}

void CBaseItem::Remove(){
	//何もしない
}

void CBaseItem::SetFuncP(void (*FuncP)(CBaseItem* eff)){
	funcP = FuncP;
}

// 待つ時間
void CBaseItem::SetWaitTime(int waitTime)
{
	m_waitTime = waitTime;
}

void CBaseItem::SetSize(double size, double plusSize, double maxSize) {
	m_size = size;
	m_plusSize = plusSize;
	m_maxSize = maxSize;
}


int CBaseItem::GetSurvivalTimeRank() {
	if (m_count <= noTakeCount + fastGetItemCount) {
		return 3; // 早く取った
	}
	else if (m_count <= noTakeCount + fastGetItemCount + nomalGetItemCount) {
		return 2; // 普通の速度で取った
	}
	else {
		return 1; // 取得遅すぎ
	}
}

void CBaseItem::SetRecovery() {
	m_recoveryFlag = true;
	m_itemRank = GetSurvivalTimeRank();
}
void CBaseItem::SetPreRecovery()
{
	m_preRecoveryFlag = true;
	m_itemRank = GetSurvivalTimeRank();
}

// アイテムによるスコア加算
void CBaseItem::GetItemAddScore() {
	CBattleScene::AddItem(m_itemRank);
}

void CBaseItem::SetRemove()
{
	m_removeFlg = true;
}

// 出てからアイテムを一定時間は取れないようにするための判定
bool CBaseItem::IsTakeTime()
{
	if (m_count < noTakeCount) {
		return false;
	}
	if (m_preRecoveryFlag) {
		m_recoveryFlag = true;
	}
	return true;
}

// どっちかのフラグがたっていたら、自動回収範囲の判定を行わない
bool CBaseItem::GetRecoveryFlag() {
	return m_recoveryFlag || m_preRecoveryFlag;
}
