#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBullet.h"
#include "CustomBullet.h"
#include "BattleScene.h"
CBulletManager* CCustomBullet::m_manager = nullptr;

CCustomBullet::CCustomBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
				CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, image){
	//�e�̃R���X�g���N�^���Ă΂ꂽ���ƁiSet�Ƃ��Őݒ�j�R�R�ɗ���
	//�p���N���X�̐ݒ�
	Set();

	//�l�������p��NOTRANS�̏ꍇ�A���̂܂܂̒l�i99999�j�ň����p�������肵�Ă��邽��
	//�R���X�g���N�^��/RAD���ꂽ�獢��
	//���ׁ̈A�����ŏ㏑��
	if(angle == NOTRANS){
		m_angle = angle;
	}
}
//CCustomBullet::CCustomBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName) :
//				CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, ImageName){
//	//�e�̃R���X�g���N�^���Ă΂ꂽ���ƁiSet�Ƃ��Őݒ�j�R�R�ɗ���
//	//�p���N���X�̐ݒ�
//	Set();
//
//	//�l�������p��NOTRANS�̏ꍇ�A���̂܂܂̒l�i99999�j�ň����p�������肵�Ă��邽��
//	//�R���X�g���N�^��/RAD���ꂽ�獢��
//	//���ׁ̈A�����ŏ㏑��
//	if(angle == NOTRANS){
//		m_angle = angle;
//	}
//}


CCustomBullet::~CCustomBullet(){
	if(m_transBullet != nullptr){
		delete m_transBullet;
	}
}

void CCustomBullet::Set(){
	m_transFrame = 0;
	m_transAngle = 0;
	m_transBullet = nullptr;

	m_addFrame = 0;
	m_addFuncP = nullptr;

	m_standByCount = 0;
}

void CCustomBullet::SetBulletManagerPointer(CBulletManager *Manager){
	m_manager = Manager;
}


void CCustomBullet::Action(){
	//���̉���
	if(m_standByCount < m_standByTime){
		m_standByCount++;
		return;
	}

	//�ړ�
	Move();

	//�e�ω�
	Trans();
	//�e����
	Add();

	//�͈͊O����
	RectOut();
	//�폜
	Remove();

	m_count++;
}

void CCustomBullet::Draw(){
	CPos subPos = CBattleScene::GetBattleScene()->GetBackGroundscrollSmall();

	if(m_standByCount < m_standByTime){
		//���̉���
		int alpha = (int)((double)255 * ((double)m_standByCount / m_standByTime ));

		SetDrawBlendMode( DX_BLENDMODE_ALPHA , alpha ) ;
		CDxFunc::MyDrawRotaGraph(m_pos + subPos, 1.0, m_angle + m_imageInfo.m_rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.m_animePos]);
	} else {
		//�ʏ�`��
		CBaseBullet::Draw();
	}
}

void CCustomBullet::SetTransBullet(CCustomBullet* Bullet, int Frame){
	m_transFrame = Frame;
	m_transBullet = Bullet;
}

void CCustomBullet::SetTransAngle(double Angle){
	if(m_transFrame > 0){
		//nullptr�`�F�b�N���Ă���
		if(m_transBullet == nullptr){
			return;
		}
		if(m_transBullet->m_type == EDirType::Abs){
			return;
		}
		if(m_transBullet->m_angle == NOTRANS){
			return;
		}
		m_transAngle = Angle / CFunc::RAD;
	}
}

void CCustomBullet::Trans(){
	if(m_transFrame > 0 && m_count >= m_transFrame){
		//nullptr�`�F�b�N���Ă���
		if(m_transBullet == nullptr){
			//����nullptr�Ȃ�ω��p�����[�^��������
			m_transFrame = 0;
			return;
		}
		////�l�̈��p��
		////�ŏ��͒l�������p����addBullet����\�肾�������ǁA����������Remove�t���O��ON�ɂ��ď����ۂ�
		////�G�t�F�N�g����������H�\��������A�߂�ǂ������Ȃ����B
		////������l�̏㏑����
		//m_transBullet->pos = pos;
		//if(m_transBullet->vel == NOTRANS){
		//	m_transBullet->vel = vel;		
		//}
		//if(m_transBullet->m_angle == NOTRANS){
		//	m_transBullet->m_angle = m_angle;
		//} else {
		//	if(m_transBullet->m_type == EDirType::Player){
		//		m_angle = (CFunc::GetTwoPointAngle(pos, m_target) + m_transAngle + (180.0/CFunc::RAD));
		//	}
		//}
		//vel.x = cos(m_angle) * m_speed;
		//vel.y = sin(m_angle) * m_speed;

		//if(m_transBullet->m_corner == NOTRANS){
		//	m_transBullet->m_corner = m_corner;		
		//}
		//if(m_transBullet->m_accelerate == NOTRANS){
		//	m_transBullet->m_accelerate = m_accelerate;		
		//}
		//if(m_transBullet->m_maxsp == NOTRANS){
		//	m_transBullet->m_maxsp = m_maxsp;		
		//}
		////�摜no���قȂ�ꍇ�㏑��
		//if(m_transBullet->m_imageInfo.m_imageNo == m_imageInfo.m_imageNo){
		//	m_transBullet->m_imageInfo = m_imageInfo;
		//	m_transBullet->m_image = m_image;	//�Ӗ��Ȃ�
		//}
		//m_transBullet = nullptr;
		//Remove = true;

		//�ω��e�̒l�����݂̒e�ɏ㏑������
		//���̍ۏ��������X
		if(m_transBullet->m_speed != NOTRANS){
			m_speed = m_transBullet->m_speed;		
		}
		if(m_transBullet->m_angle != NOTRANS){
			if(m_transBullet->m_type == EDirType::Player){
				m_angle = (CFunc::GetTwoPointAngle(m_pos, m_target) + m_transBullet->m_transAngle + (180.0/CFunc::RAD));
			} else {
				m_angle = m_transBullet->m_angle;
			}
		}

		//���x�Čv�Z
		m_vel.x = cos(m_angle) * m_speed;
		m_vel.y = sin(m_angle) * m_speed;

		if(m_transBullet->m_corner != NOTRANS){
			m_corner = m_transBullet->m_corner;
		}
		if(m_transBullet->m_accelerate != NOTRANS){
			m_accelerate = m_transBullet->m_accelerate;
		}
		if(m_transBullet->m_maxsp != NOTRANS){
			m_maxsp = m_transBullet->m_maxsp;	
		}
		//�摜no���قȂ�ꍇ�㏑��
		if(m_transBullet->m_imageInfo.m_imageNo != m_imageInfo.m_imageNo){
			m_imageInfo = m_transBullet->m_imageInfo;
			m_image = m_transBullet->m_image;
		}

		m_count = 0;
		m_removeFlg = false;

		//����p�ɃA�h���X��ۑ����Ă���
		CCustomBullet* wrk;
		wrk = m_transBullet;
		
		//�ǉ��e
		m_addFrame = m_transBullet->m_addFrame;
		m_addFuncP = m_transBullet->m_addFuncP;
		//addBullet = m_transBullet->addBullet;
		//�ω��e
		m_transFrame = m_transBullet->m_transFrame;
		m_transBullet = m_transBullet->m_transBullet;	//���̏����͈�ԍŌ�

		//�e��nullptr�ɂ��Ă����Ȃ��ƃf�X�g���N�^�ŉ�����ꂿ�Ⴄ
		wrk->m_transBullet = nullptr;

		//���ׂď㏑�������������ׁA���
		delete wrk;

		int debug = 0;
		debug++;
	}
}

void CCustomBullet::SetAddBullet(void (*funcP)(CCustomBullet* m_bullet), int AddFrame){
	m_addFuncP = funcP;
	m_addFrame = AddFrame;
}

void CCustomBullet::Add(){
	if(m_addFrame > 0 && m_count >= m_addFrame){
		//nullptr�`�F�b�N���Ă���
		if(m_addFuncP == nullptr){
			//����nullptr�Ȃ�ω��p�����[�^��������
			m_addFrame = 0;
			return;
		}
		if(m_count % m_addFrame == 0){
			//�֐��|�C���^����e��f��
			(m_addFuncP)(this);
		}
	}
}

void CCustomBullet::SetStandByTime(int Time){
	m_standByTime = Time;
	m_standByCount = 0;
}