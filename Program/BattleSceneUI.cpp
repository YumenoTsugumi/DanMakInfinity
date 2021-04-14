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
		CImage* img1 = (CImage*)CGame::GetResource(1000 + ii);
		m_textNumber.push_back(img1);

		CImage* img2 = (CImage*)CGame::GetResource(1010 + ii);
		m_textBigNumber.push_back(img2);

		CImage* img3 = (CImage*)CGame::GetResource(1020 + ii);
		m_textItemTakeNumber.push_back(img3);
	}
	m_textScoreItem = (CImages*)CGame::GetResource(1030);

	// 外側
	m_UIFoundation = (CImage*)CGame::GetResource(1500);

	m_itemImage = (CBulletImage*)CGame::GetResource(20720); // アイテム
	m_shineImage = (CBulletImage*)CGame::GetResource(20721); // アイテムの輝き
	m_itemDrawSize1 = 1.0;
	m_itemDrawSize2 = 0.0;
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
	
	// 得点アイテムの合計
	int totalItemCount, rank3ItemCount, rank2ItemCount, rank1ItemCount;
	CBattleScene::GetItemCount(totalItemCount, rank3ItemCount, rank2ItemCount, rank1ItemCount);
	DrawItemGetCounter(GameWindowAreaRight + 100, 28, 70, 0.7, totalItemCount);


	// 得点アイテムの累計(3ランク)
	m_rotationAngle += CFunc::ToRad(m_itemImage->m_rotaSpeed);
	m_itemDrawSize1 += 0.02;
	if (m_itemDrawSize1 > 2.0)m_itemDrawSize1 = 0.0;
	m_itemDrawSize2 += 0.02;
	if (m_itemDrawSize2 > 2.0)m_itemDrawSize2 = 0.0;
	// 3
	double itemDrawSize = 1.0;
	CPos itemRank3 = CPos(GameWindowAreaRight + 100, 140);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	CDxFunc::MyDrawRotaGraph(itemRank3, itemDrawSize * 2, m_rotationAngle, m_shineImage->m_images[0]);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(itemRank3, itemDrawSize, m_rotationAngle, m_itemImage->m_images[0]);
	{
		double ang = CFunc::RandF(0, 360);
		double speed = CFunc::RandD(1.0, 1.6);
		CBaseEffect* eff = new CBaseEffect(60, EDirType::Abs, itemRank3, speed, ang, 0, -0.2, 0.5, 0, 20721);
		eff->SetSize(0.2, +0.0);
		eff->SetWaitTime(CFunc::RandI(1, 15));
		eff->SetBlend(128, -1.0, 0);
		eff->SetBlendType(DX_BLENDMODE_ADD);
		eff->SetAnimeEndDelFlg(true);	//アニメーション終了後削除するか
		eff->SetRemoveCount(60);	//60frで削除
		CBattleScene::m_effectManager.Add(eff);
	}
	DrawItemGetCounter(GameWindowAreaRight + 160, 20, 140, 0.5, rank3ItemCount);



	CPos itemRank2 = CPos(GameWindowAreaRight + 100, 190);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 192);
	CDxFunc::MyDrawRotaGraph(itemRank2, itemDrawSize * 2, m_rotationAngle, m_shineImage->m_images[0]);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(itemRank2, itemDrawSize, m_rotationAngle, m_itemImage->m_images[0]);
	DrawItemGetCounter(GameWindowAreaRight + 160, 20, 190, 0.5, rank2ItemCount);

	CPos itemRank1 = CPos(GameWindowAreaRight + 100, 240);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(itemRank1, itemDrawSize, m_rotationAngle, m_itemImage->m_images[0]);
	DrawItemGetCounter(GameWindowAreaRight + 160, 20, 240, 0.5, rank1ItemCount);
}

void CBattleSceneUI::DrawItemGetCounter(double x, double xGap, double y, double size, int value)
{
	int digit = CFunc::GetDigit(value);
	if (digit == 0)digit = 1;
	for (int ii = 0; ii < digit; ii++) {
		int showValue = value / (int)(pow(10, (digit - ii - 1))) % 10;
		int drawImage = m_textScoreItem->m_images[showValue];
		CDxFunc::MyDrawRotaGraph(x + ii * xGap, y, size, 0, drawImage);
	}
}


