#include "TitleScene.h"

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

	m_selectItemSizeMaxWigth = 0;
	for (int ii = 0; ii < SelectItemNum; ii++) {
		m_selectItem[ii][0] = (CImage*)CGame::GetResource(15004 + ii*2); // 非アクティブ
		m_selectItem[ii][1] = (CImage*)CGame::GetResource(15005 + ii*2); // アクティブ
		int dmyY;
		GetGraphSize(m_selectItem[ii][0]->m_iamge, &m_selectItemSizeWigth[ii], &dmyY);
		if (m_selectItemSizeWigth[ii] > m_selectItemSizeMaxWigth) {
			m_selectItemSizeMaxWigth = m_selectItemSizeWigth[ii];
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
		GetGraphSize(m_playStandby_selectItem[ii][0]->m_iamge, &m_playStandby_selectItemSizeWigth[ii], &dmyY);
		if (m_playStandby_selectItemSizeWigth[ii] > m_playStandby_selectItemSizeMaxWigth) {
			m_playStandby_selectItemSizeMaxWigth = m_playStandby_selectItemSizeWigth[ii];
		}
	}

}


void CTitleScene::Main(CInputAllStatus *input){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	////フェード中でなければ
	//if(!NowFeed()){
	//	//入力参照
	//	if(input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH){
	//		SetFeedOut(120);
	//		SetBackScene();
	//	}
	//}

	if(m_status == Status::Top){
		Action(input);
	}
	Draw();

	if (m_status == Status::PlayStandby) {
		Action_PlayStandby(input);
		Draw_PlayStandby();
	}
}


void CTitleScene::Draw()
{
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.5), CGame::ToAllSizeY(0.5)), 1.0, 0, m_background->m_iamge);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(1.0), CGame::ToAllSizeY(1.0)), 1.0, 0, m_planet1->m_iamge);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.1), CGame::ToAllSizeY(0.85)), 0.2, 0, m_planet2->m_iamge);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.7), CGame::ToAllSizeY(0.2)), 0.1, 0, m_planet3->m_iamge);

	constexpr int TitleAnimeTime1 = 60;
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

		//m_titleLog3Size -= (1.0 / TitleAnimeTime1);
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

			int x = CGame::ToAllSizeX(0.7) - (m_selectItemSizeMaxWigth - m_selectItemSizeWigth[ii])/2;
			int y = CGame::ToAllSizeY(0.45 + 0.1 * (double)ii);
			CDxFunc::MyDrawRotaGraph(CPos(x,y ), size, 0.0, m_selectItem[ii][isAct]->m_iamge);
		}
	}
}
void CTitleScene::Action(CInputAllStatus* input)
{
	if (!m_top_animetionFinishFlag) {
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

		int x = CGame::ToAllSizeX(0.7) - (m_playStandby_selectItemSizeMaxWigth - m_playStandby_selectItemSizeWigth[ii]) / 2;
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

	}
	if (input->m_btnStatus[INPUT_DEF_CANCEL] == INPUT_PUSH) {
		m_selectIndex = 0;
		m_status = Status::Top;
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