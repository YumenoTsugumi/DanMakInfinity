#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBullet.h"
#include "Resource.h"

//---------------------------------------------------------------------------------
//	CBulletImageInfo
//---------------------------------------------------------------------------------
CBulletImageInfo::CBulletImageInfo(){
	init();
}
void CBulletImageInfo::init(){
	m_imageNo = 0;
	sizeX = 0;
	sizeY = 0;

	animeFlg = false;
	animeNum = 0;
	animePos = 0;
	animeSpeed = 0;
	rotationFlg = false;
	rotationSpeed = 0.0;
	rotationAngle = 0.0;
}
void CBulletImageInfo::Set(bool animeFlg,char animeNum,char animePos,char animeSpeed,
						   bool rotationFlg, double rotationSpeed,double rotationAngle){
	this->animeFlg = animeFlg;
	this->animeNum = animeNum;
	this->animePos = animePos;
	this->animeSpeed = animeSpeed;
	this->rotationFlg = rotationFlg;
	this->rotationSpeed = rotationSpeed;
	this->rotationAngle = rotationAngle;
}

//---------------------------------------------------------------------------------
//	CBulletManager
//---------------------------------------------------------------------------------


CBulletManager::CBulletManager(int m_num/* = 1024*/){
	m_bulletTotalNum = m_num;

	m_bullet = new CBaseBullet*[m_bulletTotalNum];
	for(int i=0;i<m_bulletTotalNum;i++){
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}

CBulletManager::~CBulletManager(){

	delete [] m_bullet;
}

void CBulletManager::Action(){
	for(int i=0;i<m_bulletTotalNum;i++){
		if(m_bullet[i] != nullptr){
			m_bullet[i]->Action();
			if(m_bullet[i]->m_removeFlg == true){
				m_bullet[i]->Remove();
				delete m_bullet[i];
				m_bullet[i] = nullptr;
			}
		}
	}
}

void CBulletManager::Draw(){
	for(int i=0;i<m_bulletTotalNum;i++){
		if(m_bullet[i] != nullptr){
			m_bullet[i]->Draw();
		}
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
//CBulletImageManager* CBaseBullet::m_imageManager = nullptr;
CResourceManager* CBaseBullet::m_imageManager = nullptr;

CBaseBullet::CBaseBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG){
	Set(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP);
	SetImage(IMG);
}
CBaseBullet::CBaseBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME){
	Set(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP);
	SetImage(IMAGENAME);
}


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
	if(m_imageInfo.animeFlg == true){
		if(m_count % m_imageInfo.animeSpeed == 0){
			m_imageInfo.animePos++;
			if(m_imageInfo.animePos >= m_imageInfo.animeNum){
				m_imageInfo.animePos = 0;
			}
		}
	}
	//回転フラグがONなら
	if(m_imageInfo.rotationFlg == true){
		m_imageInfo.rotationAngle += m_imageInfo.rotationSpeed;
	}
	//描画
	CDxFunc::DrawRotaGraph(m_pos, 1.0f, m_angle + m_imageInfo.rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.animePos]);

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
	if(	m_pos.x <	CBaseBullet::m_rect.leftUp.x		- m_imageInfo.sizeX/2	|| 
		m_pos.x >	CBaseBullet::m_rect.rightDown.x	+ m_imageInfo.sizeX/2	|| 
		m_pos.y <	CBaseBullet::m_rect.leftUp.y		- m_imageInfo.sizeY/2	|| 
		m_pos.y >	CBaseBullet::m_rect.rightDown.y	+ m_imageInfo.sizeY/2	){
		m_removeFlg = true;
	}
}

void CBaseBullet::Remove(){

}

void CBaseBullet::SetRect(CRect &rect){
	CBaseBullet::m_rect = rect;
}



void CBaseBullet::Set(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP){

	//角度タイプ
	m_type = TYPE;

	//座標
	m_pos = P;
	//速度
	m_speed = SPEED;

	//----------
	//狙い方向
	//----------
	//固定角度
	if(TYPE == ABS){
		m_angle = ANGLE / CFunc::RAD;
	}
	//時期狙い角度
	else {
		m_angle = (CFunc::GetTwoPointAngle(m_pos, m_target) + ANGLE/CFunc::RAD + (180.0/CFunc::RAD));
	}

	//速度
	m_vel.x = cos(m_angle) * m_speed;
	m_vel.y = sin(m_angle) * m_speed;

	//可変角度
	m_corner = CORNER / CFunc::RAD;

	//加速度
	m_accelerate = ACCE; 
	//最大速度
	m_maxsp = MAXSP;

	//カウント　毎フレーム刻む　いろいろ使う
	m_count = 0;

	m_blendType = DX_BLENDMODE_ALPHA;

	//弾を消す時にtrueにする
	m_removeFlg = false;	

}
void CBaseBullet::SetImage(int IMG){
	if(IMG == -1){
		m_image = nullptr;
		return ;
	}
	CBulletImage* bulletImage;
	bulletImage = (CBulletImage*)m_imageManager->GetResource(IMG);

	SetImageInfo(bulletImage);
}

void CBaseBullet::SetImage(const char* IMAGENAME){
	CBulletImage* bulletImage;
	bulletImage = (CBulletImage*)m_imageManager->GetResource(IMAGENAME);

	SetImageInfo(bulletImage);
}
void CBaseBullet::SetImageInfo(CBulletImage* bulletImage){
	//画像総合情報からidxに対応する情報を設定
	if(bulletImage->m_num > 1){
		m_imageInfo.animeFlg = true;
	} else {
		m_imageInfo.animeFlg = false;
	}
	m_imageInfo.animeNum = bulletImage->m_num;
	m_imageInfo.animePos = 0;
	m_imageInfo.animeSpeed = bulletImage->m_animeSpeed;
	m_imageInfo.m_imageNo = bulletImage->info.m_index;

	if(bulletImage->m_rotaSpeed == 0.0){
		m_imageInfo.rotationFlg = false;
	} else {
		m_imageInfo.rotationFlg = true;
	}
	m_imageInfo.rotationSpeed = bulletImage->m_rotaSpeed / CFunc::RAD;
	m_imageInfo.rotationAngle = 0.0;

	m_imageInfo.sizeX = bulletImage->m_sizeX;
	m_imageInfo.sizeY = bulletImage->m_sizeY;
	
	//イメージ実データへのポインタ
	m_image = &(bulletImage->m_images[0]);
}


void CBaseBullet::SetTarget(CPos m_target){
	CBaseBullet::m_target = m_target;
}

void CBaseBullet::SetResource(CResourceManager *ImageManager){
	m_imageManager = ImageManager;
}

void CBaseBullet::SetBlendType(int BlendType){
	m_blendType = BlendType;
}

