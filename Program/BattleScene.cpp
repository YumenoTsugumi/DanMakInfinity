#include "BattleScene.h"

#include "Player.h"
//#include "Inheritance.h"
//#include "Area.h"
//#include "OtherInfo.h"
#include "Game.h"
#include "BaseBullet.h"
#include "CustomBullet.h"
#include "HomingBullet.h"

#include "EnemySmall.h"
#include "EnemyMedium.h"
#include "EnemyLarge.h"

bool CBattleScene::m_enemyHitSizeDraw = false;
bool CBattleScene::m_enemyLauncherDraw = false;
bool CBattleScene::m_enableDebugCommand = true;

//静的なのを使うにはコレがいる？
//CResourceManager* CScene::resManager;

CEffectManager CBattleScene::m_effectManager;
CItemManager CBattleScene::m_itemManager;
CBattleScene::BulletRemoveType CBattleScene::m_bulletRemoveType;
int CBattleScene::m_bulletRemoveTime;
int CBattleScene::m_bulletRemoveCount;

CEnemyManager CBattleScene::m_enemyManager; // 敵
CBulletManager CBattleScene::m_bulletManager; // 敵の弾
CBeamManager CBattleScene::m_beamManager; // 敵のビーム

// スコア
long long CBattleScene::m_hiScore;
long long CBattleScene::m_score;
int CBattleScene::m_rank;

CBattleScene::CBattleScene(int InTime) :
	CScene(InTime),
	m_playerBullet(256),
	m_player(),
	m_bg(),
	testLauncher(nullptr),
	m_ui()
{
	//シーン,	フェードイン時間60, フェードアウト時間60, 
	//			フェードイン濃淡0.0, フェードアウト濃淡0.0
	Set(InTime);

	

	testLauncherPos = CPos(0, 200);
	testLauncher = new CLauncher999(0, testLauncherPos, CPos(0,0));

}

CBattleScene::~CBattleScene(){

}

void CBattleScene::Init(CGame* gameP) {
	m_player.Init();
	m_player.SetBulletManager(&m_playerBullet); // プレイヤーにプレイヤ弾管理を設定

	m_hiScore = 0;
	m_score = 0;
	m_rank = 1;
	m_bulletRemoveTime = m_bulletRemoveCount = 0;

	m_game = gameP;

	// 変化弾の定義に必要
	CCustomBullet::SetBulletManagerPointer(&m_bulletManager);
	CBaseLauncher::SetBulletManagerPointer(&m_bulletManager);
	CBaseLauncher::SetBeamManagerPointer(&m_beamManager);




	// 001敵
	//for(int ii=0;ii<120;ii++)
	//{
	//	CEnemy001* e2 = new CEnemy001(CPos(-100, -100));

	//	std::vector<CPos> poss = {
	//	CPos(-100, -100), CPos(100, 100), CPos(200, 100), CPos(300, 100),
	//	CPos(300, 100), CPos(600, 100), CPos(600, 200), CPos(300, 200),
	//	CPos(300, 200), CPos(100, 200), CPos(0, 200), CPos(-100, 200), };

	//	CBezierBehavior* move = new CBezierBehavior(poss, 5.0);
	//	e2->SetBehaviorComponent(move, ii * 30);

	//	m_enemyManager.Add(e2);
	//}
	// 020蛾
	//{
	//	CEnemy200* e2 = new CEnemy200(CPos(900, 200));

	//	std::vector<CPos> poss = {
	//	CPos(900, 50), CPos(600, 150), CPos(300, 150), CPos(-200, 50),
	//	};

	//	CBezierBehavior* move = new CBezierBehavior(poss, 1.3);
	//	e2->SetBehaviorComponent(move);

	//	m_enemyManager.Add(e2);
	//}

	// デバッグ用の全敵表示
	//DebugAllEnemyDirection();

	m_bg.SetInitPlayerPos(m_player.m_pos);

	m_ui.Init();
}

void addFuncA(CCustomBullet* m_bullet) {
	for (int i = 0; i < 5; i++) {
		m_bullet->m_manager->Add(new CBaseBullet(EDirType::Abs, m_bullet->m_pos, 10.0, 90.0 + -20.0 + i * 10.0, 0, 0.10, 10, 0,10));
	}
}
void addFuncB(CCustomBullet* m_bullet) {
	for (int i = 0; i < 10; i++) {
		m_bullet->m_manager->Add(new CBaseBullet(EDirType::Player, m_bullet->m_pos, 2.5, i * (360.0 / 10), 0, 0, 0, 0,10));
	}
}

void CBattleScene::Main(CInputAllStatus *input){
	////フェード中でなければ
	//if(!NowFeed()){
	//	//入力参照
	//	if(input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH){
	//		SetFeedOut(120);
	//		SetBackScene();
	//	}
	//}
	static int tekitouCount = 0;
	tekitouCount++;

#if 0
	if (tekitouCount % 120 == 0) {
		// 000敵
		for (int ii = 0; ii < 1; ii++)
		{
			double x = CFunc::RandF(50, 750);
			double y = CFunc::RandF(-100, -10);
			CPos pos1(x, y);
			CEnemyS001* e1 = new CEnemyS001(pos1);

			CInOutBehavior* move = new CInOutBehavior(pos1, pos1 + CPos(0, 250), 7, 10, 180);
			e1->SetBehaviorComponent(move, CFunc::RandI(0, 60));

			m_enemyManager.Add(e1);
		}

		// 002敵
		for (int ii = 0; ii < 3; ii++)
		{
			double x = CFunc::RandF(50, 750);
			double y = CFunc::RandF(-100, -10);
			CPos pos1(x, y);
			CEnemyS002* e1 = new CEnemyS002(pos1);

			CInOutBehavior* move = new CInOutBehavior(pos1, pos1 + CPos(0, 3000), 7, 10, 1800);
			e1->SetBehaviorComponent(move, CFunc::RandI(0, 60));

			m_enemyManager.Add(e1);
		}
	}
	if (tekitouCount % 120 == 0) {
		double x = CFunc::RandF(50, 750);
		double y = CFunc::RandF(-100, -10);
		CPos pos1(x, y);
		CEnemyS003* e1 = new CEnemyS003(pos1);

		CInOutBehavior* move = new CInOutBehavior(pos1, pos1 + CPos(0, 3000), 1, 1, 180);
		e1->SetBehaviorComponent(move);

		m_enemyManager.Add(e1);
	}
#endif

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
			CPos p(CFunc::RandF(0, WindowX), CFunc::RandF(0, WindowY));
			CBaseBeam* m_beam = new CBaseBeam(p, CFunc::RandF(0,360), 100);

			//長さ設定 1～64まで	ピクセルで言うと*10まで伸びる
			m_beam->SetLength(100);
			//照射までの待機時間　fr単位
			m_beam->SetWait(60);
			//削除時間
			m_beam->SetRemoveTime(300);

			m_beamManager.Add(m_beam);
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
				m_bulletManager.Add(hl);
			}
		}

	}
#if 0
	if (count > 0 && count <= 100) {
		if (count % 10 == 0) {
			static int cr = 0;
			cr++;
			CBaseBullet* bu = new CBaseBullet(EDirType::Abs, CPos(400, 50), 5.0, 90.0, 0, 0, 0, cr%3);
			m_bulletManager.Add(bu);
		}
	} else if (count > 100 && count <= 200) {
		if (count % 10 == 0) {
			static int cr = 0;
			cr++;
			CBaseBullet* bu = new CBaseBullet(EDirType::Player, CPos(400, 50), 5.0, 0.0, 0, 0, 0, cr % 3);
			m_bulletManager.Add(bu);
		}
	}
	else if (count > 200 && count <= 300) {
		if (count % 10 == 0) {
			static int cr = 0;
			cr++;
			CBaseBullet* bu = new CBaseBullet(EDirType::Abs, CPos(400, 50), 5.0, 90.0, 0.3, 0, 0, 10 + cr % 3);
			m_bulletManager.Add(bu);
		}
	}
	else if (count > 300 && count <= 400) {
		if (count % 10 == 0) {
			static int cr = 0;
			cr++;
			CBaseBullet* bu = new CBaseBullet(EDirType::Abs, CPos(400, 50), 1.0, 90+-30.0, 0.0, 0.15, 20, 10 + cr % 3);
			CBaseBullet* bu2 = new CBaseBullet(EDirType::Abs, CPos(400, 50), 1.0, 90 + 30.0, 0.0, 0.15, 20, 10 + cr % 3);
			m_bulletManager.Add(bu);
			m_bulletManager.Add(bu2);
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
			m_bulletManager.Add(m_bullet);
		}
	}
	else if (count > 500 && count <= 600) {
		if (count % 10 == 0) {
			//適当な関数

			CCustomBullet* ppp = new CCustomBullet(EDirType::Abs, CPos(320, 240), 0.0, CFunc::RandF(180, 360), 0, 0.02, 10, 0);
			//20fr毎にaddFuncAを実行する
			ppp->SetAddBullet(addFuncA, 20);

			//弾を追加する
			m_bulletManager.Add(ppp);
		}
	}
#endif
	//int bulletA[] = {00,01,02, 10,11,12, 20,21,22,23,24,25, 30,31,32, 40,41,42, 50,51,52};

	//CPos ppp(m_count % 100,m_count % 100);
#endif


	//testLauncher->Action(testLauncherPos);

	////どんな状態でもアクションする処理
	m_stageManager.Main(); //敵スポーン

	m_bg.Action();
	m_player.Action(input);
	RemoveBullet(); // 弾消し処理
	m_playerBullet.Action();
	m_enemyManager.Action();
	m_itemManager.Action();
	m_bulletManager.Action();
	m_beamManager.Action();
	m_bg.SetPlayerMovedPos(m_player.m_pos);
	CBaseBullet::SetTarget(m_player.m_pos);
	CBaseEnemy::SetTarget(m_player.m_pos);
	CBaseLauncher::SetTarget(m_player.m_pos);
	m_effectManager.Action();



	Collision_Enemy_PulyerBullet();
	Collision_Item_Player();

	m_bg.Draw();
	m_effectManager.Draw(0); // 0 一番ボトム

	m_enemyManager.Draw();
	m_effectManager.Draw(10); // 10	敵よりあと
	m_player.Draw();
	
	m_effectManager.Draw(20); // 20 プレイヤーより後
	m_playerBullet.Draw();
	m_itemManager.Draw();
	m_effectManager.Draw(30); // 30 プレイヤーの弾より後
	m_bulletManager.Draw();
	m_effectManager.Draw(40);
	m_beamManager.Draw();
	m_effectManager.Draw(50); // 50 最前面

	m_ui.Draw();

	// デバッグコマンド
	DebugCommand();
}

// ランクUP
void CBattleScene::AddRank(int delta)
{
	m_rank += delta;
	if (m_rank < 1)m_rank = 1;
	if (m_rank > 999)m_rank = 999;
}

void CBattleScene::SetBulletRemoveTime(BulletRemoveType type, int time)
{
	m_bulletRemoveType = type;
	m_bulletRemoveTime = time; // 1なら1fr弾けしする
	m_bulletRemoveCount = 0;
}
// 弾消し処理
void CBattleScene::RemoveBullet() 
{
	if (m_bulletRemoveCount >= m_bulletRemoveTime) {
		return; // 弾消し終了
	}
	if(m_bulletRemoveCount)
	for (int ii = 0; ii < m_bulletManager.m_bulletTotalNum; ii++) {
		if (m_bulletManager.m_bullet[ii] == nullptr) {
			continue;
		}
		CBaseBullet* bullet = m_bulletManager.m_bullet[ii];
		bullet->SetRemove();

		// 削除時アイテムなら
		if (m_bulletRemoveType == BulletRemoveType::Item) {
			// アイテム
			int itemImage[6] = { 20700 ,20701 ,20702 ,20703 ,20704 ,20705 };

			double ang = 270.0;
			double speed = 1.0 + CFunc::RandF(1000, 3000) / 1000.0;
			CBaseItem* eff = new CBaseItem(EDirType::Abs, bullet->m_pos, speed, ang, 0, 0, -0.1, 0, itemImage[CFunc::RandI(0, 5)]);
			eff->SetSize(0.0, +0.033, 0.125);
			CBattleScene::m_itemManager.Add(eff);

		}
	}

	m_bulletRemoveCount++;
}


// 全敵表示
void CBattleScene::DebugAllEnemyDirection()
{
	static int count = 0;
	if (count == 0) {
		for (int ii = 0; ii < 3; ii++) {
			CEnemyS007* e1 = new CEnemyS007(CPos(900, 200));

			std::vector<CPos> poss = {
				CPos(100, -100), CPos(300, 500), CPos(500, 500), CPos(700, -100),
			};

			CBezierBehavior* move = new CBezierBehavior(poss, 3.0);
			e1->SetBehaviorComponent(move, ii*120);
			e1->SetDrawSize(1.0 + ii*2);
			m_enemyManager.Add(e1);
		}

	}
	count++;
}

// デバッグコマンド
void CBattleScene::DebugCommand()
{
	if (!m_enableDebugCommand)return;

	// ランクDOWN
	static int qc = 0;
	if (CheckHitKey(KEY_INPUT_Q) != 0) {
		if (qc++ % 10 == 0) {
			AddRank(-1);
		};
	}

	// ランクUP
	if (CheckHitKey(KEY_INPUT_W) != 0) {
		if (qc++ % 10 == 0) {
			AddRank(1);
		};
	}

	// ランクDOWN
	if (CheckHitKey(KEY_INPUT_A) != 0) {
		if (qc++ % 10 == 0) {
			AddRank(-10);
		};
	}

	// ランクUP
	if (CheckHitKey(KEY_INPUT_S) != 0) {
		if (qc++ % 10 == 0) {
			AddRank(10);
		};
	}
}