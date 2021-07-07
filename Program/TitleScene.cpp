#include "TitleScene.h"
#include "BattleScene.h"
#include "Game.h"
#include "ResultScene.h"
char fileNameSpaceBG_[8][20] = {
	"X_bg_space_00",	"X_bg_space_01",	"X_bg_space_02",	"X_bg_space_03",
	"X_bg_space_04",	"X_bg_space_05",	"X_bg_space_06",	"X_bg_space_07"
};


CTitleScene::CTitleScene(int InTime) :
	CScene(InTime)
{
	//シーン,	フェードイン時間60, フェードアウト時間60, 
	//			フェードイン濃淡0.0, フェードアウト濃淡0.0
	Set(InTime);
}

CTitleScene::~CTitleScene(){

}

void CTitleScene::Init(CGame* gameP) {
	m_game = gameP;
	m_topTitleAnimeCount = 0;
	m_top_animetionFinishFlag = false;
	m_top_textBlinkAnimeCount = -1;

	m_titleLog1Pos = CPos(CGame::ToAllSizeX(0.75), CGame::ToAllSizeY(0.3));
	m_titleLog1Size = 3.0;
	m_titleLog1Alpha = 0;
	m_titleLog1 = (CImage*)CGame::GetResource(15001);

	m_titleLog2Pos = CPos(CGame::ToAllSizeX(0.20), CGame::ToAllSizeY(0.65));
	m_titleLog2Size = 2.0;
	m_titleLog2Alpha = 0;
	m_titleLog2Angle = 0;
	m_titleLog3Pos = CPos(CGame::ToAllSizeX(0.20), CGame::ToAllSizeY(0.62));
	m_titleLog3Size = 1.0;
	m_titleLog3Alpha = 0;
	m_titleLog2 = (CImage*)CGame::GetResource(15002);
	m_titleLog3 = (CImage*)CGame::GetResource(15003);

	int rand = CFunc::RandI(0, 7);
	m_background = (CImage*)CGame::GetResource(fileNameSpaceBG_[rand]);
	m_planet1 = (CImage*)CGame::GetResource("bg_planet_09");
	m_planet2 = (CImage*)CGame::GetResource("bg_planet_00");
	m_planet3 = (CImage*)CGame::GetResource("bg_planet_06");

	m_selectItemSizeMaxWidth = 0;
	for (int ii = 0; ii < SelectItemNum; ii++) {
		m_selectItem[ii][0] = (CImage*)CGame::GetResource(15004 + ii*2); // 非アクティブ
		m_selectItem[ii][1] = (CImage*)CGame::GetResource(15005 + ii*2); // アクティブ
		int dmyY;
		GetGraphSize(m_selectItem[ii][0]->m_iamge, &m_selectItemSizeWidth[ii], &dmyY);
		if (m_selectItemSizeWidth[ii] > m_selectItemSizeMaxWidth) {
			m_selectItemSizeMaxWidth = m_selectItemSizeWidth[ii];
		}
	}
	m_selectIndex = 0;

	m_selectItemFunc[0] = Play;
	m_selectItemFunc[1] = Extra;
	m_selectItemFunc[2] = Result;
	m_selectItemFunc[3] = Config;
	m_selectItemFunc[4] = Exit;

	m_status = Status::Top;


	m_playStandby_selectIndex = 0;
	for (int ii = 0; ii < SelectRankItemNum; ii++) {
		m_playStandby_selectItem[ii][0] = (CImage*)CGame::GetResource(15014 + ii * 2); // 非アクティブ
		m_playStandby_selectItem[ii][1] = (CImage*)CGame::GetResource(15015 + ii * 2); // アクティブ
		int dmyY;
		GetGraphSize(m_playStandby_selectItem[ii][0]->m_iamge, &m_playStandby_selectItemSizeWidth[ii], &dmyY);
		if (m_playStandby_selectItemSizeWidth[ii] > m_playStandby_selectItemSizeMaxWidth) {
			m_playStandby_selectItemSizeMaxWidth = m_playStandby_selectItemSizeWidth[ii];
		}
	}

	// 武器選択
	m_WeaponSelect_Left = (CImage*)CGame::GetResource(15026);
	m_WeaponSelect_Right = (CImage*)CGame::GetResource(15027);
	m_WeaponSelect_Rapid = (CImage*)CGame::GetResource(15028);
	m_WeaponSelect_Slow = (CImage*)CGame::GetResource(15029);
	m_WeaponSelect_Speed[0] = (CImage*)CGame::GetResource(15030);
	m_WeaponSelect_Speed[1] = (CImage*)CGame::GetResource(15031);
	m_WeaponSelect_ShotType[0] = (CImage*)CGame::GetResource(15032);
	m_WeaponSelect_ShotType[1] = (CImage*)CGame::GetResource(15033);
	m_WeaponSelect_Sortie[0] = (CImage*)CGame::GetResource(15034);
	m_WeaponSelect_Sortie[1] = (CImage*)CGame::GetResource(15035);
	for (int ii = 0; ii < 4; ii++) {
		m_WeaponSelect_Weapon[ii][0] = (CImage*)CGame::GetResource(15036 + ii * 2);
		m_WeaponSelect_Weapon[ii][1] = (CImage*)CGame::GetResource(15037 + ii * 2);
	}
	for (int ii = 0; ii < 10; ii++) {
		m_WeaponSelect_Number[ii][0] = (CImage*)CGame::GetResource(15044 + ii);
		m_WeaponSelect_Number[ii][1] = (CImage*)CGame::GetResource(15054 + ii);
	}
	m_WeaponSelect_SelectWeapon = (CImage*)CGame::GetResource(15065);
	m_WeaponSelect_StartRank = (CImage*)CGame::GetResource(15066);

	m_WeaponSelect_selectIndex = 0;
	m_SelectedWeapon[0] = 0;
	m_SelectedWeapon[1] = 1; // ラピッド　スロー
	m_SelectedSpeed[0] = 7;
	m_SelectedSpeed[1] = 4; // ラピッド　スロー

	m_sceneReturnStatus = SceneReturnStatus::Nothing;

	// リザルト画面
	m_ResultImage = (CImage*)CGame::GetResource(15092);
	m_resultIndex = 0;
	for (int ii = 0; ii < 10; ii++) {
		m_strScoreImage[ii] = (CImage*)CGame::GetResource(15044 + ii);
	}

	m_strRankingGold = (CImage*)CGame::GetResource(15093);
	m_strRankingSliver = (CImage*)CGame::GetResource(15094);
	for (int ii = 0; ii < 10; ii++) {
		m_strRankingBronzeImage[ii] = (CImage*)CGame::GetResource(15095+ii);
	}
	for (int ii = 0; ii < NameEntryFontMax; ii++) {
		m_strImage[ii] = (CImage*)CGame::GetResource(16000 + ii);
	}
	for (int ii = 0; ii < 10; ii++) {
		m_strNumber[ii] = m_strImage[26 + ii];
	}
	m_colonImage = (CImage*)CGame::GetResource(15110);
	m_slashImage = (CImage*)CGame::GetResource(15111);
}


void CTitleScene::Main(CInputAllStatus *input){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	// リトライなら何もせずにシーン以降する
	if (m_sceneReturnStatus == SceneReturnStatus::Retry) {
		return;
	}
	if (m_sceneReturnStatus == SceneReturnStatus::GameOver) {
		return;
	}

	if(m_status == Status::Top){
		Action(input);
	}
	Draw();

	if (m_status == Status::PlayStandby) {
		Action_PlayStandby(input);
		Draw_PlayStandby();
	} else if (m_status == Status::PlayWeaponSelect) {
		Action_WeaponSelect(input);
		Draw_WeaponSelect();
	} else if (m_status == Status::ShowResult) {
		Draw_Result();
		Action_Result(input);
	}
}

constexpr int TitleAnimeTime1 = 60;
void CTitleScene::Draw()
{
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.5), CGame::ToAllSizeY(0.5)), 1.0, 0, m_background->m_iamge);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(1.0), CGame::ToAllSizeY(1.0)), 1.0, 0, m_planet1->m_iamge);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.1), CGame::ToAllSizeY(0.85)), 0.2, 0, m_planet2->m_iamge);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.7), CGame::ToAllSizeY(0.2)), 0.1, 0, m_planet3->m_iamge);

	m_topTitleAnimeCount++;
	if (m_topTitleAnimeCount <= TitleAnimeTime1) {
		m_titleLog1Size -= (2.0 / TitleAnimeTime1);
		m_titleLog1Alpha += 255 / TitleAnimeTime1;
		m_titleLog1Pos.x -= CGame::ToAllSizeX(0.55) / TitleAnimeTime1;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_titleLog1Alpha);
	}
	CDxFunc::MyDrawRotaGraph(m_titleLog1Pos, m_titleLog1Size, 0, m_titleLog1->m_iamge);

	SetDrawBlendMode(DX_BLENDMODE_ADD, m_titleLog2Alpha);
	constexpr int TitleAnime2TotalTime = 30;
	constexpr int TitleAnimeTime2 = TitleAnimeTime1 + TitleAnime2TotalTime;
	if (m_topTitleAnimeCount >= TitleAnimeTime1 && m_topTitleAnimeCount <= TitleAnimeTime2) {
		m_titleLog2Size -= (1.0 / TitleAnimeTime1);
		m_titleLog2Alpha += 255 / TitleAnime2TotalTime;
		m_titleLog2Angle += CFunc::ToRad(175.0 / TitleAnime2TotalTime);

		m_titleLog3Alpha += 255 / TitleAnime2TotalTime;
	}
	if (m_topTitleAnimeCount > TitleAnimeTime1) {
		m_top_animetionFinishFlag = true;
	}
	CDxFunc::MyDrawRotaGraph(m_titleLog2Pos, m_titleLog2Size, m_titleLog2Angle, m_titleLog2->m_iamge);
	CDxFunc::MyDrawRotaGraph(m_titleLog2Pos, m_titleLog2Size, -m_titleLog2Angle, m_titleLog2->m_iamge);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_titleLog3Alpha);
	CDxFunc::MyDrawRotaGraph(m_titleLog3Pos, m_titleLog3Size, 0.0, m_titleLog3->m_iamge);

	// 選択肢
	if (m_status == Status::Top) {
		for (int ii = 0; ii < SelectItemNum; ii++) {
			int isAct = 0;
			if (ii == m_selectIndex)isAct = 1;

			double size = 1.0;
			if (ii == m_selectIndex)size = 1.2;

			int x = CGame::ToAllSizeX(0.7) - (m_selectItemSizeMaxWidth - m_selectItemSizeWidth[ii])/2;
			int y = CGame::ToAllSizeY(0.45 + 0.1 * (double)ii);
			CDxFunc::MyDrawRotaGraph(CPos(x,y ), size, 0.0, m_selectItem[ii][isAct]->m_iamge);
		}
	}
}

void CTitleScene::Action(CInputAllStatus* input)
{
	//フェード中
	if (NowFeed()) {
		return;
	}

	if (!m_top_animetionFinishFlag) {
		if (input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH) {
			m_top_animetionFinishFlag = true;
			m_titleLog1Size = 1.0;
			m_titleLog1Alpha = 255;
			m_titleLog1Pos.x = CGame::ToAllSizeX(0.75) - CGame::ToAllSizeX(0.55);
			m_topTitleAnimeCount = TitleAnimeTime1;
		}
		return;
	}
	if (input->m_btnStatus[INPUT_DEF_UP] == INPUT_PUSH) {
		m_selectIndex--;
		if (m_selectIndex < 0)m_selectIndex = SelectItemNum - 1;
	}
	if (input->m_btnStatus[INPUT_DEF_DOWN] == INPUT_PUSH) {
		m_selectIndex++;
		if (m_selectIndex >= SelectItemNum)m_selectIndex = 0;
	}

	if (input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH) {
		input->Reset();
		m_selectItemFunc[m_selectIndex](this);
	}
	
	if (input->m_btnStatus[INPUT_DEF_CANCEL] == INPUT_PUSH) {
		m_selectIndex = SelectItemNum - 1; // キャンセルボタンは一番下に移動
	}
}

//------------------------------------------------------
void CTitleScene::Draw_PlayStandby()
{
	// 選択肢
	for (int ii = 0; ii < SelectRankItemNum; ii++) {
		int isAct = 0;
		if (ii == m_playStandby_selectIndex)isAct = 1;

		double size = 0.8;
		if (ii == m_playStandby_selectIndex)size = 1.0;

		int x = CGame::ToAllSizeX(0.7) - (m_playStandby_selectItemSizeMaxWidth - m_playStandby_selectItemSizeWidth[ii]) / 2;
		int y = CGame::ToAllSizeY(0.45 + 0.08 * (double)ii);
		CDxFunc::MyDrawRotaGraph(CPos(x, y), size, 0.0, m_playStandby_selectItem[ii][isAct]->m_iamge);
	}
}
void CTitleScene::Action_PlayStandby(CInputAllStatus* input)
{
	//フェード中
	if (NowFeed()) {
		return;
	}

	if (input->m_btnStatus[INPUT_DEF_UP] == INPUT_PUSH) {
		m_playStandby_selectIndex--;
		if (m_playStandby_selectIndex < 0)m_playStandby_selectIndex = SelectRankItemNum - 1;
	}
	if (input->m_btnStatus[INPUT_DEF_DOWN] == INPUT_PUSH) {
		m_playStandby_selectIndex++;
		if (m_playStandby_selectIndex >= SelectRankItemNum)m_playStandby_selectIndex = 0;
	}

	if (input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH) {
		m_selectIndex = 0;
		input->Reset();
		m_status = Status::PlayWeaponSelect;
	}
	if (input->m_btnStatus[INPUT_DEF_CANCEL] == INPUT_PUSH) {
		m_selectIndex = 0;
		m_status = Status::Top;
	}
}

//------------------------------------------------------
void CTitleScene::Draw_WeaponSelect()
{
	int isAct = 0;
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.65), CGame::ToAllSizeY(0.30)), 1.0, 0.0, m_WeaponSelect_Rapid->m_iamge);
	{
		isAct = (m_WeaponSelect_selectIndex == 0) ? 1 : 0;
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.7), CGame::ToAllSizeY(0.40)), 1.0, 0.0, m_WeaponSelect_Speed[isAct]->m_iamge); // スピード
		CImage* number = m_WeaponSelect_Number[m_SelectedSpeed[0]][isAct];
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.89), CGame::ToAllSizeY(0.40)), 1.0, 0.0, number->m_iamge); // スピード

		isAct = (m_WeaponSelect_selectIndex == 1) ? 1 : 0;
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.68), CGame::ToAllSizeY(0.47)), 1.0, 0.0, m_WeaponSelect_ShotType[isAct]->m_iamge); // タイプ
		CImage* text = m_WeaponSelect_Weapon[m_SelectedWeapon[0]][isAct];
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.888), CGame::ToAllSizeY(0.47)), 1.0, 0.0, text->m_iamge); // スピード
	}

	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.65), CGame::ToAllSizeY(0.60)), 1.0, 0.0, m_WeaponSelect_Slow->m_iamge);
	{
		isAct = (m_WeaponSelect_selectIndex == 2) ? 1 : 0;
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.7), CGame::ToAllSizeY(0.70)), 1.0, 0.0, m_WeaponSelect_Speed[isAct]->m_iamge); // スピード
		CImage* number = m_WeaponSelect_Number[m_SelectedSpeed[1]][isAct];
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.89), CGame::ToAllSizeY(0.70)), 1.0, 0.0, number->m_iamge); // スピード

		isAct = (m_WeaponSelect_selectIndex == 3) ? 1 : 0;
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.68), CGame::ToAllSizeY(0.77)), 1.0, 0.0, m_WeaponSelect_ShotType[isAct]->m_iamge); // タイプ
		CImage* text = m_WeaponSelect_Weapon[m_SelectedWeapon[1]][isAct];
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.888), CGame::ToAllSizeY(0.77)), 1.0, 0.0, text->m_iamge); // スピード
	}

	isAct = (m_WeaponSelect_selectIndex == 4) ? 1 : 0;
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.7), CGame::ToAllSizeY(0.90)), 1.0, 0.0, m_WeaponSelect_Sortie[isAct]->m_iamge); // 出撃

	if (m_WeaponSelect_selectIndex != 4) {
		double lX;
		double rX;
		if (m_WeaponSelect_selectIndex == 0 || m_WeaponSelect_selectIndex == 2) { // スピード
			lX = 0.85;
			rX = 0.93;
		} if (m_WeaponSelect_selectIndex == 1 || m_WeaponSelect_selectIndex == 3) { // ショットタイプ
			lX = 0.824;
			rX = 0.950;
		}
		double leftRightrY[4] = { 0.40, 0.47, 0.70, 0.77 };
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(lX), CGame::ToAllSizeY(leftRightrY[m_WeaponSelect_selectIndex] - 0.005)), 1.0, 0.0, m_WeaponSelect_Left->m_iamge);
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(rX), CGame::ToAllSizeY(leftRightrY[m_WeaponSelect_selectIndex] - 0.005)), 1.0, 0.0, m_WeaponSelect_Right->m_iamge);
	}
}
void CTitleScene::Action_WeaponSelect(CInputAllStatus* input)
{
	//フェード中
	if (NowFeed()) {
		return;
	}

	if (input->m_btnStatus[INPUT_DEF_UP] == INPUT_PUSH) {
		m_WeaponSelect_selectIndex--;
		if (m_WeaponSelect_selectIndex < 0)m_WeaponSelect_selectIndex = SelectWeaponMenuNum - 1;
	}
	if (input->m_btnStatus[INPUT_DEF_DOWN] == INPUT_PUSH) {
		m_WeaponSelect_selectIndex++;
		if (m_WeaponSelect_selectIndex >= SelectWeaponMenuNum)m_WeaponSelect_selectIndex = 0;
	}

	int* p = nullptr;
	if (m_WeaponSelect_selectIndex == 0)p = &(m_SelectedSpeed[0]);
	if (m_WeaponSelect_selectIndex == 1)p = &(m_SelectedWeapon[0]);
	if (m_WeaponSelect_selectIndex == 2)p = &(m_SelectedSpeed[1]);
	if (m_WeaponSelect_selectIndex == 3)p = &(m_SelectedWeapon[1]);
	int min = 0, max = 0;
	if (m_WeaponSelect_selectIndex == 1 || m_WeaponSelect_selectIndex == 3) {
		min = 0;
		max = SelectWeaponNum;
	}
	if (m_WeaponSelect_selectIndex == 0 || m_WeaponSelect_selectIndex == 2) {
		min = 1;
		max = 10;
	}

	if (input->m_btnStatus[INPUT_DEF_RIGHT] == INPUT_PUSH) {
		(*p)++;
		if ((*p) >= max )(*p) = min;
	}
	if (input->m_btnStatus[INPUT_DEF_LEFT] == INPUT_PUSH) {
		(*p)--;
		if (*p <= 0)(*p) = max-1;
	}

	if (input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH) {
		if (m_WeaponSelect_selectIndex == 4) {

			SetFeedOut(30); // 今のシーンのフェードアウト時間

			CBattleScene* battleScene = new CBattleScene(30); // 次のシーンのフェードイン時間
			battleScene->Init(this->m_game, SelectRankItems[m_playStandby_selectIndex],
				m_SelectedSpeed[0],	m_SelectedWeapon[0],
				m_SelectedSpeed[1],	m_SelectedWeapon[1]	);	//シーンにゲームクラスポインタを渡す
			battleScene->SetTitleScene(this); // 下位シーンに本シーンを伝えておく
			SetNextScene(battleScene); // 次のシーンを設定
			ChangeScene(); // シーン切り替え
		}
		else {
			m_WeaponSelect_selectIndex = 4;
		}
	}
	if (input->m_btnStatus[INPUT_DEF_CANCEL] == INPUT_PUSH) {
		m_status = Status::PlayStandby;
	}
}

//------------------------------------------------------
// リザルト
void CTitleScene::Draw_Result()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawBox(CGame::ToAllSizeX(0.05), CGame::ToAllSizeY(0.05),
			CGame::ToAllSizeX(0.95), CGame::ToAllSizeY(0.95), GetColor(0, 0, 0), TRUE);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	// Result
	// ノーマル　<- ->　EXTRA
	// 1
	// 2
	CImage* result = m_selectItem[2][0];
	CImage* extra = m_selectItem[1][0];
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.4), CGame::ToAllSizeY(0.1)), 1.0, 0, result->m_iamge);
	if (m_resultIndex == 0) {
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.5), CGame::ToAllSizeY(0.1)), 1.0, 0, m_strImage[36]->m_iamge);
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.6), CGame::ToAllSizeY(0.1)), 1.0, 0, m_ResultImage->m_iamge);
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.7), CGame::ToAllSizeY(0.1)), 1.0, 0, m_strImage[36]->m_iamge);
	} else {
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.5), CGame::ToAllSizeY(0.1)), 1.0, 0, m_strImage[36]->m_iamge);
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.6), CGame::ToAllSizeY(0.1)), 1.0, 0, extra->m_iamge);
		CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.7), CGame::ToAllSizeY(0.1)), 1.0, 0, m_strImage[36]->m_iamge);
	}

	// 表示する範囲
	int y_min = 1; // 1位
	int y_max = 10; // 10位
	std::vector<SaveDatus>& saveData = m_game->GetSaveData();
	int count = 0;
	for (int ii = 0; ii < saveData.size(); ii++) {
		if (ii < y_min-1)continue;
		if (ii > y_max-1)continue;
		SaveDatus s = saveData[ii];
		Draw_ResultSub(count,ii+1, false, s);
		count++;
	}
	for (int ii = count; ii < 10; ii++) {
		SaveDatus s;
		Draw_ResultSub(count, ii + 1, true, s);
		count++;
	}
}
void CTitleScene::Draw_ResultSub(int posY, int number, bool nulldata, SaveDatus& s) {

	int py = CGame::ToAllSizeY(0.3 + (0.6 / 10.0) * (double)posY);
	if (number == 1) {
		CDxFunc::MyDrawRotaGraph(CGame::ToAllSizeX(0.065), py, 0.3, 0, m_strRankingGold->m_iamge);
	}
	else if (number == 2) {
		CDxFunc::MyDrawRotaGraph(CGame::ToAllSizeX(0.065), py, 0.25, 0, m_strRankingSliver->m_iamge);
	}
	else {
		std::vector<int> numberAry;
		CFunc::GetDigitArray2(number, numberAry);
		if (number < 10) {
			CDxFunc::MyDrawRotaGraph(CGame::ToAllSizeX(0.065), py, 0.25, 0, m_strRankingBronzeImage[numberAry[0]]->m_iamge);
		}
		else {
			for (int ii = 0; ii < numberAry.size(); ii++) {
				CDxFunc::MyDrawRotaGraph(CGame::ToAllSizeX(0.065) - 14 + ii * 24, py, 0.25, 0, m_strRankingBronzeImage[numberAry[ii]]->m_iamge);
			}
		}
	}

	if (nulldata) {
		return;
	}

	// スコア
	std::vector<int> scoreAry;
	CFunc::GetDigitArray2(s.m_score, scoreAry);
	int digit = CFunc::GetDigit(s.m_score);
	int coA = 0;
	SetDrawBright(128,128,128);
	for (int ii = 0; ii < (12-digit); ii++) {
		double x = (coA * 26) + CGame::ToAllSizeX(0.09);
		double y = py;
		CDxFunc::MyDrawRotaGraph(x, y, 0.4, 0, m_strScoreImage[0]->m_iamge);
		coA++;
	}
	SetDrawBright(255,255,255);
	for (int ii = 0; ii < scoreAry.size(); ii++) {
		int index = scoreAry[ii];
		if (index < 0 || index > 9) {
			continue;
		}
		double x = (coA * 26) + CGame::ToAllSizeX(0.09);
		double y = py;
		CDxFunc::MyDrawRotaGraph(x, y, 0.4, 0, m_strScoreImage[index]->m_iamge);
		coA++;
	}

	//　名前
	for (int ii = 0; ii < 10; ii++) {
		double x13 = CGame::ToAllSizeX(0.3) + ii * 22;
		double y13 = py;
		int index = s.m_name[ii];
		bool space = false;
		if (index == NameEntryFontMax_s - 1) {
			space = true;// 空白
		}
		if (!space) {
			CImage* img = m_strImage[index];
			CDxFunc::MyDrawRotaGraph(x13, y13, 0.4, 0, img->m_iamge);
		}
	}

	// 年月日
	{
		std::vector<int> yearAry, monAry, dayAry, hourAry, minAry, secAry;
		CFunc::GetDigitArray3(4, s.m_year, yearAry);
		CFunc::GetDigitArray3(2, s.m_mon, monAry);
		CFunc::GetDigitArray3(2, s.m_day, dayAry);
		CFunc::GetDigitArray3(2, s.m_hour, hourAry);
		CFunc::GetDigitArray3(2, s.m_min, minAry);
		CFunc::GetDigitArray3(2, s.m_sec, secAry);

		double size = 0.4;
		int wigth = 18;
		auto draw = [&](int value, int& count) {
			double x = CGame::ToAllSizeX(0.6) + count * wigth;
			CImage* img = m_strNumber[value];
			CDxFunc::MyDrawRotaGraph(x, py, size, 0, img->m_iamge);
			count++;
		};

		int co = 0;
		for (int ii = 0; ii < yearAry.size(); ii++) {
			draw(yearAry[ii], co);
		}
		double x;
		x = CGame::ToAllSizeX(0.6) + co * wigth;
		CDxFunc::MyDrawRotaGraph(x, py, size, 0, m_slashImage->m_iamge);

		co++;
		for (int ii = 0; ii < monAry.size(); ii++) {
			draw(monAry[ii], co);
		}

		x = CGame::ToAllSizeX(0.6) + co * wigth;
		CDxFunc::MyDrawRotaGraph(x, py, size, 0, m_slashImage->m_iamge);
		co++;
		for (int ii = 0; ii < dayAry.size(); ii++) {
			draw(dayAry[ii], co);
		}

		co++;
		for (int ii = 0; ii < hourAry.size(); ii++) {
			draw(hourAry[ii], co);
		}
		x = CGame::ToAllSizeX(0.6) + co * wigth;
		CDxFunc::MyDrawRotaGraph(x, py, size, 0, m_colonImage->m_iamge);
		co++;
		for (int ii = 0; ii < minAry.size(); ii++) {
			draw(minAry[ii], co);
		}
		x = CGame::ToAllSizeX(0.6) + co * wigth;
		CDxFunc::MyDrawRotaGraph(x, py, size, 0, m_colonImage->m_iamge);
		co++;
		for (int ii = 0; ii < secAry.size(); ii++) {
			draw(secAry[ii], co);
		}
	}

}

void CTitleScene::Action_Result(CInputAllStatus* input)
{
	//フェード中
	if (NowFeed()) {
		return;
	}
}

// シーンに戻ってきた時の状態
void CTitleScene::SetReturnStatus_Retry() {
	m_sceneReturnStatus = SceneReturnStatus::Retry;

	SetFeedOut(0); // 今のシーンのフェードアウト時間
	CBattleScene* battleScene = new CBattleScene(30); // 次のシーンのフェードイン時間
	battleScene->Init(this->m_game, SelectRankItems[m_playStandby_selectIndex],
		m_SelectedSpeed[0], m_SelectedWeapon[0],
		m_SelectedSpeed[1], m_SelectedWeapon[1]);	//シーンにゲームクラスポインタを渡す
	battleScene->SetTitleScene(this); // 下位シーンに本シーンを伝えておく
	SetNextScene(battleScene); // 次のシーンを設定
	ChangeScene(); // シーン切り替え
}
void CTitleScene::SetReturnStatus_GoTitle() {
	m_sceneReturnStatus = SceneReturnStatus::GoTitle;
	Init(this->m_game);
}
void CTitleScene::SetReturnStatus_GameOver(
	int startRank, int endRank,
	int stage, int item, int liveTime,
	int rapidShot, int rapidspeed, int slowShot, int slowspeed, long long score
){
	m_sceneReturnStatus = SceneReturnStatus::GameOver;

	SetFeedOut(0); // 今のシーンのフェードアウト時間
	CResultScene* resultScene = new CResultScene(30); // 次のシーンのフェードイン時間
	resultScene->Init(this->m_game);	//シーンにゲームクラスポインタを渡す
	resultScene->SetTitleScene(this); // 下位シーンに本シーンを伝えておく
	resultScene->SetData(startRank, endRank,stage, item, liveTime,rapidShot, rapidspeed, slowShot, slowspeed, score);
	SetNextScene(resultScene); // 次のシーンを設定
	ChangeScene(); // シーン切り替え
}
void CTitleScene::SetReturnStatus_GoResult()
{
	m_sceneReturnStatus = SceneReturnStatus::GoResult;
	Init(this->m_game);

}

//--------------------
// 関数ポインタ
void CTitleScene::Play(CTitleScene* thisScene)
{
	thisScene->m_playStandby_selectIndex = 0;
	thisScene->m_status = Status::PlayStandby;
}
void CTitleScene::Extra(CTitleScene* thisScene)
{

}
void CTitleScene::Result(CTitleScene* thisScene)
{
	thisScene->m_status = Status::ShowResult;
	thisScene->m_playStandby_selectIndex = 0;

	thisScene->m_game->Load_SaveData();
}
void CTitleScene::Config(CTitleScene* thisScene)
{

}
void CTitleScene::Exit(CTitleScene* thisScene)
{
	thisScene->m_game->SetExit();
}