#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBullet.h"
#include "Resource.h"
#include "Game.h"

#include "BaseEffect.h"
#include "BattleScene.h"

CImage* m_imageRedAura = nullptr;
CImage* m_imageBlueAura = nullptr;

//---------------------------------------------------------------------------------
//	CBulletImageInfo
//---------------------------------------------------------------------------------
CBulletImageInfo::CBulletImageInfo(){
	Init();
}
void CBulletImageInfo::Init(){
	m_imageNo = 0;
	m_sizeX = 0;
	m_sizeY = 0;

	m_animeFlg = false;
	m_animeNum = 0;
	m_animePos = 0;
	m_animeSpeed = 0;
	m_rotationFlg = false;
	m_rotationSpeed = 0.0;
	m_rotationAngle = 0.0;
}
void CBulletImageInfo::Set(bool animeFlg,char animeNum,char animePos,char animeSpeed,
						   bool rotationFlg, double rotationSpeed,double rotationAngle){
	this->m_animeFlg = animeFlg;
	this->m_animeNum = animeNum;
	this->m_animePos = animePos;
	this->m_animeSpeed = animeSpeed;
	this->m_rotationFlg = rotationFlg;
	this->m_rotationSpeed = rotationSpeed;
	this->m_rotationAngle = rotationAngle;
}

//---------------------------------------------------------------------------------
//	CBulletManager
//---------------------------------------------------------------------------------
CBulletManager::CBulletManager(int num/* = 1024*/){
	m_bulletTotalNum = num;

	m_bullet = new CBaseBullet*[m_bulletTotalNum];
	for(int i=0;i<m_bulletTotalNum;i++){
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}

CBulletManager::~CBulletManager()
{
	delete [] m_bullet;
	m_bullet = nullptr;
}

void CBulletManager::Action(){
	for(int i=0;i<m_bulletTotalNum;i++){
		if (m_bullet[i] == nullptr) {
			continue;
		}
		m_bullet[i]->Action();

		if(m_bullet[i]->m_removeFlg == true){
			m_bullet[i]->Remove();
			delete m_bullet[i];
			m_bullet[i] = nullptr;
		}
	}
}

void CBulletManager::AllRemove()
{
	for (int i = 0; i < m_bulletTotalNum; i++) {
		delete m_bullet[i];
		m_bullet[i] = nullptr;
	}
}

void CBulletManager::Draw(){
	CPos subPos = CBattleScene::GetBattleScene()->GetBackGroundscrollSmall();

	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] == nullptr) {
			continue;
		}
		// オーラの描画
		CBaseBullet* bullet = m_bullet[i];
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		if (bullet->m_imageInfo.m_imageNo >= 0 && bullet->m_imageInfo.m_imageNo <= 100) {
			if (bullet->m_imageInfo.m_imageNo % 10 == 2) {
				CDxFunc::MyDrawRotaGraph(bullet->m_pos + subPos, 1.0f, bullet->m_angle + bullet->m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, m_imageBlueAura->m_iamge); // 青オーラ
			}
			else if (bullet->m_imageInfo.m_imageNo % 10 == 1) {
				CDxFunc::MyDrawRotaGraph(bullet->m_pos + subPos, 1.0f, bullet->m_angle + bullet->m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, m_imageRedAura->m_iamge); // 赤オーラ
			}
		}
	}

	for(int i=0;i<m_bulletTotalNum;i++){
		if (m_bullet[i] == nullptr) {
			continue;
		}
		m_bullet[i]->Draw();
	}

	if (m_imageRedAura == nullptr) {
		m_imageBlueAura = (CImage*)CGame::GetResource(500);
		m_imageRedAura = (CImage*)CGame::GetResource(501);
	}

}

//配列の空きを探し弾を代入
int CBulletManager::Add(CBaseBullet *Bullet){
	for(int i=m_order;i<m_bulletTotalNum;i++){
		//挿入位置が最大値なら挿入位置を0に戻す
		if(m_bullet[i] == nullptr){
			//挿入位置をインクリメントし
			m_order++;
			if(m_order >= m_bulletTotalNum){
				m_order = 0;
			}
			
			m_bullet[i] = Bullet;
			return i;
		}
		if(m_order >= m_bulletTotalNum-1){
			m_order = 0;
		}
	} 

	//見つからない場合最初から再びチャレンジ
	for(int i=m_order;i<m_bulletTotalNum;i++){
		//挿入位置が最大値なら挿入位置を0に戻す
		if(m_bullet[i] == nullptr){
			//挿入位置をインクリメントし
			m_order++;
			if(m_order >= m_bulletTotalNum){
				m_order = 0;
			}
			
			m_bullet[i] = Bullet;
			return i;
		}
		if(m_order >= m_bulletTotalNum-1){
			m_order = 0;
		}
	}

	m_order = 0;
	delete Bullet;
	return -1;
}



//---------------------------------------------------------------------------------
//	CBaseBullet
//---------------------------------------------------------------------------------
CPos CBaseBullet::m_target;
CRect CBaseBullet::m_rect;

CBaseBullet::CBaseBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) : 
	m_shotEnemyId(-1)
{
	Set(type, P, speed, angle, corner, acce, maxSpeed, nearAngle);
	SetImage(image);
}
//CBaseBullet::CBaseBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* imageName){
//	Set(type, P, speed, angle, corner, acce, maxSpeed, nearAngle);
//	SetImage(imageName);
//	if (m_imageRedAura == nullptr) {
//		m_imageRedAura = (CImage*)CGame::GetResource(600);
//	}
//}


CBaseBullet::~CBaseBullet(){

}

void CBaseBullet::Action(){
	//移動
	Move();
	//範囲外判定
	RectOut();

	m_count++;
}

void CBaseBullet::Draw(){
	SetDrawBlendMode( m_blendType , 255 ) ;

	//アニメフラグがONなら
	if(m_imageInfo.m_animeFlg == true){
		if(m_count % m_imageInfo.m_animeSpeed == 0){
			m_imageInfo.m_animePos++;
			if(m_imageInfo.m_animePos >= m_imageInfo.m_animeNum){
				m_imageInfo.m_animePos = 0;
			}
		}
	}
	//回転フラグがONなら
	if(m_imageInfo.m_rotationFlg == true){
		m_imageInfo.m_rotationAngle += m_imageInfo.m_rotationSpeed;
	}

	SetDrawBlendMode(m_blendType, 255);
	//描画
	CPos subPos = CBattleScene::GetBattleScene()->GetBackGroundscrollSmall();
	CDxFunc::MyDrawRotaGraph(m_pos + subPos, 1.0f, m_angle + m_imageInfo.m_rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.m_animePos]);

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 ) ;
}


void CBaseBullet::Move(){
	if(m_accelerate > 0.0){
		if(m_speed < m_maxsp){
			m_speed += m_accelerate;
		} else {
			m_speed = m_maxsp;
		}
	} else if(m_accelerate < 0.0){
		if(m_speed > m_maxsp){
			m_speed += m_accelerate;
		} else {
			m_speed = m_maxsp;
		}
	}

	m_angle += m_corner;

	m_vel.x = m_speed * cos(m_angle);
	m_vel.y = m_speed * sin(m_angle);

	m_pos += m_vel;
}


void CBaseBullet::RectOut(){
	//領域外に出たら削除
	const double bufferW = CGame::GetAllGameRect().x * 0.05;
	const double bufferH = CGame::GetAllGameRect().y * 0.05;
	if(	m_pos.x <	CGame::GetGameRect().leftUp.x		- m_imageInfo.m_sizeX/2	- bufferW ||
		m_pos.x >	CGame::GetGameRect().rightDown.x	+ m_imageInfo.m_sizeX/2 + bufferW ||
		m_pos.y <	CGame::GetGameRect().leftUp.y		- m_imageInfo.m_sizeY/2 - bufferH ||
		m_pos.y >	CGame::GetGameRect().rightDown.y	+ m_imageInfo.m_sizeY/2 + bufferH){
		m_removeFlg = true;
	}
}

// 弾が消える時の処理
void CBaseBullet::Remove(){
	if (m_enableRemoveEffect) {
		// 消滅エフェクト
		CBaseEffect* eff = new CBaseEffect(0, EDirType::Abs, m_pos, 0, 0, 0, 0, 0, 0, 20801); // "BulletDeleteEffect0"
		eff->SetSize(1.0, +0.0);
		eff->SetBlend(255, +0.0);
		eff->SetBlendType(DX_BLENDMODE_ADD);
		eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
		CBattleScene::m_effectManager.Add(eff);
	}
}

void CBaseBullet::SetRect(CRect &rect){
	CBaseBullet::m_rect = rect;
}



void CBaseBullet::Set(bool type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle){

	//角度タイプ
	m_type = type;

	//座標
	m_pos = P;
	//速度
	m_speed = speed * CGame::GetWindowRatio(); //　ウィンドウサイズ考慮

	//----------
	//狙い方向
	//----------
	//固定角度
	if(type == EDirType::Abs){
		m_angle = angle / CFunc::RAD;
	}
	//時期狙い角度
	else {
		if (nearAngle == 0) {
			m_angle = (CFunc::GetTwoPointAngle(m_pos, m_target) + angle/CFunc::RAD + (180.0/CFunc::RAD));
		} else {
			// 射角制限あり
			double ang = CFunc::GetTwoPointAngle(m_pos, m_target) * CFunc::RAD;
			double ang2 = CFunc::GetNearAngle(ang, nearAngle) / CFunc::RAD;
			m_angle = ang2 + angle / CFunc::RAD + (180.0 / CFunc::RAD);
		}
	}

	//速度
	m_vel.x = cos(m_angle) * m_speed;
	m_vel.y = sin(m_angle) * m_speed;

	//可変角度
	m_corner = corner / CFunc::RAD;

	//加速度
	m_accelerate = acce * CGame::GetWindowRatio(); //　ウィンドウサイズ考慮
	//最大速度
	m_maxsp = maxSpeed * CGame::GetWindowRatio(); //　ウィンドウサイズ考慮

	//カウント　毎フレーム刻む　いろいろ使う
	m_count = 0;

	m_blendType = DX_BLENDMODE_ALPHA;

	//弾を消す時にtrueにする
	m_removeFlg = false;	
	// 削除時のエフェクトアリ
	m_enableRemoveEffect = true;
	// 発射した敵は後でSetShotEnemyIdする必要あり
	m_shotEnemyId = -1;
}
void CBaseBullet::SetImage(int image){
	if(image == -1){
		m_image = nullptr;
		return ;
	}
	CBulletImage* bulletImage = (CBulletImage*)CGame::GetResource(image);

	SetImageInfo(image, bulletImage);
}

//void CBaseBullet::SetImage(const char* imageName){
//	CBulletImage* bulletImage = (CBulletImage*)CGame::GetResource(imageName);
//
//	SetImageInfo(bulletImage);
//}

void CBaseBullet::SetImageInfo(int imageNo, CBulletImage* bulletImage){

	m_imageInfo.m_imageNo = imageNo;

	//画像総合情報からidxに対応する情報を設定
	if(bulletImage->m_num > 1){
		m_imageInfo.m_animeFlg = true;
	} else {
		m_imageInfo.m_animeFlg = false;
	}
	m_imageInfo.m_animeNum = bulletImage->m_num;
	m_imageInfo.m_animePos = 0;
	m_imageInfo.m_animeSpeed = bulletImage->m_animeSpeed;


	if(bulletImage->m_rotaSpeed == 0.0){
		m_imageInfo.m_rotationFlg = false;
	} else {
		m_imageInfo.m_rotationFlg = true;
	}
	m_imageInfo.m_rotationSpeed = bulletImage->m_rotaSpeed / CFunc::RAD;
	m_imageInfo.m_rotationAngle = 0.0;

	m_imageInfo.m_sizeX = bulletImage->m_sizeX;
	m_imageInfo.m_sizeY = bulletImage->m_sizeY;
	
	//イメージ実データへのポインタ
	m_image = &(bulletImage->m_images[0]);
}


void CBaseBullet::SetTarget(CPos m_target){
	CBaseBullet::m_target = m_target;
}


void CBaseBullet::SetBlendType(int BlendType){
	m_blendType = BlendType;
}

// 弾が何かに当たった時
void CBaseBullet::Hit()
{
	m_removeFlg = true;
}

// 削除フラグ立てる
void CBaseBullet::SetRemove() {
	m_removeFlg = true;
}

void CBaseBullet::SetShotEnemyId(int shotEnemyId)
{
	m_shotEnemyId = shotEnemyId;
}