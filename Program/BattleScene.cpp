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
long long CBattleScene::m_hiScore = 0;
long long CBattleScene::m_score = 0;
int CBattleScene::m_rank = 0;

// 取得アイテム合計数
int CBattleScene::m_takeItemRankCount[3] = { 0,0,0 }; // それぞれのランク毎

CBattleScene::CBattleScene(int InTime) :
	CScene(InTime),
	m_playerBullet(256),
	m_player(),
	m_bg(),
	//testLauncher(nullptr),
	m_ui()
{
	//シーン,	フェードイン時間60, フェードアウト時間60, 
	//			フェードイン濃淡0.0, フェードアウト濃淡0.0
	Set(InTime);
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

	SetBattleScene(this);
	m_stageManager.SetBattleScene(this);

	// デバッグ用の全敵表示
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
	static int count = 0;
	count++;

	////どんな状態でもアクションする処理
	m_stageManager.Main(); //敵スポーン

	m_bg.Action();
	m_player.Action(input);
	RemoveBullet(); // 弾消し処理

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
	m_effectManager.Draw(0); // 0 一番ボトム

	m_enemyManager.Draw();
	m_effectManager.Draw(10); // 10	敵よりあと

	m_playerBullet.Draw();
	m_player.Draw();
	
	m_effectManager.Draw(20); // 20 プレイヤーより後
	
	m_itemManager.Draw();
	m_effectManager.Draw(30); // 30 プレイヤーの弾より後
	m_bulletManager.Draw();
	m_effectManager.Draw(40);
	m_beamManager.Draw();
	m_effectManager.Draw(50); // 50 最前面

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

// スコア追加
void CBattleScene::AddScore(int addScore)
{
	m_score += addScore;
}

// アイテムゲット
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
	m_bulletRemoveTime = time; // 1なら1fr弾消しする
	m_bulletRemoveCount = 0;
}
// 弾消し処理
void CBattleScene::RemoveBullet() 
{
	if (m_bulletRemoveCount >= m_bulletRemoveTime) {
		return; // 弾消し終了
	}

	for (int ii = 0; ii < m_bulletManager.m_bulletTotalNum; ii++) {
		if (m_bulletManager.m_bullet[ii] == nullptr) {
			continue;
		}
		CBaseBullet* bullet = m_bulletManager.m_bullet[ii];
		bullet->SetRemove();

		// 削除時アイテムなら
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
void CBattleScene::RemoveBulletByMidiumEnemy(int id) // 弾消し処理
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

// 全敵破壊
void CBattleScene::DestoryAllEnemyNothingItemDrop() {
	m_enemyManager.DestoryAllEnemyNothingItemDrop();
}


// 全敵表示
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