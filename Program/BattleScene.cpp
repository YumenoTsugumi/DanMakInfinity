#include "BattleScene.h"

#include "Player.h"
//#include "Inheritance.h"
//#include "Area.h"
//#include "OtherInfo.h"
#include "Game.h"
#include "BaseBullet.h"
#include "CustomBullet.h"
#include "HomingBullet.h"

//#include "EnemySmall.h"
//#include "EnemyMedium.h"
//#include "EnemyLarge.h"

bool CBattleScene::m_enemyHitSizeDraw = false;
bool CBattleScene::m_enemyLauncherDraw = false;
bool CBattleScene::m_enableDebugCommand = true;
CPos CBattleScene::m_playerPos;


CBattleScene* CBattleScene::m_scene = nullptr;

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
long long CBattleScene::m_hiScore = 0;
long long CBattleScene::m_score = 0;
int CBattleScene::m_rank = 0;

// �擾�A�C�e�����v��
int CBattleScene::m_takeItemRankCount[3] = { 0,0,0 }; // ���ꂼ��̃����N��

CBattleScene::CBattleScene(int InTime) :
	CScene(InTime),
	m_playerBullet(256),
	m_player(),
	m_bg(),
	//testLauncher(nullptr),
	m_ui()
{
	//�V�[��,	�t�F�[�h�C������60, �t�F�[�h�A�E�g����60, 
	//			�t�F�[�h�C���Z�W0.0, �t�F�[�h�A�E�g�Z�W0.0
	Set(InTime);
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

	SetBattleScene(this);
	m_stageManager.SetBattleScene(this);

	// �f�o�b�O�p�̑S�G�\��
	//DebugAllEnemyDirection();
	m_initPlayerPos = m_player.m_pos;

	m_bg.SetInitPlayerPos(m_player.m_pos);

	m_ui.Init();
	m_battleResultUI.Init();
	m_battleResultUIReset = true;
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
	static int count = 0;
	count++;

	////�ǂ�ȏ�Ԃł��A�N�V�������鏈��
	m_stageManager.Main(); //�G�X�|�[��

	m_bg.Action();
	m_player.Action(input);
	RemoveBullet(); // �e��������

	m_bg.SetPlayerMovedPos(m_player.m_pos);
	CBaseBullet::SetTarget(m_player.m_pos - GetBackGroundscrollSmall());
	CBaseEnemy::SetTarget(m_player.m_pos - GetBackGroundscrollSmall());
	CBaseLauncher::SetTarget(m_player.m_pos - GetBackGroundscrollSmall());
	m_bg.SetBattleScene(this);
	SetPlayerPos(m_player.m_pos);

	m_playerBullet.Action();
	m_enemyManager.Action();
	m_itemManager.Action();
	m_bulletManager.Action();
	m_beamManager.Action();
	m_effectManager.Action();



	Collision_Enemy_PulyerBullet();
	Collision_Item_Player();

	m_bg.Draw();
	m_effectManager.Draw(0); // 0 ��ԃ{�g��

	m_enemyManager.Draw();
	m_effectManager.Draw(10); // 10	�G��肠��

	m_playerBullet.Draw();
	m_player.Draw();
	
	m_effectManager.Draw(20); // 20 �v���C���[����
	
	m_itemManager.Draw();
	m_effectManager.Draw(30); // 30 �v���C���[�̒e����
	m_bulletManager.Draw();
	m_effectManager.Draw(40);
	m_beamManager.Draw();
	m_effectManager.Draw(50); // 50 �őO��

	m_ui.Draw();

	StageManager::StageManageStatus status = m_stageManager.GetStatus();
	if (status == StageManager::StageManageStatus::ResultDrawWait) {
		if (m_battleResultUIReset) {
			m_battleResultUI.Set(9999, 8765, 0, 3, 2, 5555, 12345678, 10);
			m_battleResultUIReset = false;
		}
		m_battleResultUI.Draw();
	}

	m_effectManager.Draw(60); // 60 UI


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

// �X�R�A�ǉ�
void CBattleScene::AddScore(int addScore)
{
	m_score += addScore;
}

// �A�C�e���Q�b�g
void CBattleScene::AddItem(int itemRank) // itemRank 1 2 3
{
	if (itemRank <= 0 || itemRank >= 4) {
		assert(0);
		return;
	}
	m_takeItemRankCount[itemRank-1]++;
	int addScore = (itemRank) * (m_rank* m_rank);
	AddScore(addScore);
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

	for (int ii = 0; ii < m_bulletManager.m_bulletTotalNum; ii++) {
		if (m_bulletManager.m_bullet[ii] == nullptr) {
			continue;
		}
		CBaseBullet* bullet = m_bulletManager.m_bullet[ii];
		bullet->SetRemove();

		// �폜���A�C�e���Ȃ�
		if (m_bulletRemoveType == BulletRemoveType::Item) {
			double ang = CFunc::RandI(270-2, 270+2);
			double speed = 1.0 + CFunc::RandF(100, 300) / 100.0;
			CPos addPos = CPos(CFunc::RandI(-50, 50), CFunc::RandI(-50, 50));
			CBaseItem* eff = new CBaseItem(EDirType::Abs, bullet->m_pos, speed, ang, 0, 0, -0.1, 0, 20720);
			eff->SetSize(1.0, 0, 1.0);
			CBattleScene::m_itemManager.Add(eff);
		}
	}

	m_bulletRemoveCount++;
}
#include "BaseEffect.h"
void CBattleScene::RemoveBulletByMidiumEnemy(int id) // �e��������
{
	for (int ii = 0; ii < m_bulletManager.m_bulletTotalNum; ii++) {
		if (m_bulletManager.m_bullet[ii] == nullptr) {
			continue;
		}
		CBaseBullet* bullet = m_bulletManager.m_bullet[ii];
		if (bullet->m_shotEnemyId != id) {
			continue;
		}
		bullet->SetRemove();

		double ang = CFunc::RandI(270 - 2, 270 + 2);
		double speed = 1.0 + CFunc::RandF(100, 300) / 100.0;
		CPos addPos = CPos(CFunc::RandI(-50, 50), CFunc::RandI(-50, 50));
		CBaseItem* eff = new CBaseItem(EDirType::Abs, bullet->m_pos, speed, ang, 0, 0, -0.1, 0, 20720);
		eff->SetSize(1.0, 0, 1.0);
		CBattleScene::m_itemManager.Add(eff);
		
	}
}

// �S�G�j��
void CBattleScene::DestoryAllEnemyNothingItemDrop() {
	m_enemyManager.DestoryAllEnemyNothingItemDrop();
}


// �S�G�\��
void CBattleScene::DebugAllEnemyDirection()
{
	//static int count = 0;
	//if (count == 0) {
	//	for (int ii = 0; ii < 3; ii++) {
	//		CEnemyS007* e1 = new CEnemyS007(CPos(900, 200));

	//		std::vector<CPos> poss = {
	//			CPos(100, -100), CPos(300, 500), CPos(500, 500), CPos(700, -100),
	//		};

	//		CBezierBehavior* move = new CBezierBehavior(poss, 3.0);
	//		e1->SetBehaviorComponent(move, ii*120);
	//		e1->SetDrawSize(1.0 + ii*2);
	//		m_enemyManager.Add(e1);
	//	}

	//}
	//count++;
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