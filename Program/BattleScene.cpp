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

//�ÓI�Ȃ̂��g���ɂ̓R��������H
//CResourceManager* CScene::resManager;

CEffectManager CBattleScene::m_effectManager;
CItemManager CBattleScene::m_itemManager;
CBattleScene::BulletRemoveType CBattleScene::m_bulletRemoveType;
int CBattleScene::m_bulletRemoveTime;
int CBattleScene::m_bulletRemoveCount;

CEnemyManager CBattleScene::m_enemyManager; // �G
CBulletManager CBattleScene::m_bulletManager; // �G�̒e
CBeamManager CBattleScene::m_beamManager; // �G�̃r�[��

// �X�R�A
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
	//�V�[��,	�t�F�[�h�C������60, �t�F�[�h�A�E�g����60, 
	//			�t�F�[�h�C���Z�W0.0, �t�F�[�h�A�E�g�Z�W0.0
	Set(InTime);

	

	testLauncherPos = CPos(0, 200);
	testLauncher = new CLauncher999(0, testLauncherPos, CPos(0,0));

}

CBattleScene::~CBattleScene(){

}

void CBattleScene::Init(CGame* gameP) {
	m_player.Init();
	m_player.SetBulletManager(&m_playerBullet); // �v���C���[�Ƀv���C���e�Ǘ���ݒ�

	m_hiScore = 0;
	m_score = 0;
	m_rank = 1;
	m_bulletRemoveTime = m_bulletRemoveCount = 0;

	m_game = gameP;

	// �ω��e�̒�`�ɕK�v
	CCustomBullet::SetBulletManagerPointer(&m_bulletManager);
	CBaseLauncher::SetBulletManagerPointer(&m_bulletManager);
	CBaseLauncher::SetBeamManagerPointer(&m_beamManager);




	// 001�G
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
	// 020��
	//{
	//	CEnemy200* e2 = new CEnemy200(CPos(900, 200));

	//	std::vector<CPos> poss = {
	//	CPos(900, 50), CPos(600, 150), CPos(300, 150), CPos(-200, 50),
	//	};

	//	CBezierBehavior* move = new CBezierBehavior(poss, 1.3);
	//	e2->SetBehaviorComponent(move);

	//	m_enemyManager.Add(e2);
	//}

	// �f�o�b�O�p�̑S�G�\��
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
	////�t�F�[�h���łȂ����
	//if(!NowFeed()){
	//	//���͎Q��
	//	if(input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH){
	//		SetFeedOut(120);
	//		SetBackScene();
	//	}
	//}
	static int tekitouCount = 0;
	tekitouCount++;

#if 0
	if (tekitouCount % 120 == 0) {
		// 000�G
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

		// 002�G
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
	//�ړ���]
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

			//�����ݒ� 1�`64�܂�	�s�N�Z���Ō�����*10�܂ŐL�т�
			m_beam->SetLength(100);
			//�Ǝ˂܂ł̑ҋ@���ԁ@fr�P��
			m_beam->SetWait(60);
			//�폜����
			m_beam->SetRemoveTime(300);

			m_beamManager.Add(m_beam);
		}
	}
	else if (count > 300) {
		if (count % 10 == 0) {
			CPos pp(CFunc::RandF(400, 400), CFunc::RandF(100, 100));
			for (int i = 0; i < 6; i++) {
				CHomingLaser* hl = new CHomingLaser(EDirType::Player,		//�ˏo����
					pp,			//���W
					20,			//���[�U�[����
					12,			//���x
					i * 60.0,		//�ˏo�p�x
					0, 0, 0, 0, 		//�����͗L��
					"hLaser02");//�摜
				hl->AddLimitInfo(30, 5.0);						//30fr�܂ł͊p�x����5.0
				hl->AddLimitInfo(60, 3.0);						//60fr�܂ł͊p�x����3.0
				hl->AddLimitInfo(MAXINT, 0.5);					//�ȍ~�͊p�x����0.5
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

			//m_bullet��30fr���Trans�ɂȂ�
			m_bullet->SetTransBullet(Trans, 60);

			//EDirType::Player�_���̎������A�ω���̊p�x���Đݒ肵�Ă����Ȃ��Ƃ����Ȃ�
			Trans->SetTransAngle(0);

			//Trans��20fr���trans2�ɂȂ�
			Trans->SetTransBullet(trans2, 60);

			//��L��ݒ��m_bullet�𔭎�
			m_bulletManager.Add(m_bullet);
		}
	}
	else if (count > 500 && count <= 600) {
		if (count % 10 == 0) {
			//�K���Ȋ֐�

			CCustomBullet* ppp = new CCustomBullet(EDirType::Abs, CPos(320, 240), 0.0, CFunc::RandF(180, 360), 0, 0.02, 10, 0);
			//20fr����addFuncA�����s����
			ppp->SetAddBullet(addFuncA, 20);

			//�e��ǉ�����
			m_bulletManager.Add(ppp);
		}
	}
#endif
	//int bulletA[] = {00,01,02, 10,11,12, 20,21,22,23,24,25, 30,31,32, 40,41,42, 50,51,52};

	//CPos ppp(m_count % 100,m_count % 100);
#endif


	//testLauncher->Action(testLauncherPos);

	////�ǂ�ȏ�Ԃł��A�N�V�������鏈��
	m_stageManager.Main(); //�G�X�|�[��

	m_bg.Action();
	m_player.Action(input);
	RemoveBullet(); // �e��������
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
	m_effectManager.Draw(0); // 0 ��ԃ{�g��

	m_enemyManager.Draw();
	m_effectManager.Draw(10); // 10	�G��肠��
	m_player.Draw();
	
	m_effectManager.Draw(20); // 20 �v���C���[����
	m_playerBullet.Draw();
	m_itemManager.Draw();
	m_effectManager.Draw(30); // 30 �v���C���[�̒e����
	m_bulletManager.Draw();
	m_effectManager.Draw(40);
	m_beamManager.Draw();
	m_effectManager.Draw(50); // 50 �őO��

	m_ui.Draw();

	// �f�o�b�O�R�}���h
	DebugCommand();
}

// �����NUP
void CBattleScene::AddRank(int delta)
{
	m_rank += delta;
	if (m_rank < 1)m_rank = 1;
	if (m_rank > 999)m_rank = 999;
}

void CBattleScene::SetBulletRemoveTime(BulletRemoveType type, int time)
{
	m_bulletRemoveType = type;
	m_bulletRemoveTime = time; // 1�Ȃ�1fr�e��������
	m_bulletRemoveCount = 0;
}
// �e��������
void CBattleScene::RemoveBullet() 
{
	if (m_bulletRemoveCount >= m_bulletRemoveTime) {
		return; // �e�����I��
	}
	if(m_bulletRemoveCount)
	for (int ii = 0; ii < m_bulletManager.m_bulletTotalNum; ii++) {
		if (m_bulletManager.m_bullet[ii] == nullptr) {
			continue;
		}
		CBaseBullet* bullet = m_bulletManager.m_bullet[ii];
		bullet->SetRemove();

		// �폜���A�C�e���Ȃ�
		if (m_bulletRemoveType == BulletRemoveType::Item) {
			// �A�C�e��
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


// �S�G�\��
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

// �f�o�b�O�R�}���h
void CBattleScene::DebugCommand()
{
	if (!m_enableDebugCommand)return;

	// �����NDOWN
	static int qc = 0;
	if (CheckHitKey(KEY_INPUT_Q) != 0) {
		if (qc++ % 10 == 0) {
			AddRank(-1);
		};
	}

	// �����NUP
	if (CheckHitKey(KEY_INPUT_W) != 0) {
		if (qc++ % 10 == 0) {
			AddRank(1);
		};
	}

	// �����NDOWN
	if (CheckHitKey(KEY_INPUT_A) != 0) {
		if (qc++ % 10 == 0) {
			AddRank(-10);
		};
	}

	// �����NUP
	if (CheckHitKey(KEY_INPUT_S) != 0) {
		if (qc++ % 10 == 0) {
			AddRank(10);
		};
	}
}