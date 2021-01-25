#include "BattleSceneUI.h"
#include "Game.h"
#include "GameDefine.h"

CBattleSceneUI::CBattleSceneUI() :
	m_score(0),
	m_hiScore(0)
{

}

CBattleSceneUI::~CBattleSceneUI(){

}

void CBattleSceneUI::Init() {
	// 内側
	m_textScore = (CImage*)CGame::GetResource(1050);
	m_textHiScore = (CImage*)CGame::GetResource(1051);
	m_text0g = (CImage*)CGame::GetResource(1010);
	m_textComma = (CImage*)CGame::GetResource(1011);
	m_textCommag = (CImage*)CGame::GetResource(1012);
	m_numberMaxSize = CPos(0, 0);
	for (int ii = 0; ii < 10; ii++) {
		CImage* img = (CImage*)CGame::GetResource(1000 + ii);
		m_textNumber.push_back(img);

		if (img->m_sizeX > m_numberMaxSize.x) {
			m_numberMaxSize.x = img->m_sizeX;
		}
		if (img->m_sizeY > m_numberMaxSize.y) {
			m_numberMaxSize.y = img->m_sizeY;
		}
	}

	// 外側
	m_UIFoundation = (CImage*)CGame::GetResource(1500);

	m_score = 123456;
	m_hiScore = 123456789012;
}
void CBattleSceneUI::Draw() {
	DrawGameAreaUI();
	DrawOutArea();
}

// 画面内のUI描画
void CBattleSceneUI::DrawGameAreaUI() {
	m_score += rand()*20;
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
	int digit = CFunc::GetDigit(m_score); // 12345 なら 5
	
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
			long long number = (m_score / (long long)(pow(10, (ii-1)))) % 10;
			drawImage = m_textNumber[number];
		}
		else {
			// 桁が足りない場合　黒文字で0
			drawImage = m_text0g;
		}

		// スコアテキストの終点から　文字の幅*1~8
		int drawWigth = m_numberMaxSize.x * 0.65;
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
	double textHiScore_x = GameWindowCenterX + 32 + 3;
	CDxFunc::MyDrawGraph(textHiScore_x, textScore_y, m_textHiScore->m_iamge);
	double textHiScore_endX = textHiScore_x + m_textHiScore->m_sizeX;

	// スコアの値の表示
	int hiDigit = CFunc::GetDigit(m_hiScore); // 12345 なら 5

	double hiCommaGap = 0;
	for (int ii = maxDrawDigit; ii > 0; ii--) {
		CImage* drawImage;
		if (hiDigit >= ii) {
			// 桁がある場合　数値を表示
			// 4444 で3桁目なら 4444 / 10^2 % 10
			long long number = (m_hiScore / (long long)(pow(10, (ii - 1)))) % 10;
			drawImage = m_textNumber[number];
		}
		else {
			// 桁が足りない場合　黒文字で0
			drawImage = m_text0g;
		}

		// スコアテキストの終点から　文字の幅*1~8
		int drawWigth = m_numberMaxSize.x * 0.65;
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
}

// 画面外の描画
void CBattleSceneUI::DrawOutArea()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	CDxFunc::MyDrawGraph(0, 0, m_UIFoundation->m_iamge);
	
}




