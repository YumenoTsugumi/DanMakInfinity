#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBullet.h"
#include "Resource.h"
#include "Game.h"

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

CBulletManager::~CBulletManager(){

	delete [] m_bullet;
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

void CBulletManager::Draw(){
	for(int i=0;i<m_bulletTotalNum;i++){
		if (m_bullet[i] == nullptr) {
			continue;
		}
		m_bullet[i]->Draw();
	}
}

//�z��̋󂫂�T���e����
int CBulletManager::Add(CBaseBullet *Bullet){
	for(int i=m_order;i<m_bulletTotalNum;i++){
		//�}���ʒu���ő�l�Ȃ�}���ʒu��0�ɖ߂�
		if(m_bullet[i] == nullptr){
			//�}���ʒu���C���N�������g��
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

	//������Ȃ��ꍇ�ŏ�����Ăу`�������W
	for(int i=m_order;i<m_bulletTotalNum;i++){
		//�}���ʒu���ő�l�Ȃ�}���ʒu��0�ɖ߂�
		if(m_bullet[i] == nullptr){
			//�}���ʒu���C���N�������g��
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

CBaseBullet::CBaseBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image){
	Set(type, P, speed, angle, corner, acce, maxSpeed, nearAngle);
	SetImage(image);
}
CBaseBullet::CBaseBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* imageName){
	Set(type, P, speed, angle, corner, acce, maxSpeed, nearAngle);
	SetImage(imageName);
}


CBaseBullet::~CBaseBullet(){

}

void CBaseBullet::Action(){
	//�ړ�
	Move();
	//�͈͊O����
	RectOut();

	m_count++;
}

void CBaseBullet::Draw(){
	SetDrawBlendMode( m_blendType , 255 ) ;

	//�A�j���t���O��ON�Ȃ�
	if(m_imageInfo.m_animeFlg == true){
		if(m_count % m_imageInfo.m_animeSpeed == 0){
			m_imageInfo.m_animePos++;
			if(m_imageInfo.m_animePos >= m_imageInfo.m_animeNum){
				m_imageInfo.m_animePos = 0;
			}
		}
	}
	//��]�t���O��ON�Ȃ�
	if(m_imageInfo.m_rotationFlg == true){
		m_imageInfo.m_rotationAngle += m_imageInfo.m_rotationSpeed;
	}
	//�`��
	CDxFunc::MyDrawRotaGraph(m_pos, 1.0f, m_angle + m_imageInfo.m_rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.m_animePos]);

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
	//�̈�O�ɏo����폜
	if(	m_pos.x <	CGame::GetBattleRect().leftUp.x		- m_imageInfo.m_sizeX/2	|| 
		m_pos.x >	CGame::GetBattleRect().rightDown.x	+ m_imageInfo.m_sizeX/2	|| 
		m_pos.y <	CGame::GetBattleRect().leftUp.y		- m_imageInfo.m_sizeY/2	|| 
		m_pos.y >	CGame::GetBattleRect().rightDown.y	+ m_imageInfo.m_sizeY/2	){
		m_removeFlg = true;
	}
}

void CBaseBullet::Remove(){

}

void CBaseBullet::SetRect(CRect &rect){
	CBaseBullet::m_rect = rect;
}



void CBaseBullet::Set(bool type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle){

	//�p�x�^�C�v
	m_type = type;

	//���W
	m_pos = P;
	//���x
	m_speed = speed;

	//----------
	//�_������
	//----------
	//�Œ�p�x
	if(type == EDirType::Abs){
		m_angle = angle / CFunc::RAD;
	}
	//�����_���p�x
	else {
		if (nearAngle == 0) {
			m_angle = (CFunc::GetTwoPointAngle(m_pos, m_target) + angle/CFunc::RAD + (180.0/CFunc::RAD));
		} else {
			// �ˊp��������
			double ang = CFunc::GetTwoPointAngle(m_pos, m_target) * CFunc::RAD;
			double ang2 = CFunc::GetNearAngle(ang, nearAngle) / CFunc::RAD;
			m_angle = ang2 + angle / CFunc::RAD + (180.0 / CFunc::RAD);
		}
	}

	//���x
	m_vel.x = cos(m_angle) * m_speed;
	m_vel.y = sin(m_angle) * m_speed;

	//�ϊp�x
	m_corner = corner / CFunc::RAD;

	//�����x
	m_accelerate = acce; 
	//�ő呬�x
	m_maxsp = maxSpeed;

	//�J�E���g�@���t���[�����ށ@���낢��g��
	m_count = 0;

	m_blendType = DX_BLENDMODE_ALPHA;

	//�e����������true�ɂ���
	m_removeFlg = false;	

}
void CBaseBullet::SetImage(int image){
	if(image == -1){
		m_image = nullptr;
		return ;
	}
	CBulletImage* bulletImage = (CBulletImage*)CGame::GetResource(image);

	SetImageInfo(bulletImage);
}

void CBaseBullet::SetImage(const char* ImageName){
	CBulletImage* bulletImage = (CBulletImage*)CGame::GetResource(ImageName);

	SetImageInfo(bulletImage);
}

void CBaseBullet::SetImageInfo(CBulletImage* bulletImage){
	//�摜������񂩂�idx�ɑΉ��������ݒ�
	if(bulletImage->m_num > 1){
		m_imageInfo.m_animeFlg = true;
	} else {
		m_imageInfo.m_animeFlg = false;
	}
	m_imageInfo.m_animeNum = bulletImage->m_num;
	m_imageInfo.m_animePos = 0;
	m_imageInfo.m_animeSpeed = bulletImage->m_animeSpeed;
	m_imageInfo.m_imageNo = bulletImage->m_index;

	if(bulletImage->m_rotaSpeed == 0.0){
		m_imageInfo.m_rotationFlg = false;
	} else {
		m_imageInfo.m_rotationFlg = true;
	}
	m_imageInfo.m_rotationSpeed = bulletImage->m_rotaSpeed / CFunc::RAD;
	m_imageInfo.m_rotationAngle = 0.0;

	m_imageInfo.m_sizeX = bulletImage->m_sizeX;
	m_imageInfo.m_sizeY = bulletImage->m_sizeY;
	
	//�C���[�W���f�[�^�ւ̃|�C���^
	m_image = &(bulletImage->m_images[0]);
}


void CBaseBullet::SetTarget(CPos m_target){
	CBaseBullet::m_target = m_target;
}


void CBaseBullet::SetBlendType(int BlendType){
	m_blendType = BlendType;
}

// �e�������ɓ���������
void CBaseBullet::Hit()
{
	m_removeFlg = true;
}