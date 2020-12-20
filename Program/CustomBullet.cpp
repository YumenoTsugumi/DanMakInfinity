#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBullet.h"
#include "CustomBullet.h"

CBulletManager* CCustomBullet::m_manager = nullptr;

CCustomBullet::CCustomBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG) : 
				CBaseBullet(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, IMG){
	//�e�̃R���X�g���N�^���Ă΂ꂽ���ƁiSet�Ƃ��Őݒ�j�R�R�ɗ���
	//�p���N���X�̐ݒ�
	Set();

	//�l�������p��NOTRANS�̏ꍇ�A���̂܂܂̒l�i99999�j�ň����p�������肵�Ă��邽��
	//�R���X�g���N�^��/RAD���ꂽ�獢��
	//���ׁ̈A�����ŏ㏑��
	if(ANGLE == NOTRANS){
		m_angle = ANGLE;
	}
}
CCustomBullet::CCustomBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME) : 
				CBaseBullet(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, IMAGENAME){
	//�e�̃R���X�g���N�^���Ă΂ꂽ���ƁiSet�Ƃ��Őݒ�j�R�R�ɗ���
	//�p���N���X�̐ݒ�
	Set();

	//�l�������p��NOTRANS�̏ꍇ�A���̂܂܂̒l�i99999�j�ň����p�������肵�Ă��邽��
	//�R���X�g���N�^��/RAD���ꂽ�獢��
	//���ׁ̈A�����ŏ㏑��
	if(ANGLE == NOTRANS){
		m_angle = ANGLE;
	}
}


CCustomBullet::~CCustomBullet(){
	if(transBullet != nullptr){
		delete transBullet;
	}
}

void CCustomBullet::Set(){
	m_transFrame = 0;
	transAngle = 0;
	transBullet = nullptr;

	m_addFrame = 0;
	addFuncP = nullptr;

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
	if(m_standByCount < m_standByTime){
		//���̉���
		int alpha = (int)((double)255 * ((double)m_standByCount / m_standByTime ));

		SetDrawBlendMode( DX_BLENDMODE_ALPHA , alpha ) ;
		CDxFunc::DrawRotaGraph(m_pos.x, m_pos.y, 1.0, m_angle + m_imageInfo.rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.animePos]);
	} else {
		//�ʏ�`��
		CBaseBullet::Draw();
	}
}

void CCustomBullet::SetTransBullet(CCustomBullet* Bullet, int Frame){
	m_transFrame = Frame;
	transBullet = Bullet;
}

void CCustomBullet::SetTransAngle(double Angle){
	if(m_transFrame > 0){
		//nullptr�`�F�b�N���Ă���
		if(transBullet == nullptr){
			return;
		}
		if(transBullet->m_type == ABS){
			return;
		}
		if(transBullet->m_angle == NOTRANS){
			return;
		}
		transAngle = Angle / CFunc::RAD;
	}
}

void CCustomBullet::Trans(){
	if(m_transFrame > 0 && m_count >= m_transFrame){
		//nullptr�`�F�b�N���Ă���
		if(transBullet == nullptr){
			//����nullptr�Ȃ�ω��p�����[�^��������
			m_transFrame = 0;
			return;
		}
		////�l�̈��p��
		////�ŏ��͒l�������p����addBullet����\�肾�������ǁA����������Remove�t���O��ON�ɂ��ď����ۂ�
		////�G�t�F�N�g����������H�\��������A�߂�ǂ������Ȃ����B
		////������l�̏㏑����
		//transBullet->pos = pos;
		//if(transBullet->vel == NOTRANS){
		//	transBullet->vel = vel;		
		//}
		//if(transBullet->m_angle == NOTRANS){
		//	transBullet->m_angle = m_angle;
		//} else {
		//	if(transBullet->m_type == PLAYER){
		//		m_angle = (CFunc::GetTwoPointAngle(pos, m_target) + transAngle + (180.0/CFunc::RAD));
		//	}
		//}
		//vel.x = cos(m_angle) * m_speed;
		//vel.y = sin(m_angle) * m_speed;

		//if(transBullet->m_corner == NOTRANS){
		//	transBullet->m_corner = m_corner;		
		//}
		//if(transBullet->m_accelerate == NOTRANS){
		//	transBullet->m_accelerate = m_accelerate;		
		//}
		//if(transBullet->m_maxsp == NOTRANS){
		//	transBullet->m_maxsp = m_maxsp;		
		//}
		////�摜no���قȂ�ꍇ�㏑��
		//if(transBullet->m_imageInfo.m_imageNo == m_imageInfo.m_imageNo){
		//	transBullet->m_imageInfo = m_imageInfo;
		//	transBullet->m_image = m_image;	//�Ӗ��Ȃ�
		//}
		//transBullet = nullptr;
		//Remove = true;

		//�ω��e�̒l�����݂̒e�ɏ㏑������
		//���̍ۏ��������X
		if(transBullet->m_speed != NOTRANS){
			m_speed = transBullet->m_speed;		
		}
		if(transBullet->m_angle != NOTRANS){
			if(transBullet->m_type == PLAYER){
				m_angle = (CFunc::GetTwoPointAngle(m_pos, m_target) + transBullet->transAngle + (180.0/CFunc::RAD));
			} else {
				m_angle = transBullet->m_angle;
			}
		}

		//���x�Čv�Z
		m_vel.x = cos(m_angle) * m_speed;
		m_vel.y = sin(m_angle) * m_speed;

		if(transBullet->m_corner != NOTRANS){
			m_corner = transBullet->m_corner;
		}
		if(transBullet->m_accelerate != NOTRANS){
			m_accelerate = transBullet->m_accelerate;
		}
		if(transBullet->m_maxsp != NOTRANS){
			m_maxsp = transBullet->m_maxsp;	
		}
		//�摜no���قȂ�ꍇ�㏑��
		if(transBullet->m_imageInfo.m_imageNo != m_imageInfo.m_imageNo){
			m_imageInfo = transBullet->m_imageInfo;
			m_image = transBullet->m_image;
		}

		m_count = 0;
		m_removeFlg = false;

		//����p�ɃA�h���X��ۑ����Ă���
		CCustomBullet* wrk;
		wrk = transBullet;
		
		//�ǉ��e
		m_addFrame = transBullet->m_addFrame;
		addFuncP = transBullet->addFuncP;
		//addBullet = transBullet->addBullet;
		//�ω��e
		m_transFrame = transBullet->m_transFrame;
		transBullet = transBullet->transBullet;	//���̏����͈�ԍŌ�

		//�e��nullptr�ɂ��Ă����Ȃ��ƃf�X�g���N�^�ŉ�����ꂿ�Ⴄ
		wrk->transBullet = nullptr;

		//���ׂď㏑�������������ׁA���
		delete wrk;

		int debug = 0;
		debug++;
	}
}

void CCustomBullet::SetAddBullet(void (*funcP)(CCustomBullet* m_bullet), int AddFrame){
	addFuncP = funcP;
	m_addFrame = AddFrame;
}

void CCustomBullet::Add(){
	if(m_addFrame > 0 && m_count >= m_addFrame){
		//nullptr�`�F�b�N���Ă���
		if(addFuncP == nullptr){
			//����nullptr�Ȃ�ω��p�����[�^��������
			m_addFrame = 0;
			return;
		}
		if(m_count % m_addFrame == 0){
			//�֐��|�C���^����e��f��
			(addFuncP)(this);
		}
	}
}

void CCustomBullet::SetStandByTime(int Time){
	m_standByTime = Time;
	m_standByCount = 0;
}