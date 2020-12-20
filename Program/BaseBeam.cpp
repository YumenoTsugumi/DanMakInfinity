#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBeam.h"

//---------------------------------------------------------------------------------
//	CBaseBeamManager
//---------------------------------------------------------------------------------

CBaseBeamManager::CBaseBeamManager(int num/* = 64*/) {
	m_beamTotalNum = num;

	m_beam = new CBaseBeam * [m_beamTotalNum];
	for (int i = 0; i < m_beamTotalNum; i++) {
		m_beam[i] = nullptr;
	}

	m_order = 0;
}
CBaseBeamManager::~CBaseBeamManager() {
	delete[] m_beam;
}

void CBaseBeamManager::Action() {
	for (int i = 0; i < m_beamTotalNum; i++) {
		if (m_beam[i] != nullptr) {
			m_beam[i]->Action();
			if (m_beam[i]->m_removeFlg == true) {
				m_beam[i]->Remove();
				delete m_beam[i];
				m_beam[i] = nullptr;
			}
		}
	}
}
void CBaseBeamManager::Draw() {
	for (int i = 0; i < m_beamTotalNum; i++) {
		if (m_beam[i] != nullptr) {
			m_beam[i]->Draw();
		}
	}
}
int CBaseBeamManager::Add(CBaseBeam* Beam) {
	for (int i = m_order; i < m_beamTotalNum; i++) {
		//�}���ʒu���ő�l�Ȃ�}���ʒu��0�ɖ߂�
		if (m_beam[i] == nullptr) {
			//�}���ʒu���C���N�������g��
			m_order++;
			if (m_order >= m_beamTotalNum) {
				m_order = 0;
			}

			m_beam[i] = Beam;
			return i;
		}
		if (m_order >= m_beamTotalNum - 1) {
			m_order = 0;
		}
	}

	//������Ȃ��ꍇ�ŏ�����Ăу`�������W
	for (int i = m_order; i < m_beamTotalNum; i++) {
		//�}���ʒu���ő�l�Ȃ�}���ʒu��0�ɖ߂�
		if (m_beam[i] == nullptr) {
			//�}���ʒu���C���N�������g��
			m_order++;
			if (m_order >= m_beamTotalNum) {
				m_order = 0;
			}

			m_beam[i] = Beam;
			return i;
		}
		if (m_order >= m_beamTotalNum - 1) {
			m_order = 0;
		}
	}

	m_order = 0;
	delete Beam;
	return -1;
}


//---------------------------------------------------------------------------------
//	CBaseBeam
//---------------------------------------------------------------------------------
CRect CBaseBeam::m_bigRect;
CResourceManager* CBaseBeam::m_imageManager = nullptr;


CBaseBeam::CBaseBeam(CPos P, double ANGLE, int IMGNO, int num /*= 128*/) {
	//���W, ���x, �p�x, ����, ����, ���ˊԊu
	Set(P, 10.0, ANGLE, -1, -1, 1);
	SetImage(IMGNO);

	m_bulletTotalNum = num;

	m_bullet = new CBaseBullet * [m_bulletTotalNum];
	for (int i = 0; i < m_bulletTotalNum; i++) {
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}
CBaseBeam::CBaseBeam(CPos P, double ANGLE, const char* IMGNAME, int num /*= 128*/) {
	//���W, ���x, �p�x, ����, ����, ���ˊԊu
	Set(P, 10.0, ANGLE, -1, -1, 1);
	SetImage(IMGNAME);

	m_bulletTotalNum = num;

	m_bullet = new CBaseBullet * [m_bulletTotalNum];
	for (int i = 0; i < m_bulletTotalNum; i++) {
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}

void CBaseBeam::SetImage(int IMG) {
	m_image = (CImageSet*)m_imageManager->GetResource(IMG);
}

void CBaseBeam::SetImage(const char* IMAGENAME) {
	m_image = (CImageSet*)m_imageManager->GetResource(IMAGENAME);
}


CBaseBeam::~CBaseBeam() {
	Clear();
}

void CBaseBeam::Clear() {
	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] != nullptr) {
			m_bullet[i]->Remove();
			delete m_bullet[i];
			m_bullet[i] = nullptr;
		}
	}

	delete[] * m_bullet;
}

void CBaseBeam::Set(CPos Pos, double Speed, double Angle, int Length, double Thick, int ShotSpan) {
	m_pos = Pos;
	m_speed = Speed;
	m_angle = Angle;
	m_drawAngle = Angle / CFunc::RAD;
	m_length = Length;
	m_thick = Thick;
	m_shotSpan = ShotSpan;

	m_wait = -1;
	m_removeTime = INT_MAX;
	m_removeNow = false;
	m_removeSize = 1.0;

	m_count = 0;
}

void CBaseBeam::Action() {
	if (m_count > m_wait) {
		//�ҋ@���Ԃ�m_count�𒴂���܂ł͏Ǝ˂��Ȃ�

		if (m_count % m_shotSpan == 0) {
			//�����x�A�摜����0���f�t�H���g
			//�摜���w�肵�����Ȃ��������ǁA�}�C�i�X�Ƃ��w��ł��Ȃ��悤�ɂ��Ă邩��0��
			CBaseBeamChild* b = new CBaseBeamChild(ABS, m_pos, m_speed, m_angle, 0, 0, 0, 0);
			Add(b);
		}
	}
	//�폜���Ԃ𒴂�����폜
	if (m_count > m_removeTime) {
		m_removeNow = true;
	}
	//�폜���Ȃ�ǂ�ǂ񏬂�������
	if (m_removeNow == true) {
		m_removeSize -= 0.05;
		if (m_removeSize < 0) {
			m_removeSize = 0.0;
			//�v�f�̍폜
			m_removeFlg = true;
		}
	}

	//��]�Ƃ��Ŗ��t���[���Q�Ƃ����Ɨ�����̂�
	//�r�[���͊Ǘ�����SetRemove���Ȃ��ƍ폜���Ȃ�
	if (m_pos.x <	CBaseBeam::m_bigRect.leftUp.x ||
		m_pos.x >	CBaseBeam::m_bigRect.rightDown.x ||
		m_pos.y <	CBaseBeam::m_bigRect.leftUp.y ||
		m_pos.y >	CBaseBeam::m_bigRect.rightDown.y) {
		//m_removeFlg = true;
	}

	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] != nullptr) {
			m_bullet[i]->Action();

			//�J�E���g���ݒ肳��Ă���
			//�J�E���g���������������炻������폜����
			if (m_length > 0 && m_bullet[i]->m_count > m_length) {
				m_bullet[i]->m_removeFlg = true;
			}

			if (m_bullet[i]->m_pos.x <	CBaseBeam::m_bigRect.leftUp.x ||
				m_bullet[i]->m_pos.x >	CBaseBeam::m_bigRect.rightDown.x ||
				m_bullet[i]->m_pos.y <	CBaseBeam::m_bigRect.leftUp.y ||
				m_bullet[i]->m_pos.y >	CBaseBeam::m_bigRect.rightDown.y) {
				m_bullet[i]->m_removeFlg = true;
			}

			//�폜
			if (m_bullet[i]->m_removeFlg == true) {
				m_bullet[i]->Remove();
				delete m_bullet[i];
				m_bullet[i] = nullptr;
			}
		}
	}

	m_count++;
}

void CBaseBeam::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	//GetImage
	CImage* standImg = m_image->GetImage(0);
	CImage* laserImg = m_image->GetImage(1);

	//���
	double siz = 1.0 * m_removeSize;
	CDxFunc::DrawRotaGraph(m_pos.x, m_pos.y, siz, 0.0, standImg->m_iamge);
	CDxFunc::DrawRotaGraph(m_pos.x, m_pos.y, siz * 0.8, 0.0, standImg->m_iamge);

	//�폜���łȂ���Η\������\������
	if (m_removeNow == false) {
		//�\����
		//�f�t�H���g-1������A-1�łȂ����length���Q��
		int len = m_bulletTotalNum;
		if (m_length != -1) {
			len = m_length;
		}
		//*2�Ȃ̂͊Ԋu���������邽��
		for (int i = 0; i < (len * 2 - 2); i++) {
			double x, y;
			x = m_pos.x + cos(m_drawAngle) * (double)i / 2 * m_speed;
			y = m_pos.y + sin(m_drawAngle) * (double)i / 2 * m_speed;
			CDxFunc::DrawRotaGraph(x, y, 0.12, m_drawAngle + 90.0 / CFunc::RAD, laserImg->m_iamge);
		}
	}

	//�q��Draw�ɔC�����A�e���`�悷��
	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] != nullptr) {
			double bure = CFunc::RandF(-5, 5) / CFunc::RAD;
			//�폜�����Ƃ��񂾂񏬂�������
			double siz = 1.0 * m_removeSize;
			CDxFunc::DrawRotaGraph(m_bullet[i]->m_pos, siz, m_drawAngle + 90.0 / CFunc::RAD + bure, laserImg->m_iamge);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

//void CBaseBeam::setImageData(CBeamImageManager *BeamImageManager){
//	beamImageManager = BeamImageManager;
//}
void CBaseBeam::SetResource(CResourceManager* ImageManager) {
	m_imageManager = ImageManager;
}



void CBaseBeam::Remove() {

}
void CBaseBeam::SetBigRect(CRect& bigrect) {
	CBaseBeam::m_bigRect = bigrect;
}


void CBaseBeam::SetLength(int Length) {
	m_length = Length;
}
void CBaseBeam::SetWait(int Wait) {
	m_wait = Wait;
}
void CBaseBeam::SetRemoveTime(int DelTime) {
	m_removeTime = DelTime;
}
void CBaseBeam::SetRemove() {
	m_removeTime = -1;
	m_removeNow = true;
}

void CBaseBeam::Move(CPos& newpos, double plusAngle) {
	double ang = plusAngle / CFunc::RAD;

	//�`��p�x���Z
	m_drawAngle += ang;
	//�r�[���������Z
	m_angle += plusAngle;

	CPos subpos;
	subpos.x = newpos.x - m_pos.x;
	subpos.y = newpos.y - m_pos.y;

	m_pos = newpos;

	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] != nullptr) {
			m_bullet[i]->m_angle = m_angle / CFunc::RAD;
			m_bullet[i]->m_vel.x = cos(m_bullet[i]->m_angle) * m_bullet[i]->m_speed;
			m_bullet[i]->m_vel.y = sin(m_bullet[i]->m_angle) * m_bullet[i]->m_speed;

			CFunc::RotatingMatrix(&(m_bullet[i]->m_pos), m_bullet[i]->m_pos, m_pos, -ang);
			m_bullet[i]->m_pos += subpos;
		}
	}
}
//---------------------------------------------------------------------------------
//	CBaseBeamChild
//---------------------------------------------------------------------------------


CBaseBeamChild::CBaseBeamChild(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG) :
	CBaseBullet(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, IMG) {

}

CBaseBeamChild::~CBaseBeamChild() {

}

void CBaseBeamChild::Remove() {
	//�������Ȃ�
	//�G�t�F�N�g�o�������Ȃ�
}

void CBaseBeamChild::RectOut() {
	//�̈�O�ɏo����폜�������Ȃ�
	//�폜�͏�ʂ�����
	//if(	m_pos.x <	CBaseBeamChild::m_bigRect.leftUp.x		|| 
	//	m_pos.x >	CBaseBeamChild::m_bigRect.rightDown.x	|| 
	//	m_pos.y <	CBaseBeamChild::m_bigRect.leftUp.y		|| 
	//	m_pos.y >	CBaseBeamChild::m_bigRect.rightDown.y	){
	//	m_removeFlg = true;
	//}
}



