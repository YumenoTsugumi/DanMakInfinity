#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBullet.h"
#include "CustomBullet.h"
#include "BattleScene.h"
CBulletManager* CCustomBullet::m_manager = nullptr;

CCustomBullet::CCustomBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
				CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, image){
	//親のコンストラクタが呼ばれたあと（Setとかで設定）ココに来る
	//継承クラスの設定
	Set();

	//値を引き継ぐNOTRANSの場合、そのままの値（99999）で引き継ぐか判定しているため
	//コンストラクタで/RADされたら困る
	//その為、ここで上書き
	if(angle == NOTRANS){
		m_angle = angle;
	}
}
//CCustomBullet::CCustomBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName) :
//				CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, ImageName){
//	//親のコンストラクタが呼ばれたあと（Setとかで設定）ココに来る
//	//継承クラスの設定
//	Set();
//
//	//値を引き継ぐNOTRANSの場合、そのままの値（99999）で引き継ぐか判定しているため
//	//コンストラクタで/RADされたら困る
//	//その為、ここで上書き
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
	//実体化中
	if(m_standByCount < m_standByTime){
		m_standByCount++;
		return;
	}

	//移動
	Move();

	//弾変化
	Trans();
	//弾発射
	Add();

	//範囲外判定
	RectOut();
	//削除
	Remove();

	m_count++;
}

void CCustomBullet::Draw(){
	CPos subPos = CBattleScene::GetBattleScene()->GetBackGroundscrollSmall();

	if(m_standByCount < m_standByTime){
		//実体化中
		int alpha = (int)((double)255 * ((double)m_standByCount / m_standByTime ));

		SetDrawBlendMode( DX_BLENDMODE_ALPHA , alpha ) ;
		CDxFunc::MyDrawRotaGraph(m_pos + subPos, 1.0, m_angle + m_imageInfo.m_rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.m_animePos]);
	} else {
		//通常描画
		CBaseBullet::Draw();
	}
}

void CCustomBullet::SetTransBullet(CCustomBullet* Bullet, int Frame){
	m_transFrame = Frame;
	m_transBullet = Bullet;
}

void CCustomBullet::SetTransAngle(double Angle){
	if(m_transFrame > 0){
		//nullptrチェックしておく
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
		//nullptrチェックしておく
		if(m_transBullet == nullptr){
			//もしnullptrなら変化パラメータを初期化
			m_transFrame = 0;
			return;
		}
		////値の引継ぎ
		////最初は値を引き継いでaddBulletする予定だったけど、そうしたらRemoveフラグをONにして消す際に
		////エフェクトが発生する？可能性があり、めんどくさくなった。
		////だから値の上書きに
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
		////画像noが異なる場合上書き
		//if(m_transBullet->m_imageInfo.m_imageNo == m_imageInfo.m_imageNo){
		//	m_transBullet->m_imageInfo = m_imageInfo;
		//	m_transBullet->m_image = m_image;	//意味ない
		//}
		//m_transBullet = nullptr;
		//Remove = true;

		//変化弾の値を現在の弾に上書きする
		//その際初期化等々
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

		//速度再計算
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
		//画像noが異なる場合上書き
		if(m_transBullet->m_imageInfo.m_imageNo != m_imageInfo.m_imageNo){
			m_imageInfo = m_transBullet->m_imageInfo;
			m_image = m_transBullet->m_image;
		}

		m_count = 0;
		m_removeFlg = false;

		//解放用にアドレスを保存しておく
		CCustomBullet* wrk;
		wrk = m_transBullet;
		
		//追加弾
		m_addFrame = m_transBullet->m_addFrame;
		m_addFuncP = m_transBullet->m_addFuncP;
		//addBullet = m_transBullet->addBullet;
		//変化弾
		m_transFrame = m_transBullet->m_transFrame;
		m_transBullet = m_transBullet->m_transBullet;	//この処理は一番最後

		//弾をnullptrにしておかないとデストラクタで解放されちゃう
		wrk->m_transBullet = nullptr;

		//すべて上書きが完了した為、解放
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
		//nullptrチェックしておく
		if(m_addFuncP == nullptr){
			//もしnullptrなら変化パラメータを初期化
			m_addFrame = 0;
			return;
		}
		if(m_count % m_addFrame == 0){
			//関数ポインタから弾を吐く
			(m_addFuncP)(this);
		}
	}
}

void CCustomBullet::SetStandByTime(int Time){
	m_standByTime = Time;
	m_standByCount = 0;
}