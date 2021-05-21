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

		CImage* img4 = (CImage*)CGame::GetResource(1040 + ii);
		m_RankBigNumber.push_back(img4);
	}
	m_textScoreItem = (CImages*)CGame::GetResource(1030);

	// 外側
	m_UIFoundation = (CImage*)CGame::GetResource(1500);

	m_itemImage = (CBulletImage*)CGame::GetResource(20720); // アイテム
	m_shineImage = (CBulletImage*)CGame::GetResource(20721); // アイテムの輝き
	m_itemDrawSize1 = 1.0;
	m_itemDrawSize2 = 0.0;

	m_bombIconSize = 1.0;
	m_bombIconSize2 = 0.5;
	m_bombIcon = (CImage*)CGame::GetResource(987);
	m_bombText = (CImage*)CGame::GetResource(988);
	m_playerIcon = (CImage*)CGame::GetResource(800);
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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

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

	// 残機
	CPos lifeBaseRB = CGame::GetGamePosRB();
	CPos lifeBaseLT = CGame::GetGamePosLT();
	int life = CBattleScene::GetHaveLife();
	for (int ii = 0; ii < life; ii++) {
		CPos lifeP;
		lifeP.x = lifeBaseLT.x + 26 + 8 + ii * (46 + 6);
		lifeP.y = lifeBaseRB.y - 26 - 8;
		CDxFunc::MyDrawRotaGraph(lifeP, 0.5, 0, m_playerIcon->m_iamge);
	}

	// 残ボム
	int bomb = CBattleScene::GetHaveBomb();
	CPos bombBaseP = CGame::GetGamePosRB();
	for (int ii = 0; ii < bomb; ii++) {
		CPos bombP;
		bombP.x = bombBaseP.x - 26 - 8 - ii*(46+6);
		bombP.y = bombBaseP.y - 26 - 8;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		CDxFunc::MyDrawRotaGraph(bombP, 1.0, 0, m_bombIcon->m_iamge);
		SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
		CDxFunc::MyDrawRotaGraph(bombP, m_bombIconSize, 0, m_bombIcon->m_iamge);
		if (m_bombIconSize > 0)m_bombIconSize -= 0.005;
		if (m_bombIconSize < 0)m_bombIconSize = 1.0;
		CDxFunc::MyDrawRotaGraph(bombP, m_bombIconSize2, 0, m_bombIcon->m_iamge);
		if (m_bombIconSize2 > 0)m_bombIconSize2 -= 0.005;
		if (m_bombIconSize2 < 0)m_bombIconSize2 = 1.0;
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
		CDxFunc::MyDrawRotaGraph(bombP - CPos(0, 0), 1.0, 0, m_bombText->m_iamge);
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
	DrawItemGetCounter(GameWindowAreaRight + 40, 30, 85, 0.7, totalItemCount);

	// 得点アイテムの累計(3ランク)
	m_rotationAngle += CFunc::ToRad(m_itemImage->m_rotaSpeed);
	m_itemDrawSize1 += 0.02;
	if (m_itemDrawSize1 > 2.0)m_itemDrawSize1 = 0.0;
	m_itemDrawSize2 += 0.02;
	if (m_itemDrawSize2 > 2.0)m_itemDrawSize2 = 0.0;
	// 3
	double itemDrawSize = 1.0;
	CPos itemRank3 = CPos(GameWindowAreaRight + 40, 140);
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
		eff->SetScroll(false);
		CBattleScene::m_effectManager.Add(eff);
	}
	DrawItemGetCounter(GameWindowAreaRight + 40+40, 18, 140, 0.5, rank3ItemCount);



	CPos itemRank2 = CPos(GameWindowAreaRight + 40+40+100+10, 140);
	SetDrawBlendMode(DX_BLENDMODE_ADD, 192);
	CDxFunc::MyDrawRotaGraph(itemRank2, itemDrawSize * 2, m_rotationAngle, m_shineImage->m_images[0]);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(itemRank2, itemDrawSize, m_rotationAngle, m_itemImage->m_images[0]);
	DrawItemGetCounter(GameWindowAreaRight + 40 + 40 + 100+40, 20, 140, 0.5, rank2ItemCount);

	CPos itemRank1 = CPos(GameWindowAreaRight + 40 + 40 + 100 +10+ 40+100, 140);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::MyDrawRotaGraph(itemRank1, itemDrawSize, m_rotationAngle, m_itemImage->m_images[0]);
	DrawItemGetCounter(GameWindowAreaRight + 40 + 40 + 100 + 40 + 100+40, 20, 140, 0.5, rank1ItemCount);

	DrawOutArea_Rank();
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

void CBattleSceneUI::DrawOutArea_Rank() {
	// 画面外の描画rank
	CPos basePos = CPos(GameWindowAreaRight +25, 230);
	CPos basePosEd = basePos + CPos(430, 230);
	//DrawBoxAA(basePos.x, basePos.y, basePosEd.x, basePosEd.y, GetColor(255,255,255), FALSE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 96);
	int rank = CBattleScene::GetRank();
	if (rank < 100) {
		int rankNumberX = basePos.x + 115;
		double rankNumberY = 350;

		int low = rank % 10;
		int high = rank / 10;

		CDxFunc::MyDrawRotaGraph(rankNumberX, rankNumberY, 1.0, 0.0,		m_RankBigNumber[high]->m_iamge);
		CDxFunc::MyDrawRotaGraph(rankNumberX + 189, rankNumberY, 1.0, 0.0,	m_RankBigNumber[low]->m_iamge);
	}
	else {
		int rankNumberX = basePos.x + 115;
		double rankNumberY = 350;

		int low = rank % 10; // 987%10=7
		int middle = (rank / 10) % 10; // 987/10 98%10=8
		int high = rank / 100; // 987/100=9

		CDxFunc::MyDrawRotaGraph(rankNumberX -  33, rankNumberY, 0.7, 0.0, m_RankBigNumber[high]->m_iamge);
		CDxFunc::MyDrawRotaGraph(rankNumberX + 104, rankNumberY, 0.7, 0.0, m_RankBigNumber[middle]->m_iamge);
		CDxFunc::MyDrawRotaGraph(rankNumberX + 240, rankNumberY, 0.7, 0.0, m_RankBigNumber[low]->m_iamge);
	}

	//std::vector<double> lineY;
	//if (rank <= 7) {
	//	lineY = { 1,3,5,7,9 }; // 10
	//}
	//else if (rank <= 15) { // 20
	//	lineY = { 1,5,10,15,20 };
	//}
	//else if (rank <= 22) { // 30
	//	lineY = { 1,7,15,22,27 };
	//}
	//else if (rank <= 50) {
	//	lineY = { 5,15,25,35,45 };
	//}
	//else if (rank <= 100) {
	//	lineY = { 1,25,50,75,100 };
	//}
	double lineY[5];
	lineY[0] = 1;
	lineY[1] = 25;
	lineY[2] = 50;
	lineY[3] = 75;
	lineY[4] = 100;

	

	int bottomLineY = basePosEd.y - 2; // [0]
	DrawLineAA(basePos.x, bottomLineY, basePosEd.x,  bottomLineY, GetColor(255, 255, 255));

	int centerLineY = basePos.y + (basePosEd.y - basePos.y) / 2; // [2]
	DrawLineAA(basePos.x, centerLineY, basePosEd.x,  centerLineY, GetColor(255, 255, 255));

	int topLineY = basePos.y + 2; // [4]
	DrawLineAA(basePos.x, topLineY, basePosEd.x,  topLineY, GetColor(255, 255, 255));


	int bottomcenterLineY = bottomLineY + (centerLineY - bottomLineY)/2; // [1]
	DrawLineAA(basePos.x, bottomcenterLineY, basePosEd.x,  bottomcenterLineY, GetColor(255, 255, 255));

	int centertopLineY = centerLineY + (topLineY - centerLineY) / 2; // [3]
	DrawLineAA(basePos.x, centertopLineY, basePosEd.x,  centertopLineY, GetColor(255, 255, 255));


	static int aaa = 0;
	if (aaa == 0) {
		SetFontSize(16);
		aaa++;
	}
	int stringX = basePosEd.x - 16 - 4;
	int stringGosaY = 6;

#if 0
	{
		int nowDrawRangeMin = 1;
		int nowDrawRangeMax = 1000;

		for (int ii = 0; ii <= 999; ii += 10) {
			if (rank <= ii) {
				nowDrawRangeMin = 0;
				nowDrawRangeMax = ii * 2;
				if (nowDrawRangeMax >= 1000)nowDrawRangeMax = 1000;
				break;
			}
		}

		char tempStr1[80], tempStr2[80], tempStr3[80], tempStr4[80], tempStr5[80];
		int range = nowDrawRangeMax / 4;
		sprintf_s(tempStr1, "%00d", nowDrawRangeMin);
		sprintf_s(tempStr2, "%00d", range * 1);
		sprintf_s(tempStr3, "%00d", range * 2);
		sprintf_s(tempStr4, "%00d", range * 3);
		sprintf_s(tempStr5, "%00d", nowDrawRangeMax);
		DrawString(stringX, bottomLineY - stringGosaY, tempStr1, GetColor(255, 255, 255));
		DrawString(stringX, bottomcenterLineY - stringGosaY, tempStr2, GetColor(255, 255, 255));
		DrawString(stringX, centerLineY - stringGosaY, tempStr3, GetColor(255, 255, 255));
		DrawString(stringX, centertopLineY - stringGosaY, tempStr4, GetColor(255, 255, 255));
		DrawString(stringX, topLineY - stringGosaY, tempStr5, GetColor(255, 255, 255));

		double nowRankHeight = 1.0 - (double)rank / (nowDrawRangeMax); // ここたぶんあれ

		int centerX = basePos.x + (basePosEd.x - basePos.x) / 2;
		int centerY = basePos.y + (basePosEd.y - basePos.y) * nowRankHeight;

		// 現地点
		static double animeSize = 1.0;
		animeSize += 0.1;
		if (animeSize > 5.0)animeSize = 0.5;
		DrawCircle(centerX, centerY, 5 * animeSize, GetColor(255, 0, 0), FALSE);
		DrawCircle(centerX, centerY, 5, GetColor(255, 0, 0), TRUE);
	}
#endif

	int drawRangeMin = 1;
	int drawRangeMax = 1000;

	static int nowDrawRangeMax = 1000;
	static int targetDrawRangeMax = 1000;
	for (int ii = 0; ii <= 999; ii += 10) {
		if (rank <= ii) {
			drawRangeMin = 0;
			drawRangeMax = ii * 2;
			if (drawRangeMax >= 1000)drawRangeMax = 1000;
			break;
		}
	}
	double finalMax;
	bool anime = false;
	static int once = false;
	if (nowDrawRangeMax != drawRangeMax && !anime && once) {
		targetDrawRangeMax = drawRangeMax;
		anime = true;
	}
	static int chageCount = 0;
	once = true;
	if (anime) {
		constexpr int AnimeTime = 30;
		static int count = 0;
		if (count++ > AnimeTime) {
			anime = false;
			count = 0;
			chageCount = 0;
			nowDrawRangeMax = targetDrawRangeMax;
		}
		chageCount += 1;
		finalMax = nowDrawRangeMax + chageCount;
	}
	else {
		finalMax = drawRangeMax;
		nowDrawRangeMax = drawRangeMax;
	}
	//nowDrawRangeMax = finalMax;
	

	char tempStr1[80], tempStr2[80], tempStr3[80], tempStr4[80], tempStr5[80];
	int range = finalMax / 4;
	sprintf_s(tempStr1, "%00d", drawRangeMin);
	sprintf_s(tempStr2, "%00d", range * 1);
	sprintf_s(tempStr3, "%00d", range * 2);
	sprintf_s(tempStr4, "%00d", range * 3);
	sprintf_s(tempStr5, "%00d", finalMax);
	DrawString(stringX, bottomLineY - stringGosaY, tempStr1, GetColor(255, 255, 255));
	DrawString(stringX, bottomcenterLineY - stringGosaY, tempStr2, GetColor(255, 255, 255));
	DrawString(stringX, centerLineY - stringGosaY, tempStr3, GetColor(255, 255, 255));
	DrawString(stringX, centertopLineY - stringGosaY, tempStr4, GetColor(255, 255, 255));
	DrawString(stringX, topLineY - stringGosaY, tempStr5, GetColor(255, 255, 255));

	double nowRankHeight = 1.0 - (double)rank / (finalMax);

	int centerX = basePos.x + (basePosEd.x - basePos.x) / 2;
	int centerY = basePos.y + (basePosEd.y - basePos.y) * nowRankHeight;

	// 現地点
	static double animeSize = 1.0;
	animeSize += 0.1;
	if (animeSize > 5.0)animeSize = 0.5;
	DrawCircle(centerX, centerY, 5 * animeSize, GetColor(255, 0, 0), FALSE);
	DrawCircle(centerX, centerY, 5, GetColor(255, 0, 0), TRUE);

}

