#include "DxLib.h"
#include "DXFunc.h"
#include "BaseBullet.h"
#include "CustomBullet.h"

CBulletManager* CCustomBullet::m_manager = nullptr;

CCustomBullet::CCustomBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG) : 
				CBaseBullet(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, IMG){
	//親のコンストラクタが呼ばれたあと（Setとかで設定）ココに来る
	//継承クラスの設定
	Set();

	//値を引き継ぐNOTRANSの場合、そのままの値（99999）で引き継ぐか判定しているため
	//コンストラクタで/RADされたら困る
	//その為、ここで上書き
	if(ANGLE == NOTRANS){
		m_angle = ANGLE;
	}
}
CCustomBullet::CCustomBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME) : 
				CBaseBullet(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, IMAGENAME){
	//親のコンストラクタが呼ばれたあと（Setとかで設定）ココに来る
	//継承クラスの設定
	Set();

	//値を引き継ぐNOTRANSの場合、そのままの値（99999）で引き継ぐか判定しているため
	//コンストラクタで/RADされたら困る
	//その為、ここで上書き
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
	if(m_standByCount < m_standByTime){
		//実体化中
		int alpha = (int)((double)255 * ((double)m_standByCount / m_standByTime ));

		SetDrawBlendMode( DX_BLENDMODE_ALPHA , alpha ) ;
		CDxFunc::DrawRotaGraph(m_pos.x, m_pos.y, 1.0, m_angle + m_imageInfo.rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.animePos]);
	} else {
		//通常描画
		CBaseBullet::Draw();
	}
}

void CCustomBullet::SetTransBullet(CCustomBullet* Bullet, int Frame){
	m_transFrame = Frame;
	transBullet = Bullet;
}

void CCustomBullet::SetTransAngle(double Angle){
	if(m_transFrame > 0){
		//nullptrチェックしておく
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
		//nullptrチェックしておく
		if(transBullet == nullptr){
			//もしnullptrなら変化パラメータを初期化
			m_transFrame = 0;
			return;
		}
		////値の引継ぎ
		////最初は値を引き継いでaddBulletする予定だったけど、そうしたらRemoveフラグをONにして消す際に
		////エフェクトが発生する？可能性があり、めんどくさくなった。
		////だから値の上書きに
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
		////画像noが異なる場合上書き
		//if(transBullet->m_imageInfo.m_imageNo == m_imageInfo.m_imageNo){
		//	transBullet->m_imageInfo = m_imageInfo;
		//	transBullet->m_image = m_image;	//意味ない
		//}
		//transBullet = nullptr;
		//Remove = true;

		//変化弾の値を現在の弾に上書きする
		//その際初期化等々
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

		//速度再計算
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
		//画像noが異なる場合上書き
		if(transBullet->m_imageInfo.m_imageNo != m_imageInfo.m_imageNo){
			m_imageInfo = transBullet->m_imageInfo;
			m_image = transBullet->m_image;
		}

		m_count = 0;
		m_removeFlg = false;

		//解放用にアドレスを保存しておく
		CCustomBullet* wrk;
		wrk = transBullet;
		
		//追加弾
		m_addFrame = transBullet->m_addFrame;
		addFuncP = transBullet->addFuncP;
		//addBullet = transBullet->addBullet;
		//変化弾
		m_transFrame = transBullet->m_transFrame;
		transBullet = transBullet->transBullet;	//この処理は一番最後

		//弾をnullptrにしておかないとデストラクタで解放されちゃう
		wrk->transBullet = nullptr;

		//すべて上書きが完了した為、解放
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
		//nullptrチェックしておく
		if(addFuncP == nullptr){
			//もしnullptrなら変化パラメータを初期化
			m_addFrame = 0;
			return;
		}
		if(m_count % m_addFrame == 0){
			//関数ポインタから弾を吐く
			(addFuncP)(this);
		}
	}
}

void CCustomBullet::SetStandByTime(int Time){
	m_standByTime = Time;
	m_standByCount = 0;
}