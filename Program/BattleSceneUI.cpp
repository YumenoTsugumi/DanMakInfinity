#include "BattleSceneUI.h"
#include "Game.h"
#include "GameDefine.h"
#include "BattleScene.h"
CBattleSceneUI::CBattleSceneUI(){

}

CBattleSceneUI::~CBattleSceneUI(){

}

void CBattleSceneUI::Init() {
	// 内側
	m_textScore = (CImage*)CGame::GetResource(1150);
	m_textHiScore = (CImage*)CGame::GetResource(1151);
	m_rankWaku = (CImage*)CGame::GetResource(1152);
	m_rankWaku2 = (CImage*)CGame::GetResource(1154);
	m_text0g = (CImage*)CGame::GetResource(1110);
	m_textComma = (CImage*)CGame::GetResource(1111);
	m_textCommag = (CImage*)CGame::GetResource(1112);
	for (int ii = 0; ii < 10; ii++) {
		CImage* img = (CImage*)CGame::GetResource(1000 + ii);
		m_textNumber.push_back(img);
	}
	for (int ii = 0; ii < 10; ii++) {
		CImage* img = (CImage*)CGame::GetResource(1010 + ii);
		m_textBigNumber.push_back(img);
	}

	// 外側
	m_UIFoundation = (CImage*)CGame::GetResource(1500);
}
void CBattleSceneUI::Draw() {
	DrawGameAreaUI();
	DrawOutArea();
}

// 画面内のUI描画
void CBattleSceneUI::DrawGameAreaUI() {
	// UI関連
	long long hiScore = CBattleScene::GetHiScore();
	long long score = CBattleScene::GetScore();
	int rank = CBattleScene::GetRank();


	//score += rand()*20;
	//if (rand() < 300)rank++;
	//if(rank<999)rank++;

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//GameWindowAreaLeft
	//GameWindowAreaTop


	const int maxDrawDigit = 12; // 数値が無くても8桁までは表示する
	//--------------------------------------------------------------------------
	// Scoreの表示位置
	//--------------------------------------------------------------------------
	double textScore_x = GameWindowAreaLeft + 3;
	double textScore_y = GameWindowAreaTop + 3;
	CDxFunc::MyDrawGraph(textScore_x, textScore_y, m_textScore->m_iamge);
	double textScore_endX = textScore_x + m_textScore->m_sizeX;

	// スコアの値の表示
	int digit = CFunc::GetDigit(score); // 12345 なら 5
	
	// 12なら 3の倍数にしよう！
	// 210987654321
	// 210,987,654,321

	// 00123456 なら0から表示していく
	double commaGap = 0;
	for (int ii = maxDrawDigit; ii > 0 ; ii--) {
		CImage* drawImage;
		if (digit >= ii) {
			// 桁がある場合　数値を表示
			// 4444 で3桁目なら 4444 / 10^2 % 10
			long long number = (score / (long long)(pow(10, (ii-1)))) % 10;
			drawImage = m_textNumber[number];
		}
		else {
			// 桁が足りない場合　黒文字で0
			drawImage = m_text0g;
		}

		// スコアテキストの終点から　文字の幅*1~8
		int drawWigth = drawImage->m_sizeX * 0.65;
		double textNumber_x = textScore_endX + 10 + drawWigth * (maxDrawDigit - ii);
		double gap = (drawWigth - drawImage->m_sizeX) / 2;

		// 3の倍数の時は,を表示する
		if (ii % 3 == 0 && maxDrawDigit != ii) {
			if (digit >= ii+1) {
				CDxFunc::MyDrawGraph(textNumber_x + gap + commaGap, textScore_y, m_textComma->m_iamge);
			}
			else {
				CDxFunc::MyDrawGraph(textNumber_x + gap + commaGap, textScore_y, m_textCommag->m_iamge);
			}
			commaGap += m_textComma->m_sizeX * 0.5;
		}
		CDxFunc::MyDrawGraph(textNumber_x + gap + commaGap, textScore_y, drawImage->m_iamge);
	}
	
	//--------------------------------------------------------------------------
	// HiScoreの表示位置
	//--------------------------------------------------------------------------
	double textHiScore_x = GameWindowCenterX + 32 + 3 +15;
	CDxFunc::MyDrawGraph(textHiScore_x, textScore_y, m_textHiScore->m_iamge);
	double textHiScore_endX = textHiScore_x + m_textHiScore->m_sizeX;

	// スコアの値の表示
	int hiDigit = CFunc::GetDigit(hiScore); // 12345 なら 5

	double hiCommaGap = 0;
	for (int ii = maxDrawDigit; ii > 0; ii--) {
		CImage* drawImage;
		if (hiDigit >= ii) {
			// 桁がある場合　数値を表示
			// 4444 で3桁目なら 4444 / 10^2 % 10
			long long number = (hiScore / (long long)(pow(10, (ii - 1)))) % 10;
			drawImage = m_textNumber[number];
		}
		else {
			// 桁が足りない場合　黒文字で0
			drawImage = m_text0g;
		}

		// スコアテキストの終点から　文字の幅*1~8
		int drawWigth = drawImage->m_sizeX * 0.65;
		double textNumber_x = textHiScore_endX + 10 + drawWigth * (maxDrawDigit - ii);
		double gap = (drawWigth - drawImage->m_sizeX) / 2;

		// 3の倍数の時は,を表示する
		if (ii % 3 == 0 && maxDrawDigit != ii) {
			if (hiDigit >= ii) {
				CDxFunc::MyDrawGraph(textNumber_x + gap + hiCommaGap, textScore_y, m_textComma->m_iamge);
			}
			else {
				CDxFunc::MyDrawGraph(textNumber_x + gap + hiCommaGap, textScore_y, m_textCommag->m_iamge);
			}
			hiCommaGap += m_textComma->m_sizeX * 0.5;
		}
		CDxFunc::MyDrawGraph(textNumber_x + gap + hiCommaGap, textScore_y, drawImage->m_iamge);
	}

	//--------------------------------------------------------------------------
	// ランク
	//--------------------------------------------------------------------------
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 196);
	double rankPosX = GameWindowCenterX - m_rankWaku2->m_sizeX + 40+7;
	CDxFunc::MyDrawGraph(rankPosX, 20, m_rankWaku2->m_iamge);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (rank < 100) {
		int low = rank % 10;
		int high = rank / 10;
		int sx = m_textBigNumber[0]->m_sizeX;
		int sy = m_textBigNumber[0]->m_sizeY;
		double rankNumberY = 20 + (m_rankWaku2->m_sizeY - sy) / 2;
		CDxFunc::MyDrawGraph(rankPosX + 25, rankNumberY, m_textBigNumber[high]->m_iamge);
		CDxFunc::MyDrawGraph(rankPosX + 25 +sx*1.1, rankNumberY, m_textBigNumber[low]->m_iamge);
	}
	else {
		int low = rank % 10; // 987%10=7
		int middle = (rank  / 10) % 10; // 987/10 98%10=8
		int high = rank / 100; // 987/100=9

		int sx = m_textBigNumber[0]->m_sizeX;
		int sy = m_textBigNumber[0]->m_sizeY;
		double rankNumberY = 20 + (m_rankWaku2->m_sizeY - sy) / 2;
		CDxFunc::MyDrawGraph(rankPosX + 43 - sx * 0.95, rankNumberY, m_textBigNumber[high]->m_iamge);
		CDxFunc::MyDrawGraph(rankPosX + 43 + sx * 0, rankNumberY, m_textBigNumber[middle]->m_iamge);
		CDxFunc::MyDrawGraph(rankPosX + 43 + sx * 0.95, rankNumberY, m_textBigNumber[low]->m_iamge);
	}

}

// 画面外の描画
void CBattleSceneUI::DrawOutArea()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	CDxFunc::MyDrawGraph(0, 0, m_UIFoundation->m_iamge);
	
}




