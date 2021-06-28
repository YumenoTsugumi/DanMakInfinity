#include "TitleScene.h"
#include "BattleScene.h"
#include "Game.h"

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
	m_selectItemFunc[2] = Data;
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
}


void CTitleScene::Main(CInputAllStatus *input){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

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

			SetNextScene(battleScene); // 次のシーンを設定
			ChangeScene(); // シーン切り替え
		}
		else {
			m_WeaponSelect_selectIndex = 4;
		}
	}
	if (input->m_btnStatus[INPUT_DEF_CANCEL] == INPUT_PUSH) {
		//m_selectIndex = 0;
		m_status = Status::PlayStandby;
	}
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
void CTitleScene::Data(CTitleScene* thisScene)
{

}
void CTitleScene::Config(CTitleScene* thisScene)
{

}
void CTitleScene::Exit(CTitleScene* thisScene)
{
	thisScene->m_game->SetExit();
}