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


int CBattleScene::m_spawneSmallCount = 0;
int CBattleScene::m_spawneMediumCount = 0;
int CBattleScene::m_spawneLargeCount = 0;
int CBattleScene::m_spawneDropItemCount = 0;
int CBattleScene::m_destroySpawneSmallCount = 0;
int CBattleScene::m_destroySpawneMediumCount = 0;
int CBattleScene::m_destroySpawneLargeCount = 0;
int CBattleScene::m_getSpawneDropItemCount = 0;
int CBattleScene::m_usedBomb = 0;
int CBattleScene::m_missCount = 0;
int CBattleScene::m_haveBomb = 0;
int CBattleScene::m_haveLife = 0;
double CBattleScene::m_rankRatio = 1.0;
void CBattleScene::StageCountReset()
{
	m_spawneSmallCount = 0;
	m_spawneMediumCount = 0;
	m_spawneLargeCount = 0;
	m_spawneDropItemCount = 0;
	m_destroySpawneSmallCount = 0;
	m_destroySpawneMediumCount = 0;
	m_destroySpawneLargeCount = 0;
	m_getSpawneDropItemCount = 0;
	m_usedBomb = 0;
	m_missCount = 0;
}


constexpr int bgFeedTime = 60; // �w�i�̃t�F�[�h�A�E�g����

// �擾�A�C�e�����v��
int CBattleScene::m_takeItemRankCount[3] = { 0,0,0 }; // ���ꂼ��̃����N��

CBattleScene::CBattleScene(int InTime) :
	CScene(InTime),
	m_playerBullet(256),
	m_player(),
	m_bgA(),
	m_bgB(),
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
	m_rank = 1 * RankBasedDigit;
	m_bulletRemoveTime = m_bulletRemoveCount = 0;

	m_game = gameP;

	// �ω��e�̒�`�ɕK�v
	CCustomBullet::SetBulletManagerPointer(&m_bulletManager);
	CBaseLauncher::SetBulletManagerPointer(&m_bulletManager);
	CBaseLauncher::SetBeamManagerPointer(&m_beamManager);

	SetBattleScene(this);
	m_stageManager.SetBattleScene(this);
	m_player.SetBattleScene(this);

	// �f�o�b�O�p�̑S�G�\��
	m_initPlayerPos = m_player.m_pos;

	m_bgA.SetInitPlayerPos(m_player.m_pos);
	m_bgB.SetInitPlayerPos(m_player.m_pos);
	m_bgA.Set();
	m_bgB.Set();

	m_activeBg = &m_bgA;
	m_activeBg->SetFeedAlpha(1.0);
	m_nonActiveBg = &m_bgB;
	m_nonActiveBg->SetFeedAlpha(0.0);

	m_ui.Init();
	m_battleResultUI.Init();
	m_battleResultUIReset = true;
	m_feedoutCount = 0;
	m_feedinCount = 0;

	m_haveLife = 3;
	m_haveBomb = 3;

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

	m_bgA.Action();
	m_bgB.Action();
	m_player.Action(input);
	RemoveBullet(); // �e��������

	m_bgA.SetPlayerMovedPos(m_player.m_pos);
	m_bgB.SetPlayerMovedPos(m_player.m_pos);
	CBaseBullet::SetTarget(m_player.m_pos - GetBackGroundscrollSmall());
	CBaseEnemy::SetTarget(m_player.m_pos - GetBackGroundscrollSmall());
	CBaseLauncher::SetTarget(m_player.m_pos - GetBackGroundscrollSmall());
	m_bgA.SetBattleScene(this);
	m_bgB.SetBattleScene(this);
	SetPlayerPos(m_player.m_pos);

	m_playerBullet.Action();
	m_enemyManager.Action();
	m_itemManager.Action();
	m_bulletManager.Action();
	m_beamManager.Action();
	m_effectManager.Action();



	Collision_Enemy_PulyerBullet();
	Collision_Item_Player();

	//m_bgA.Draw();
	//m_bgA.Draw();
	m_nonActiveBg->Draw();
	m_activeBg->Draw();
	
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


	// �X�e�[�W�N���A���U���g
	StageClearResult();


	m_effectManager.Draw(60); // 60 UI


	// �f�o�b�O�R�}���h
	DebugCommand();

}

std::tuple<CBattleScene::ClearRank, int, int> CBattleScene::CalcClearBounus(double destoryLargeEnemyRatio, double destoryMediumEnemyRatio, double destorySmallEnemyRatio,
	double usedBomb, double missCount, double getItemRatio)
{
	int sumBombMiss = usedBomb + missCount;
	int gameLimitRank = GetRank();
	if (gameLimitRank > 100)gameLimitRank = 100;
	int rankScore = gameLimitRank * gameLimitRank;

	// �ő�l 1000*100*100*(1�`20���炢�H)
	// =200,000,000
	int totalBombMiss = m_haveLife * 3 + m_haveBomb;

	//S	�G�̌��j�������ׂ�85���ȏ�@���@�A�C�e���̎擾����90���ȏ�
	//	�{���ƃ~�X�̍��v��1��ȓ�
	if (destoryLargeEnemyRatio >= 8000 && destoryMediumEnemyRatio >= 8000 && destorySmallEnemyRatio >= 8000 &&
		getItemRatio >= 9000 && sumBombMiss <= 1) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_S, 1000 * rankScore * totalBombMiss, 5);
	}
	//A	�G�̌��j�������ׂ�60���ȏ�@���@�A�C�e���̎擾����80���ȏ�
	//	�{���ƃ~�X�̍��v��2��ȓ�
	if (destoryLargeEnemyRatio >= 6000 && destoryMediumEnemyRatio >= 6000 && destorySmallEnemyRatio >= 6000 &&
		getItemRatio >= 8000 && sumBombMiss <= 2) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_A, 500 * rankScore * totalBombMiss, 4);
	}
	//B	�G�̌��j���̍��v��50��*3�ȏ�@���@�A�C�e���̎擾����70���ȏ�
	//	�{���ƃ~�X�̍��v��3��ȓ�
	int aveRatio = (destoryLargeEnemyRatio + destoryMediumEnemyRatio + destorySmallEnemyRatio) / 3;
	if (aveRatio >= 5000 && getItemRatio >= 7000 && sumBombMiss <= 3) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_B, 300 * rankScore * totalBombMiss, 3);
	}
	//C	�G�̌��j���̍��v��40��*3�ȏ�@���@�A�C�e���̎擾����60���ȏ�
	//	�{���ƃ~�X�̍��v��4��ȓ�
	if (aveRatio >= 4000 && getItemRatio >= 6000 && sumBombMiss <= 4) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_C, 200 * rankScore * totalBombMiss, 2);
	}
	//D	�G�̌��j���̍��v��30��*3�ȏ�@���@�A�C�e���̎擾����50���ȏ�
	//	�{���ƃ~�X�̍��v��5��ȓ�
	if (aveRatio >= 3000 && getItemRatio >= 5000 && sumBombMiss <= 5) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_D, 100 * rankScore * totalBombMiss, 1);
	}
	//E	��������������Ȃ�
	return std::make_tuple(CBattleScene::ClearRank::Rank_D, 100 * rankScore * totalBombMiss, 0);
}
void CBattleScene::StageClearResult()
{
	StageManager::StageManageStatus status = m_stageManager.GetStatus();
	if (status != StageManager::StageManageStatus::ResultDrawWait) {
		return;
	}

	if (m_battleResultUIReset) {
		m_battleResultUIReset = false;
		// ���^�@���A���^�@���A��^�@���A�{���A�~�X�A�A�C�e�����A�N���A�{�[�i�X�A�����N�㏸
		// 10000�́@99.99��������9999�ɂ��邽��
		int destroySpawneSmallRatio = 0;
		if (m_spawneSmallCount > 0)destroySpawneSmallRatio = ((double)m_destroySpawneSmallCount / m_spawneSmallCount) * 10000;
		int destroySpawneMediumRatio = 0;
		if (m_spawneMediumCount > 0)destroySpawneMediumRatio = ((double)m_destroySpawneMediumCount / m_spawneMediumCount) * 10000;
		int destroySpawneLargeRatio = 0;
		if (m_spawneLargeCount > 0)destroySpawneLargeRatio = ((double)m_destroySpawneLargeCount / m_spawneLargeCount) * 10000;
		int getDropItemRatio = 0;
		if (m_spawneDropItemCount > 0)getDropItemRatio = ((double)m_getSpawneDropItemCount / m_spawneDropItemCount) * 10000;

		auto tuple = CalcClearBounus(destroySpawneLargeRatio, destroySpawneMediumRatio, destroySpawneSmallRatio, m_usedBomb, m_missCount, getDropItemRatio);
		ClearRank resultRank = std::get<0>(tuple);
		int score = std::get<1>(tuple);
		int gameRank = std::get<2>(tuple);
		m_battleResultUI.Set(
			destroySpawneLargeRatio, destroySpawneMediumRatio, destroySpawneSmallRatio,
			m_usedBomb, m_missCount, getDropItemRatio, (int)resultRank, score, gameRank);
	}
	m_battleResultUI.Draw();
	CBattleResultUI::BattleResultUIStatus uiStatus = m_battleResultUI.GetStatus();
	if (uiStatus == CBattleResultUI::BattleResultUIStatus::FeedOut ||
		uiStatus == CBattleResultUI::BattleResultUIStatus::Finish) {
		// ���U���gUI�\�����I�������
		if (m_feedoutCount < bgFeedTime) {
			m_feedoutCount++;
		}
		double feedoutAlpha = 1.0 - (double)m_feedoutCount / bgFeedTime;
		m_activeBg->SetFeedAlpha(feedoutAlpha);
		if (m_feedoutCount == bgFeedTime) { // �\�̔w�i������
			m_activeBg->SetFeedAlpha(0.0);

			if (m_feedinCount < bgFeedTime) {
				m_feedinCount++;
			}
			double feedinAlpha = (double)m_feedinCount / bgFeedTime;
			m_nonActiveBg->SetFeedAlpha(feedinAlpha);
			if (m_feedinCount == bgFeedTime) {
				m_nonActiveBg->SetFeedAlpha(1.0);
				// ���̔w�i��`�抮��
				m_activeBg->Set();

				// ����������
				CBackGroundPatternA* temp = m_nonActiveBg;
				m_nonActiveBg = m_activeBg;
				m_activeBg = temp;
				m_feedinCount = 0;
				m_feedoutCount = 0;

				m_battleResultUIReset = true;
				m_stageManager.StageReset(); // �X�|�[�����Z�b�g
				StageCountReset(); // �|�����J�E���g�̃��Z�b�g
			}
		}
	}
}


// �����NUP
void CBattleScene::AddRank(int delta)
{
	delta *= RankBasedDigit;
	m_rank += delta;
	if (m_rank < 1* RankBasedDigit)m_rank = 1 * RankBasedDigit;
	if (m_rank > 999 * RankBasedDigit)m_rank = 999 * RankBasedDigit;
}
// 1�X�e�[�W��20��Ă΂��̂Ł@2�オ��v�Z
constexpr double addRankBase = 0.1;
void CBattleScene::AddRankRatio() // �X�e�[�W����span
{
	m_rankRatio += 0.01;
	m_rank += addRankBase * m_rankRatio * RankBasedDigit;
}
void CBattleScene::AddRankRatioByStageClear(int resultrank) // S0 E5
{
	double tmp = 0;
	if (resultrank == 0)tmp = 0.1;
	else if (resultrank == 1)tmp = 0.08;
	else if (resultrank == 2)tmp = 0.06;
	else if (resultrank == 3)tmp = 0.04;
	else if (resultrank == 4)tmp = 0.02;
	else if (resultrank == 5)tmp = 0.01;
	m_rankRatio += tmp;
	m_rank += resultrank * m_rankRatio * RankBasedDigit;
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
	int limitRank = GetRank();
	if (limitRank > 100) limitRank = 100;
	int addScore = (itemRank) * (limitRank * limitRank);
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
			CBattleScene::AddDropItemCount();
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
		CBattleScene::AddDropItemCount();
	}
}

// �S�G�j��
void CBattleScene::DestoryAllEnemyNothingItemDrop() {
	m_enemyManager.DestoryAllEnemyNothingItemDrop();
}
void CBattleScene::DamageAllEnemy(int damage)
{
	m_enemyManager.DamageAllEnemy(damage);
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