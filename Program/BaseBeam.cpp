#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBeam.h"
#include "Game.h"

//---------------------------------------------------------------------------------
//	CBeamManager
//---------------------------------------------------------------------------------

CBeamManager::CBeamManager(int num/* = 64*/) {
	m_beamTotalNum = num;

	m_beam = new CBaseBeam * [m_beamTotalNum];
	for (int i = 0; i < m_beamTotalNum; i++) {
		m_beam[i] = nullptr;
	}

	m_order = 0;
}
CBeamManager::~CBeamManager() {
	delete[] m_beam;
}

void CBeamManager::Action() {
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
void CBeamManager::Draw() {
	for (int i = 0; i < m_beamTotalNum; i++) {
		if (m_beam[i] != nullptr) {
			m_beam[i]->Draw();
		}
	}
}
int CBeamManager::Add(CBaseBeam* Beam) {
	for (int i = m_order; i < m_beamTotalNum; i++) {
		//挿入位置が最大値なら挿入位置を0に戻す
		if (m_beam[i] == nullptr) {
			//挿入位置をインクリメントし
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

	//見つからない場合最初から再びチャレンジ
	for (int i = m_order; i < m_beamTotalNum; i++) {
		//挿入位置が最大値なら挿入位置を0に戻す
		if (m_beam[i] == nullptr) {
			//挿入位置をインクリメントし
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
//CResourceManager* CBaseBeam::m_imageManager = nullptr;


CBaseBeam::CBaseBeam(CPos P, double angle, int imageIndex1, int imageIndex2, int num /*= 128*/) {
	//座標, 速度, 角度, 長さ, 太さ, 発射間隔
	Set(P, 10.0, angle, -1, -1, 1);
	SetImage(imageIndex1, imageIndex2);

	m_bulletTotalNum = num;

	m_bullet = new CBaseBullet * [m_bulletTotalNum];
	for (int i = 0; i < m_bulletTotalNum; i++) {
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}
CBaseBeam::CBaseBeam(CPos P, double angle, const char* imageName1, const char* imageName2, int num /*= 128*/) {
	//座標, 速度, 角度, 長さ, 太さ, 発射間隔
	Set(P, 10.0, angle, -1, -1, 1);
	SetImage(imageName1, imageName2);

	m_bulletTotalNum = num;

	m_bullet = new CBaseBullet * [m_bulletTotalNum];
	for (int i = 0; i < m_bulletTotalNum; i++) {
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}

void CBaseBeam::SetImage(int imageIndex1, int imageIndex2) {
	m_imageStand = (CImage*)CGame::GetResource(imageIndex1);
	m_imageLaser = (CImage*)CGame::GetResource(imageIndex2);
}
void CBaseBeam::SetImage(const char* imageName1, const char* imageName2) {
	m_imageStand = (CImage*)CGame::GetResource(imageName1);
	m_imageLaser = (CImage*)CGame::GetResource(imageName2);
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
		//待機時間をm_countを超えるまでは照射しない

		if (m_count % m_shotSpan == 0) {
			//加速度、画像等は0がデフォルト
			//画像を指定したくなかったけど、マイナスとか指定できないようにしてるから0で
			CBaseBeamChild* b = new CBaseBeamChild(EDirType::Abs, m_pos, m_speed, m_angle, 0, 0, 0, 0,0);
			Add(b);
		}
	}
	//削除時間を超えたら削除
	if (m_count > m_removeTime) {
		m_removeNow = true;
	}
	//削除中ならどんどん小さくする
	if (m_removeNow == true) {
		m_removeSize -= 0.05;
		if (m_removeSize < 0) {
			m_removeSize = 0.0;
			//要素の削除
			m_removeFlg = true;
		}
	}

	//回転とかで毎フレーム参照されると落ちるので
	//ビームは管理側がSetRemoveしないと削除しない
	if (m_pos.x <	CGame::GetBattleRect().leftUp.x ||
		m_pos.x >	CGame::GetBattleRect().rightDown.x ||
		m_pos.y <	CGame::GetBattleRect().leftUp.y ||
		m_pos.y >	CGame::GetBattleRect().rightDown.y) {
		m_removeFlg = true;
	}

	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] != nullptr) {
			m_bullet[i]->Action();

			//カウントが設定されており
			//カウントが距離を上回ったらそこから削除する
			if (m_length > 0 && m_bullet[i]->m_count > m_length) {
				m_bullet[i]->m_removeFlg = true;
			}

			if (m_bullet[i]->m_pos.x <	CGame::GetBattleRect().leftUp.x ||
				m_bullet[i]->m_pos.x >	CGame::GetBattleRect().rightDown.x ||
				m_bullet[i]->m_pos.y <	CGame::GetBattleRect().leftUp.y ||
				m_bullet[i]->m_pos.y >	CGame::GetBattleRect().rightDown.y) {
				m_bullet[i]->m_removeFlg = true;
			}

			//削除
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

	//台座
	double siz = 1.0 * m_removeSize;
	CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, siz, 0.0, m_imageStand->m_iamge);
	CDxFunc::MyDrawRotaGraph(m_pos.x, m_pos.y, siz * 0.8, 0.0, m_imageLaser->m_iamge);

	//削除中でなければ予告線を表示する
	if (m_removeNow == false) {
		//予告線
		//デフォルト-1だから、-1でなければlengthを参照
		int len = m_bulletTotalNum;
		if (m_length != -1) {
			len = m_length;
		}
		//*2なのは間隔を狭くするため
		for (int i = 0; i < (len * 2 - 2); i++) {
			double x, y;
			x = m_pos.x + cos(m_drawAngle) * (double)i / 2 * m_speed;
			y = m_pos.y + sin(m_drawAngle) * (double)i / 2 * m_speed;
			CDxFunc::MyDrawRotaGraph(x, y, 0.12, m_drawAngle + 90.0 / CFunc::RAD, m_imageLaser->m_iamge);
		}
	}

	//子のDrawに任せず、親が描画する
	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] != nullptr) {
			double bure = CFunc::RandF(-5, 5) / CFunc::RAD;
			//削除中だとだんだん小さくする
			double siz = 1.0 * m_removeSize;
			CDxFunc::MyDrawRotaGraph(m_bullet[i]->m_pos, siz, m_drawAngle + 90.0 / CFunc::RAD + bure, m_imageLaser->m_iamge);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
}

//void CBaseBeam::setImageData(CBeamImageManager *BeamImageManager){
//	beamImageManager = BeamImageManager;
//}
//void CBaseBeam::SetResource(CResourceManager* ImageManager) {
//	m_imageManager = ImageManager;
//}



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

	//描画角度加算
	m_drawAngle += ang;
	//ビーム方向加算
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


CBaseBeamChild::CBaseBeamChild(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
	CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, image) {

}

CBaseBeamChild::~CBaseBeamChild() {

}

void CBaseBeamChild::Remove() {
	//何もしない
	//エフェクト出したくない
}

void CBaseBeamChild::RectOut() {
	//領域外に出たら削除したくない
	//削除は上位がする
	//if(	m_pos.x <	CBaseBeamChild::m_bigRect.leftUp.x		|| 
	//	m_pos.x >	CBaseBeamChild::m_bigRect.rightDown.x	|| 
	//	m_pos.y <	CBaseBeamChild::m_bigRect.leftUp.y		|| 
	//	m_pos.y >	CBaseBeamChild::m_bigRect.rightDown.y	){
	//	m_removeFlg = true;
	//}
}




