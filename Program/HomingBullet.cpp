
#include "DxLib.h"
#include "DXFunc.h"
#include "HomingBullet.h"
#include "Game.h"

CHomingLaser::CHomingLaser(EDirType type, CPos P, int NUM, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
				CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, image){
	m_num = NUM;
	Set();
}
//CHomingLaser::CHomingLaser(EDirType type, CPos P, int NUM, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName) :
//				CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, ImageName){
//	m_num = NUM;
//	Set();
//}
CHomingLaser::~CHomingLaser(){
	free(m_posA);
	//delete(m_limit);
}
void CHomingLaser::Action(){
	//移動
	Move();
	//範囲外判定
	RectOut();

	m_count++;
}
void CHomingLaser::Set(){
	m_posA = (CPos*)malloc(sizeof(CPos) * (m_num));
	m_angleA = (double*)malloc(sizeof(double) * (m_num));
	for(int i=0;i<m_num;i++){
		m_posA[i] = m_pos;
		m_angleA[i] = m_angle;
	}
	//m_limit = new CVoidArray;

	m_blendType = DX_BLENDMODE_ADD;
}

void CHomingLaser::Draw(){
	SetDrawBlendMode( m_blendType , 255 ) ;

	//描画
	for(int i=0;i<m_num;i++){
		if(i > m_count){
			break;
		}
		CDxFunc::MyDrawRotaGraph(m_posA[i], 0.2f, m_angleA[i] + m_imageInfo.m_rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.m_animePos]);
		//CDxFunc::MyDrawRotaGraph(m_posA[i], 0.2f, m_angleA[i] + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, m_image->m_index);
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 ) ;
}

void CHomingLaser::Move(){
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

	//自分の進行方向(角度)
	CPos movePos = CPos(m_posA[0].x - m_target.x, m_posA[0].y - m_target.y);
	double ang1 = CFunc::GetTwoVectorAngle(m_vel, movePos);

	double limitAng = 0.0 / CFunc::RAD;
	for(int i=(int)m_limit.size()-1;i>=0;i--){
		CLimitInfo *limitInfo = (CLimitInfo *)m_limit[i];
		if(m_count < limitInfo->m_count){
			limitAng = limitInfo->m_limitAngle;
		}
	}

	if(ang1 > limitAng){
		ang1 = limitAng;
	}
	
	//外積した結果で左右を判定
	double out = CFunc::OutsideProduct(m_vel, movePos);
	if(out > 0){
		m_angle -= ang1;
	} else if(out < 0){
		m_angle += ang1;
	}
	m_vel.x = m_speed * cos(m_angle);
	m_vel.y = m_speed * sin(m_angle);

	m_pos += m_vel;
	m_posA[0] += m_vel;
	m_angleA[0] = m_angle;

	for(int i=0;i<m_num-1;i++){
		m_posA[m_num-1-i] = m_posA[m_num-2-i];
		m_angleA[m_num-1-i] = m_angleA[m_num-2-i];
	}
}

void CHomingLaser::RectOut(){
	//領域外に出たら削除
	if(	m_posA[0].x <	CGame::GetBattleRect().leftUp.x - m_imageInfo.m_sizeX / 2 ||
		m_posA[0].x >	CGame::GetBattleRect().rightDown.x + m_imageInfo.m_sizeX / 2 ||
		m_posA[0].y <	CGame::GetBattleRect().leftUp.y - m_imageInfo.m_sizeY / 2 ||
		m_posA[0].y >	CGame::GetBattleRect().rightDown.y	+ m_imageInfo.m_sizeY/2	){

		int lastidx = m_num-1;
		if(	m_posA[lastidx].x <	CGame::GetBattleRect().leftUp.x - m_imageInfo.m_sizeX / 2 ||
			m_posA[lastidx].x >	CGame::GetBattleRect().rightDown.x + m_imageInfo.m_sizeX / 2 ||
			m_posA[lastidx].y <	CGame::GetBattleRect().leftUp.y - m_imageInfo.m_sizeY / 2 ||
			m_posA[lastidx].y >	CGame::GetBattleRect().rightDown.y	+ m_imageInfo.m_sizeY/2	){
			m_removeFlg = true;
		}
	}
}

void CHomingLaser::Remove(){

}

void CHomingLaser::AddLimitInfo(int Count, double Limit){
	CLimitInfo *limitInfo = new CLimitInfo;
	limitInfo->m_count = Count;
	limitInfo->m_limitAngle = Limit / CFunc::RAD;
	m_limit.push_back(limitInfo);
}