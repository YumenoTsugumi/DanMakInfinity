#include "BattleScene.h"

#include "Player.h"

#include "Game.h"
#include "BaseBullet.h"
#include "CustomBullet.h"
#include "HomingBullet.h"
#include "BaseEffect.h"

#include "EnemyL.h"

bool CBattleScene::m_enemyHitSizeDraw = false;
bool CBattleScene::m_enemyLauncherDraw = false;
bool CBattleScene::m_enableDebugCommand = true;
CPos CBattleScene::m_playerPos;
CBattleScene* CBattleScene::m_scene = nullptr;



CEffectManager CBattleScene::m_effectManager;
CItemManager CBattleScene::m_itemManager;
CBattleScene::BulletRemoveType CBattleScene::m_bulletRemoveType = CBattleScene::BulletRemoveType::Nothing;
int CBattleScene::m_bulletRemoveTime;
int CBattleScene::m_bulletRemoveCount;

CEnemyManager CBattleScene::m_enemyManager; // 敵
CBulletManager CBattleScene::m_bulletManager; // 敵の弾
CBeamManager CBattleScene::m_beamManager; // 敵のビーム

// スコア
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
int CBattleScene::m_nowStage = 1;
constexpr int bgFeedTime = 60; // 背景のフェードアウト時間

// 取得アイテム合計数
int CBattleScene::m_takeItemRankCount[3] = { 0,0,0 }; // それぞれのランク毎

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

void CBattleScene::StaticInit() {
	m_effectManager.AllRemove();
	m_itemManager.AllRemove();
	m_bulletRemoveType = CBattleScene::BulletRemoveType::Nothing;
	m_bulletRemoveTime = 0;
	m_bulletRemoveCount = 0;

	m_enemyManager.AllRemove();
	m_bulletManager.AllRemove();
	m_beamManager.AllRemove();

	// スコア
	m_hiScore = 0;
	m_score = 0;
	m_rank = 0;

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
	m_haveBomb = 0;
	m_haveLife = 0;
	m_rankRatio = 1.0;
	m_nowStage = 1;

	m_takeItemRankCount[0] = 0;
	m_takeItemRankCount[1] = 0;
	m_takeItemRankCount[2] = 0;
}


CBattleScene::CBattleScene(int InTime) :
	CScene(InTime),
	m_playerBullet(256),
	m_player(),
	m_bgA(),
	m_bgB(),
	m_ui()
{
	//シーン,	フェードイン時間60, フェードアウト時間60, 
	//			フェードイン濃淡0.0, フェードアウト濃淡0.0
	Set(InTime);
}

CBattleScene::~CBattleScene(){

}

void CBattleScene::Init(CGame* gameP, int rank, int rapidSpeed, int rapidWeapon, int slowSpeed, int slowWeapon) {
	StaticInit();
	m_player.Init();
	m_player.SetBulletManager(&m_playerBullet); // プレイヤーにプレイヤ弾管理を設定

	m_hiScore = 0;
	m_score = 0;
	m_rank = rank * RankBasedDigit;
	m_bulletRemoveTime = m_bulletRemoveCount = 0;

	m_game = gameP;

	// 変化弾の定義に必要
	CCustomBullet::SetBulletManagerPointer(&m_bulletManager);
	CBaseLauncher::SetBulletManagerPointer(&m_bulletManager);
	CBaseLauncher::SetBeamManagerPointer(&m_beamManager);

	SetBattleScene(this);
	m_stageManager.SetBattleScene(this);
	m_player.SetBattleScene(this);

	m_player.SetSpeed(rapidSpeed, slowSpeed);
	m_player.SetShotType((CPlayer::ShotType)rapidWeapon, (CPlayer::ShotType)slowWeapon);

	// デバッグ用の全敵表示
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

	m_haveLife = 0;
	m_haveBomb = 3;

	m_isEscMenu = false;
	m_resumuCount = 0;
	m_escMenuIndex = 0;
	for (int ii = 0; ii < SelectEscMenuNum; ii++) {
		m_escSelectItem[ii][0] = (CImage*)CGame::GetResource(15070 + ii * 2);
		m_escSelectItem[ii][1] = (CImage*)CGame::GetResource(15071 + ii * 2);

		int dmyY;
		GetGraphSize(m_escSelectItem[ii][0]->m_iamge, &m_escItemSize[ii], &dmyY);
		if (m_escItemSize[ii] > m_escItemSizeMaxWidth) {
			m_escItemSizeMaxWidth = m_escItemSize[ii];
		}
	}
	m_escItemFunc[0] = Resume;
	m_escItemFunc[1] = Retry;
	m_escItemFunc[2] = GoTitle;

	m_counterImage[0] = (CImage*)CGame::GetResource(15076);
	m_counterImage[1] = (CImage*)CGame::GetResource(15077);
	m_counterStringImage[0] = (CImage*)CGame::GetResource(15080);
	m_counterStringImage[1] = (CImage*)CGame::GetResource(15079);
	m_counterStringImage[2] = (CImage*)CGame::GetResource(15078);

	m_isGameOver = false;
	m_drawPlayer = true;
}

void CBattleScene::Main(CInputAllStatus *input){

	// ポーズ画面でない
	if (!m_isEscMenu && m_resumuCount == 0) {
		bool isPause = false;

		//フェード中でなければ
		if (!NowFeed()) {
			//入力参照
			if (input->m_btnStatus[INPUT_DEF_ESC] == INPUT_PUSH) {
				isPause = true;
			}
		}

		if (!m_isGameOver) {
			//どんな状態でもアクションする処理
			m_stageManager.Main(); //敵スポーン
		}

		m_bgA.Action();
		m_bgB.Action();
		m_player.Action(input);
		RemoveBullet(); // 弾消し処理

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

		Collision_EnemyBullet_Pulyer();
		Collision_Enemy_PulyerBullet();
		Collision_Item_Player();
		

		// 残機が0で、死亡アニメーションが終わったなら、シーンを切り替える
		// 何もさせない
		if (m_haveLife < 0) {
			if (!m_player.IsDieAnimetion()) {
				GoGameOver();
				m_drawPlayer = false;
			}
			m_isGameOver = true;
		}

		m_nonActiveBg->Draw();
		m_activeBg->Draw();

		m_effectManager.Draw(0); // 0 一番ボトム

		m_enemyManager.Draw();
		m_effectManager.Draw(10); // 10	敵よりあと

		m_playerBullet.Draw();
		// ゲームオーバーの処理中なのでプレイヤーは表示しない
		if (m_drawPlayer) {
			m_player.Draw();
		}

		m_effectManager.Draw(20); // 20 プレイヤーより後

		m_itemManager.Draw();
		m_effectManager.Draw(30); // 30 プレイヤーの弾より後
		m_bulletManager.Draw();
		m_effectManager.Draw(40);
		m_beamManager.Draw();
		m_effectManager.Draw(50); // 50 最前面

		m_ui.Draw();

		if (!m_isGameOver) {
			// ステージクリアリザルト
			StageClearResult();
		}

		// ポーズ画面に移行
		if (isPause) {
			m_isEscMenu = true;

			SaveDrawScreen(0, 0, CGame::GetAllGameRect().x, CGame::GetAllGameRect().y, "pause.bmp");
			m_pauseImage = LoadGraph("pause.bmp");
			isPause = false;
		}

		m_effectManager.Draw(60); // 60 UI

		// デバッグコマンド
		DebugCommand();

	}
	else {
		if (m_isEscMenu) {
			if (input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH) {
				m_escItemFunc[m_escMenuIndex](this);
			}
			if (input->m_btnStatus[INPUT_DEF_UP] == INPUT_PUSH) {
				m_escMenuIndex--;
				if (m_escMenuIndex < 0)m_escMenuIndex = SelectEscMenuNum - 1;
			}
			if (input->m_btnStatus[INPUT_DEF_DOWN] == INPUT_PUSH) {
				m_escMenuIndex++;
				if (m_escMenuIndex >= SelectEscMenuNum)m_escMenuIndex = 0;
			}
		}

		// ESC画面
		EscDraw();
	}
}

void CBattleScene::EscDraw()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	DrawGraph(0, 0, m_pauseImage, FALSE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(0, 0, CGame::GetAllGameRect().x, CGame::GetAllGameRect().y, GetColor(0, 0, 0), TRUE);

	// ポーズ解除
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (m_resumuCount > 0) {
		m_resumuCount--;

		int co = m_resumuCount % 60;

		double x = CGame::ToGamePosX(0.5) + cos(CFunc::ToRad(270 - (360.0 / 60.0) * co)) * 200;
		double y = CGame::ToGamePosY(0.5) + sin(CFunc::ToRad(270 - (360.0 / 60.0) * co)) * 200;

		CounterImage* ip = &(m_counterImageData[co]);
		ip->pos.x = x;
		ip->pos.y = y;
		ip->alpha = 255;
		for (auto& d : m_counterImageData) {
			d.alpha -= 30;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, d.alpha);
			CDxFunc::MyDrawRotaGraph(d.pos, 1.0, 0.0, m_counterImage[0]->m_iamge);
		}

		int countDown = m_resumuCount / 60;
		// 180~120 なら 2    120～60 なら1 　60～なら0
		double size = 3.0;
		double alpha = 1.0;
		if (m_resumuCount >= 120) {
			size = 1.5 + 1.5 * (m_resumuCount - 120) / 60.0;
			alpha = 0.2 + 1.0 * (m_resumuCount - 120) / 60.0;
		}
		else if (m_resumuCount >= 60) {
			size = 1.5 + 1.5 * (m_resumuCount - 60) / 60.0;
			alpha = 0.2 + 1.0 * (m_resumuCount - 60) / 60.0;
		}
		else {
			size = 1.5 + 1.5 * (m_resumuCount - 0) / 60.0;
			alpha = 0.2 + 1.0 * (m_resumuCount - 0) / 60.0;
		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * alpha);
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToGamePosX(0.5), CGame::ToGamePosY(0.5)),
			size, 0.0, m_counterStringImage[countDown]->m_iamge);
	}
	else {
		for (int ii = 0; ii < SelectEscMenuNum; ii++) {
			int isAct = 0;
			if (ii == m_escMenuIndex)isAct = 1;

			double size = 1.0;
			if (ii == m_escMenuIndex)size = 1.2;

			int x = CGame::ToGamePosX(0.5) - (m_escItemSizeMaxWidth - m_escItemSize[ii]) / 2;
			int y = CGame::ToGamePosY(0.5 + 0.1 * (double)ii);
			CDxFunc::MyDrawRotaGraph(CPos(x, y), size, 0.0, m_escSelectItem[ii][isAct]->m_iamge);
		}
	}
}
void CBattleScene::Resume(CBattleScene* thisScene)
{
	thisScene->m_isEscMenu = false;
	thisScene->m_resumuCount = ResumeTime;
}
void CBattleScene::Retry(CBattleScene* thisScene)
{
	thisScene->m_titleScene->SetReturnStatus_Retry();
	thisScene->SetFeedOut(30);
	thisScene->SetBackScene();
}
void CBattleScene::GoTitle(CBattleScene* thisScene)
{
	thisScene->m_titleScene->SetReturnStatus_GoTitle();
	thisScene->SetFeedOut(30);
	thisScene->SetBackScene();
}
void CBattleScene::GoGameOver()
{
	/*
	int startRank, int endRank,
	int stage, int item, int liveTime,
	int rapidShot, int rapidspeed, int slowShot, int slowspeed, long long score
	*/
	m_titleScene->SetReturnStatus_GameOver(789,999,120,86754,557,0,5,1,1,123456789012);
	SetFeedOut(0);
	SetBackScene();
}


std::tuple<CBattleScene::ClearRank, int, int> CBattleScene::CalcClearBounus(double destoryLargeEnemyRatio, double destoryMediumEnemyRatio, double destorySmallEnemyRatio,
	double usedBomb, double missCount, double getItemRatio)
{
	int sumBombMiss = usedBomb + missCount;
	int gameLimitRank = GetRank();
	if (gameLimitRank > 100)gameLimitRank = 100;
	int rankScore = gameLimitRank * gameLimitRank;

	// 最大値 1000*100*100*(1～20くらい？)
	// =200,000,000
	int totalBombMiss = m_haveLife * 3 + m_haveBomb;

	//S	敵の撃破率がすべて85％以上　かつ　アイテムの取得率が90％以上
	//	ボムとミスの合計が1回以内
	if (destoryLargeEnemyRatio >= 8000 && destoryMediumEnemyRatio >= 8000 && destorySmallEnemyRatio >= 8000 &&
		getItemRatio >= 9000 && sumBombMiss <= 1) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_S, 1000 * rankScore * totalBombMiss, 5);
	}
	//A	敵の撃破率がすべて60％以上　かつ　アイテムの取得率が80％以上
	//	ボムとミスの合計が2回以内
	if (destoryLargeEnemyRatio >= 6000 && destoryMediumEnemyRatio >= 6000 && destorySmallEnemyRatio >= 6000 &&
		getItemRatio >= 8000 && sumBombMiss <= 2) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_A, 500 * rankScore * totalBombMiss, 4);
	}
	//B	敵の撃破率の合計が50％*3以上　かつ　アイテムの取得率が70％以上
	//	ボムとミスの合計が3回以内
	int aveRatio = (destoryLargeEnemyRatio + destoryMediumEnemyRatio + destorySmallEnemyRatio) / 3;
	if (aveRatio >= 5000 && getItemRatio >= 7000 && sumBombMiss <= 3) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_B, 300 * rankScore * totalBombMiss, 3);
	}
	//C	敵の撃破率の合計が40％*3以上　かつ　アイテムの取得率が60％以上
	//	ボムとミスの合計が4回以内
	if (aveRatio >= 4000 && getItemRatio >= 6000 && sumBombMiss <= 4) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_C, 200 * rankScore * totalBombMiss, 2);
	}
	//D	敵の撃破率の合計が30％*3以上　かつ　アイテムの取得率が50％以上
	//	ボムとミスの合計が5回以内
	if (aveRatio >= 3000 && getItemRatio >= 5000 && sumBombMiss <= 5) {
		return std::make_tuple(CBattleScene::ClearRank::Rank_D, 100 * rankScore * totalBombMiss, 1);
	}
	//E	いずれも満たさない
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
		// 小型機率、中型機率、大型機率、ボム、ミス、アイテム率、クリアボーナス、ランク上昇
		// 10000は　99.99％を桁を9999にするため
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
		// リザルトUI表示が終わったら
		if (m_feedoutCount < bgFeedTime) {
			m_feedoutCount++;
		}
		if (m_feedoutCount == 1) {
			AddNowStage(); // ステージのカウントアップ
		}
		double feedoutAlpha = 1.0 - (double)m_feedoutCount / bgFeedTime;
		m_activeBg->SetFeedAlpha(feedoutAlpha);
		if (m_feedoutCount == bgFeedTime) { // 表の背景を消去
			m_activeBg->SetFeedAlpha(0.0);

			if (m_feedinCount < bgFeedTime) {
				m_feedinCount++;
			}
			double feedinAlpha = (double)m_feedinCount / bgFeedTime;
			m_nonActiveBg->SetFeedAlpha(feedinAlpha);
			if (m_feedinCount == bgFeedTime) {
				m_nonActiveBg->SetFeedAlpha(1.0);
				// 裏の背景を描画完了
				m_activeBg->Set();

				// 初期化処理
				CBackGroundPatternA* temp = m_nonActiveBg;
				m_nonActiveBg = m_activeBg;
				m_activeBg = temp;
				m_feedinCount = 0;
				m_feedoutCount = 0;

				m_battleResultUIReset = true;
				m_stageManager.StageReset(); // スポーンリセット
				StageCountReset(); // 倒したカウントのリセット

				
			}
		}
	}
}


// ランクUP
void CBattleScene::AddRank(int delta)
{
	delta *= RankBasedDigit;
	m_rank += delta;
	if (m_rank < 1* RankBasedDigit)m_rank = 1 * RankBasedDigit;
	if (m_rank > 999 * RankBasedDigit)m_rank = 999 * RankBasedDigit;
}
void CBattleScene::AddRankRatio(double delta)
{
	m_rankRatio += delta;
	if (m_rankRatio > 10.0)m_rankRatio = 10.0;
	if (m_rankRatio < 0.1)m_rankRatio = 0.1;
}
// 1ステージで20回呼ばれるので　2上がる計算
void CBattleScene::AddRankRatioByStageSpan() // ステージ中のspan
{
	constexpr double addRankBase = 0.1;
	AddRankRatio(0.01);
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
	AddRankRatio(tmp);
	m_rank += resultrank * m_rankRatio * RankBasedDigit;
}
void CBattleScene::SetRankRatio(double ratio)
{
	m_rankRatio = ratio;
	if (m_rankRatio > 10.0)m_rankRatio = 10.0;
	if (m_rankRatio < 0.1)m_rankRatio = 0.1;
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
	int limitRank = GetRank();
	if (limitRank > 100) limitRank = 100;
	int addScore = (itemRank) * (limitRank * limitRank);
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
			CBattleScene::AddDropItemCount();
		}
	}

	m_bulletRemoveCount++;
}

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
		CBattleScene::AddDropItemCount();
	}
}

// 全敵破壊
void CBattleScene::DestoryAllEnemyNothingItemDrop() 
{
	m_enemyManager.DestoryAllEnemyNothingItemDrop();
}
void CBattleScene::DamageAllEnemy(int damage)
{
	m_enemyManager.DamageAllEnemy(damage);
}

// 全敵表示
void CBattleScene::DebugAllEnemyDirection()
{
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