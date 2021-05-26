#include "BattleSceneUI.h"
#include "Game.h"
#include "GameDefine.h"
#include "BattleScene.h"

constexpr int updateTiming = 60*1;

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

	m_textItemTakeOther.push_back((CImage*)CGame::GetResource(1031));
	m_textItemTakeOther.push_back((CImage*)CGame::GetResource(1032));
	m_textItemTakeOther.push_back((CImage*)CGame::GetResource(1033));


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

	m_imageRankRatio = (CImage*)CGame::GetResource(1034);

	// ステージの番号の表示
	for (int ii = 0; ii < 10; ii++) {
		CImage* img1 = (CImage*)CGame::GetResource(1050 + ii);
		m_stageNumber.push_back(img1);
	}
	m_stageLogo = (CImage*)CGame::GetResource(1060);


	m_rankAnime_PreRank = 1000; // ランクアニメ）アニメが始まった時のランクの値
	m_rankAnime_TargetRank = 1000; // ランクアニメ）最終的なランクの値
	m_rankAnime_UpRank = 0; // ランクアニメ）アニメ中に上がっているランクを記録する
	m_rankAnime_Once = false; // ランクアニメ）m_rankAnime_PreRankの設定を最初の一度
	m_rankAnime_Count = 0; // ランクアニメ）アニメーションを制御するためのカウンター
	m_rankAnime_WaveAnimeSize = 1.0; // ランクアニメ）原点の波紋用のアニメ
	m_rankAnime_UpdateCount = 0;
	m_rankAnime_RankUpDown = 0;
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
	DrawOutArea_Stage();
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

	//-----------------------------------------------------
	// 背景のランク
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

	int gosaX = 40;
	int bottomLineY = basePosEd.y - 2; // [0]
	DrawLineAA(basePos.x, bottomLineY, basePosEd.x - gosaX, bottomLineY, GetColor(255, 255, 255));
	int centerLineY = basePos.y + (basePosEd.y - basePos.y) / 2; // [2]
	DrawLineAA(basePos.x, centerLineY, basePosEd.x - gosaX, centerLineY, GetColor(255, 255, 255));
	int topLineY = basePos.y + 2; // [4]
	DrawLineAA(basePos.x, topLineY, basePosEd.x - gosaX, topLineY, GetColor(255, 255, 255));
	int bottomcenterLineY = bottomLineY + (centerLineY - bottomLineY)/2; // [1]
	DrawLineAA(basePos.x, bottomcenterLineY, basePosEd.x - gosaX, bottomcenterLineY, GetColor(255, 255, 255));
	int centertopLineY = centerLineY + (topLineY - centerLineY) / 2; // [3]
	DrawLineAA(basePos.x, centertopLineY, basePosEd.x - gosaX, centertopLineY, GetColor(255, 255, 255));


	int drawRangeMin = 1;
	int drawRangeMax = 1000;

	// 現状のランクの幅を計算　10　なら　0～20
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
	
	if (m_rankAnime_PreRank != drawRangeMax && !anime && m_rankAnime_Once) {
		m_rankAnime_TargetRank = drawRangeMax;
		anime = true;
		if (m_rankAnime_PreRank < drawRangeMax) {
			m_rankAnime_RankUpDown = +1;
		}
		else {
			m_rankAnime_RankUpDown = -1;
		}
	}
	
	m_rankAnime_Once = true;
	if (anime) {
		constexpr int AnimeTime = 30;
		
		if (m_rankAnime_Count++ > AnimeTime) {
			anime = false;
			m_rankAnime_Count = 0;
			m_rankAnime_UpRank = 0;
			m_rankAnime_PreRank = m_rankAnime_TargetRank;
		}
		m_rankAnime_UpRank += m_rankAnime_RankUpDown;
		finalMax = m_rankAnime_PreRank + m_rankAnime_UpRank;
		if (m_rankAnime_RankUpDown > 0 && finalMax > m_rankAnime_TargetRank) { // +方向なら
			finalMax = m_rankAnime_TargetRank;
		}
		if (m_rankAnime_RankUpDown < 0 && finalMax < m_rankAnime_TargetRank) { // -方向なら
			finalMax = m_rankAnime_TargetRank;
		}

	}
	else {
		finalMax = drawRangeMax;
		m_rankAnime_PreRank = drawRangeMax;
	}	

	// 各線とか文字の描写
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	int range = finalMax / 4;
	auto RankDraw = [&](double x, double y, std::vector<int> indexAry) {
		for (int ii = 0; ii < indexAry.size(); ii++) {
			CDxFunc::MyDrawRotaGraph(x - ii*12+20, y+5, 0.30, 0, m_textScoreItem->m_images[indexAry[ii]]);
		}
	};
	int stringX = basePosEd.x - 16 - 4;
	int stringGosaY = 6;
	std::vector<int> indexAry1, indexAry2, indexAry3, indexAry4, indexAry5;
	CFunc::GetDigitArray(3, drawRangeMin, indexAry1);
	RankDraw(stringX, bottomLineY - stringGosaY, indexAry1);

	CFunc::GetDigitArray(3, range * 1, indexAry2);
	RankDraw(stringX, bottomcenterLineY - stringGosaY, indexAry2);

	CFunc::GetDigitArray(3, range * 2, indexAry3);
	RankDraw(stringX, centerLineY - stringGosaY, indexAry3);

	CFunc::GetDigitArray(3, range * 3, indexAry4);
	RankDraw(stringX, centertopLineY - stringGosaY, indexAry4);

	CFunc::GetDigitArray(3, (int)finalMax, indexAry5);
	RankDraw(stringX, topLineY - stringGosaY, indexAry5);

	m_rankAnime_UpdateCount++;
	if (m_rankAnime_UpdateCount % updateTiming == 0) {
		m_rankAnime_KeepRank.push_back(rank);
		if (m_rankAnime_KeepRank.size() > 30) {
			m_rankAnime_KeepRank.erase(m_rankAnime_KeepRank.begin());
		}
	}

	// ランク上昇
	CDxFunc::MyDrawRotaGraph(basePos.x + 80, basePos.y + 287, 0.40, 0, m_imageRankRatio->m_iamge);

	double rankRatio = CBattleScene::GetRankRatio(); // 1.0   0.5～10.0    100.00%
	int rankRatioPer = rankRatio * 100;
	std::vector<int> ratioIndexAry;
	CFunc::GetDigitArray(3, rankRatioPer, ratioIndexAry);
	for (int ii = 0; ii < ratioIndexAry.size(); ii++) {
		CDxFunc::MyDrawRotaGraph(basePos.x + 385 - ii*20, basePos.y + 287, 0.50, 0, m_textScoreItem->m_images[ratioIndexAry[ii]]);
	}
	CDxFunc::MyDrawRotaGraph(basePos.x + 385 + 30, basePos.y + 287, 0.50, 0, m_textItemTakeOther[0]->m_iamge);


	// ランクアニメーション
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	int relX = 0;
	double relY = 0;
	double preX = basePos.x;
	double preY = basePos.y + (basePosEd.y - basePos.y);
	for (int ii = 0; ii < m_rankAnime_KeepRank.size(); ii++) {
		int preRank = m_rankAnime_KeepRank[ii];
		relX = (ii+1) * 10;
		relY = 1.0 - (double)preRank / (finalMax);
		if (relY <= 0.0) {
			relY = 0.0;
		}

		double postX = basePos.x + relX;
		double postY = basePos.y + (basePosEd.y - basePos.y) * relY;
		if (ii == 0) {
			preX = postX;
			preY = postY;
		}

		DrawLineAA(preX, preY, postX, postY, GetColor(255, 0, 0), 3);
		
		DrawCircleAA(postX, postY, 4, GetColor(255, 0, 0), TRUE);
		preX = postX;
		preY = postY;

		if (ii == m_rankAnime_KeepRank.size() - 1) {
			SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
			// 現地点
			double nowRankHeight = 1.0 - (double)rank / (finalMax);
			int centerX = basePos.x + (basePosEd.x - basePos.x) / 2;
			int centerY = basePos.y + (basePosEd.y - basePos.y) * nowRankHeight;
			m_rankAnime_WaveAnimeSize += 0.1;
			if (m_rankAnime_WaveAnimeSize > 5.0)m_rankAnime_WaveAnimeSize = 0.5;
			DrawCircle(postX, postY, 5 * m_rankAnime_WaveAnimeSize, GetColor(255, 0, 0), FALSE);
			DrawCircle(postX, postY, 5, GetColor(255, 0, 0), TRUE);
		}
	}


}


// 画面外　現在のステージ
void CBattleSceneUI::DrawOutArea_Stage()
{
	// 画面外の描画rank
	CPos basePos = CPos(GameWindowAreaRight + 25, 630);
	CPos basePosEd = basePos + CPos(430, 230);
	
	int nowStage = CBattleScene::GetNowStage();
	std::vector<int> stageAry1;
	CFunc::GetDigitArray(3, nowStage, stageAry1);

	
	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	CDxFunc::MyDrawRotaGraph(basePos.x + 180, basePos.y + 290, 1.205, 0, m_stageLogo->m_iamge);
	for (int ii = 0; ii < stageAry1.size(); ii++) {
		CDxFunc::MyDrawRotaGraph(basePos.x + 380 - ii * 70, basePos.y + 370, 1.205, 0, m_stageNumber[stageAry1[ii]]->m_iamge);
	}

}
