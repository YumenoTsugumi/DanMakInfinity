#include "BattleScene.h"

#include "Player.h"
//#include "Inheritance.h"
//#include "Area.h"
//#include "OtherInfo.h"
#include "Game.h"
#include "BaseBullet.h"
#include "CustomBullet.h"
#include "HomingBullet.h"



//静的なのを使うにはコレがいる？
//CResourceManager* CScene::resManager;

CBattleScene::CBattleScene(int InTime) : 
	CScene(InTime),
	m_bulletManeger(2048),
	m_beamManeger(128),
	m_player()
{
	//シーン,	フェードイン時間60, フェードアウト時間60, 
	//			フェードイン濃淡0.0, フェードアウト濃淡0.0
	Set(InTime);
}

CBattleScene::~CBattleScene(){

}

void CBattleScene::Init(CGame* gameP){
	m_player.Init();

	m_game = gameP;

	//bulletManeger = new CInhBulletManager(1024);
	// 変化弾の定義に必要
	CCustomBullet::SetBulletManagerPointer(&m_bulletManeger);
	BaseLauncher::SetBulletManagerPointer(&m_bulletManeger);
	BaseLauncher::SetBeamManagerPointer(&m_beamManeger);


	//普通に発射
	m_pos = CPos(300, 300);
	CPos vel(1.0, 1.0);
	double ang = 0.0;
	//m_beam1 = new CBaseBeam(m_pos, ang, 100);
	//m_beamManeger.Add(m_beam1);

	for (int ii = 0; ii < 11; ii++) {
		CPos pos(50 + ii*70, 100);
		m_launcher.push_back(new Launcher000(ii * 10, pos));
	}
	//for (int ii = 0; ii < 10; ii++) {
	//	CPos pos(50 + ii * 70, 200);
	//	m_launcher.push_back(new Launcher001(ii * 10, pos));
	//}
	//for (int ii = 0; ii < 10; ii++) {
	//	CPos pos(50 + ii * 70, 300);
	//	m_launcher.push_back(new Launcher010(ii * 10, pos));
	//}
}

void addFuncA(CCustomBullet* m_bullet) {
	for (int i = 0; i < 5; i++) {
		m_bullet->m_manager->Add(new CBaseBullet(EDirType::Abs, m_bullet->m_pos, 10, 90 + -20 + i * 10, 0, 0.10, 10, 0,10));
	}
}
void addFuncB(CCustomBullet* m_bullet) {
	for (int i = 0; i < 10; i++) {
		m_bullet->m_manager->Add(new CBaseBullet(EDirType::Player, m_bullet->m_pos, 2.5, i * (360.0 / 10), 0, 0, 0, 0,10));
	}
}

void CBattleScene::Main(CInputAllStatus *input){
	//フェード中でなければ
	if(!NowFeed()){
		//入力参照
		if(input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH){
			SetFeedOut(120);
			SetBackScene();
		}
	}


	static int count = 0;
	count++;

#if 0
	//移動回転
	CPos ppp(300, 300);
	CPos vel(1.0, 1.0);
	m_pos += vel;
	if (m_beam1) {
		m_beam1->Move(m_pos, 1.0);
	}

	if (count > 0 && count <= 100) {
		if (count % 10 == 0) {
			CPos p(CFunc::RandF(0, 800), CFunc::RandF(0, 600));
			CBaseBeam* m_beam = new CBaseBeam(p, CFunc::RandF(0,360), 100);

			//長さ設定 1～64まで	ピクセルで言うと*10まで伸びる
			m_beam->SetLength(100);
			//照射までの待機時間　fr単位
			m_beam->SetWait(60);
			//削除時間
			m_beam->SetRemoveTime(300);

			m_beamManeger.Add(m_beam);
		}
	}
	else if (count > 300) {
		if (count % 10 == 0) {
			CPos pp(CFunc::RandF(400, 400), CFunc::RandF(100, 100));
			for (int i = 0; i < 6; i++) {
				CHomingLaser* hl = new CHomingLaser(EDirType::Player,		//射出方向
					pp,			//座標
					20,			//レーザー長さ
					12,			//速度
					i * 60.0,		//射出角度
					0, 0, 0, 0, 		//加速は有効
					"hLaser02");//画像
				hl->AddLimitInfo(30, 5.0);						//30frまでは角度制限5.0
				hl->AddLimitInfo(60, 3.0);						//60frまでは角度制限3.0
				hl->AddLimitInfo(MAXINT, 0.5);					//以降は角度制限0.5
				m_bulletManeger.Add(hl);
			}
		}

	}
#if 0
	if (count > 0 && count <= 100) {
		if (count % 10 == 0) {
			static int cr = 0;
			cr++;
			CBaseBullet* bu = new CBaseBullet(EDirType::Abs, CPos(400, 50), 5.0, 90.0, 0, 0, 0, cr%3);
			m_bulletManeger.Add(bu);
		}
	} else if (count > 100 && count <= 200) {
		if (count % 10 == 0) {
			static int cr = 0;
			cr++;
			CBaseBullet* bu = new CBaseBullet(EDirType::Player, CPos(400, 50), 5.0, 0.0, 0, 0, 0, cr % 3);
			m_bulletManeger.Add(bu);
		}
	}
	else if (count > 200 && count <= 300) {
		if (count % 10 == 0) {
			static int cr = 0;
			cr++;
			CBaseBullet* bu = new CBaseBullet(EDirType::Abs, CPos(400, 50), 5.0, 90.0, 0.3, 0, 0, 10 + cr % 3);
			m_bulletManeger.Add(bu);
		}
	}
	else if (count > 300 && count <= 400) {
		if (count % 10 == 0) {
			static int cr = 0;
			cr++;
			CBaseBullet* bu = new CBaseBullet(EDirType::Abs, CPos(400, 50), 1.0, 90+-30.0, 0.0, 0.15, 20, 10 + cr % 3);
			CBaseBullet* bu2 = new CBaseBullet(EDirType::Abs, CPos(400, 50), 1.0, 90 + 30.0, 0.0, 0.15, 20, 10 + cr % 3);
			m_bulletManeger.Add(bu);
			m_bulletManeger.Add(bu2);
		}
	}
	else if (count > 400 && count <= 500) {
		if (count % 10 == 0) {
			CCustomBullet* m_bullet = new CCustomBullet(EDirType::Abs, CPos(320, 240), 4, 90, 0, 0, 0, 0);
			CCustomBullet* Trans = new CCustomBullet(EDirType::Player, CPos(0, 0), 0, 0, 0, 0.25, 10, 1);
			CCustomBullet* trans2 = new CCustomBullet(EDirType::Abs, CPos(0, 0), NOTRANS, CFunc::RandF(0, 360), 0, 0, 0, 2);

			//m_bulletが30fr後にTransになる
			m_bullet->SetTransBullet(Trans, 60);

			//EDirType::Player狙いの時だけ、変化後の角度を再設定してあげないといけない
			Trans->SetTransAngle(0);

			//Transが20fr後にtrans2になる
			Trans->SetTransBullet(trans2, 60);

			//上記を設定後m_bulletを発射
			m_bulletManeger.Add(m_bullet);
		}
	}
	else if (count > 500 && count <= 600) {
		if (count % 10 == 0) {
			//適当な関数

			CCustomBullet* ppp = new CCustomBullet(EDirType::Abs, CPos(320, 240), 0.0, CFunc::RandF(180, 360), 0, 0.02, 10, 0);
			//20fr毎にaddFuncAを実行する
			ppp->SetAddBullet(addFuncA, 20);

			//弾を追加する
			m_bulletManeger.Add(ppp);
		}
	}
#endif
	//int bulletA[] = {00,01,02, 10,11,12, 20,21,22,23,24,25, 30,31,32, 40,41,42, 50,51,52};

	//CPos ppp(m_count % 100,m_count % 100);
#endif

	////どんな状態でもアクションする処理
	m_player.Action(input);
	m_bulletManeger.Action();
	m_beamManeger.Action();
	CBaseBullet::SetTarget(m_player.m_pos);
	//area->Action();

	for (auto* launcher : m_launcher) {
		launcher->Action(launcher->m_pos);
	}

	//area->Draw();
	m_player.Draw();
	m_bulletManeger.Draw();
	m_beamManeger.Draw();

	//m_count++;

}
