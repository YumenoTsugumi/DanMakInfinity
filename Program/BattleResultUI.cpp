#include "BattleResultUI.h"
#include "Game.h"
#include "GameDefine.h"
#include "BattleScene.h"


constexpr int NumberSpanX = 39;

CBattleResultUI::CBattleResultUI(){

}

CBattleResultUI::~CBattleResultUI(){

}

void CBattleResultUI::Init() {
	m_count = 0;

	m_resultUI_A = (CImage*)CGame::GetResource(1201);
	m_resultUI_AA = (CImage*)CGame::GetResource(1202);
	m_resultUI_AB = (CImage*)CGame::GetResource(1203);
	m_resultUI_AC = (CImage*)CGame::GetResource(1204);
	m_resultUI_B = (CImage*)CGame::GetResource(1205);
	m_resultUI_C = (CImage*)CGame::GetResource(1206);
	m_resultUI_D = (CImage*)CGame::GetResource(1207);
	m_resultUI_E = (CImage*)CGame::GetResource(1208);
	m_resultUI_F = (CImage*)CGame::GetResource(1209);
	m_resultUI_N0 = (CImage*)CGame::GetResource(1210);
	m_resultUI_N1 = (CImage*)CGame::GetResource(1211);
	m_resultUI_N2 = (CImage*)CGame::GetResource(1212);
	m_resultUI_N3 = (CImage*)CGame::GetResource(1213);
	m_resultUI_N4 = (CImage*)CGame::GetResource(1214);
	m_resultUI_N5 = (CImage*)CGame::GetResource(1215);
	m_resultUI_N6 = (CImage*)CGame::GetResource(1216);
	m_resultUI_N7 = (CImage*)CGame::GetResource(1217);
	m_resultUI_N8 = (CImage*)CGame::GetResource(1218);
	m_resultUI_N9 = (CImage*)CGame::GetResource(1219);
	m_resultUI_Np = (CImage*)CGame::GetResource(1220);
	m_resultUI_Nc = (CImage*)CGame::GetResource(1221);
	m_resultUI_Nd = (CImage*)CGame::GetResource(1222);

	m_resultUI_rankS = (CImage*)CGame::GetResource(1250);
	m_resultUI_rankA = (CImage*)CGame::GetResource(1251);
	m_resultUI_rankB = (CImage*)CGame::GetResource(1252);
	m_resultUI_rankC = (CImage*)CGame::GetResource(1253);
	m_resultUI_rankD = (CImage*)CGame::GetResource(1254);
	m_resultUI_rankE = (CImage*)CGame::GetResource(1255);
	m_resultUI_uiaura = (CImage*)CGame::GetResource(1260);
}

void CBattleResultUI::Set(
	int destoryLargeEnemyRatio,
	int destoryMediumEnemyRatio,
	int destorySmallEnemyRatio,
	int usedBomb,
	int missCount,
	int getItemRatio,
	int gameResultIndex,
	int getScore,
	int getRank
){
	m_count = 0;
	status = BattleResultUIStatus::Now;
	m_resultRank = gameResultIndex;
	m_destoryLargeEnemyRatio  = destoryLargeEnemyRatio;
	m_destoryMediumEnemyRatio = destoryMediumEnemyRatio;
	m_destorySmallEnemyRatio = destorySmallEnemyRatio;
	m_usedBomb = usedBomb;
	m_missCount = missCount;
	m_getItemRatio = getItemRatio;
	m_getScore = getScore;
	m_getRank = getRank;

	CImage* drawImage[] = { m_resultUI_A,m_resultUI_AA,m_resultUI_AB,m_resultUI_AC, m_resultUI_B, m_resultUI_C, m_resultUI_D };
	for (int ii = 0; ii < draw1Count; ii++) {
		m_draw1Size[ii] = 2.0;
		m_draw1Alpha[ii] = 0;

		int dmyY;
		GetGraphSize(drawImage[ii]->m_iamge, &m_draw1ImageSizeX[ii], &dmyY);
	}
	m_drawDestoryLargeEnemyRatio = 0;
	m_drawDestoryMediumEnemyRatio = 0;
	m_drawDestorySmallEnemyRatio = 0;
	m_drawGetItemRatio = 0;
	m_drawGetScore = m_getScore;

	m_usedBombSize = 2.0;
	m_missCountSize = 2.0;
	m_usedBombAlpha = 0;
	m_missCountAlpha = 0;

	for (int ii = 0; ii < 4; ii++) {
		m_countupFinishEndFlag[ii] = false;
	}
	for (int ii = 0; ii < 4; ii++) {
		m_rankAuraAngle[ii] = false;
	}
	m_rankDrawSize = 3.0;
	m_rankDrawAlpha = 0;

	int dmyY;
	GetGraphSize(m_resultUI_E->m_iamge, &m_getScoreImageSizeX, &dmyY);
	m_getScoreDrawSize = 2.0;
	m_getScoreDrawAlpha = 0;


	m_allAlphaRatio = 1.0;
}

void CBattleResultUI::Draw(){
	m_count++;
	constexpr double smallInvTime = 20.0; // îZÇ≠è¨Ç≥Ç≠Ç»ÇÈÉAÉjÉÅÅ[ÉVÉáÉìÇÃéûä‘
	// ìGÇÃåÇîjó¶
	// 	   è¨
	// 	   íÜ
	// 	   ëÂ
	// égópÉ{ÉÄ
	// îÌíeêî
	double resultUI_x = GameWindowAreaLeft + 20;
	double resultUI_y = GameWindowAreaTop + 120;
	constexpr int span_y = 90;
	constexpr int draw1Span = 15;
	CImage* drawImage[] = { m_resultUI_A,m_resultUI_AA,m_resultUI_AB,m_resultUI_AC, m_resultUI_B, m_resultUI_C, m_resultUI_D };
	int drawTiming[] = { draw1Span * 1,draw1Span * 2,draw1Span * 3,draw1Span * 4,draw1Span * 5,draw1Span * 6,draw1Span * 7 };
	for (int ii = 0; ii < draw1Count; ii++) {
		if (m_count >= drawTiming[ii]) {
			double subX = 0;
			if (ii == 1 || ii == 2 || ii == 3)subX = 90;
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_draw1Alpha[ii] * m_allAlphaRatio);
			double posX = resultUI_x + subX + m_draw1ImageSizeX[ii] / 2;
			double posY = resultUI_y + ii * span_y;
			CDxFunc::MyDrawRotaGraph(posX, posY, m_draw1Size[ii], 0.0, drawImage[ii]->m_iamge);

			if (m_draw1Size[ii] > 1.0) {
				m_draw1Size[ii] -= 1.0 / smallInvTime;
			}
			if (m_draw1Alpha[ii] < 255) {
				m_draw1Alpha[ii] += 225.0 / smallInvTime;
			}
		}
	}
	
	constexpr int draw2Start = 120;
	constexpr int draw2SCountupTime = 60;
	constexpr int draw2SCountupSpan = 30;
	double addRatio = 10000/ draw2SCountupSpan;

	int timing1 = draw2Start + draw2SCountupSpan * 1;
	int timing2 = draw2Start + draw2SCountupTime * 1 + draw2SCountupSpan * 1;
	int timing3 = draw2Start + draw2SCountupTime * 2 + draw2SCountupSpan * 2;
	int timing4 = draw2Start + draw2SCountupTime * 3 + draw2SCountupSpan * 3;
	int timing5 = timing4 + draw2SCountupSpan;
	int timing6 = timing5 + draw2SCountupSpan;
	if (m_count >= timing1) {
		if (m_drawDestorySmallEnemyRatio <= m_destorySmallEnemyRatio && !m_countupFinishEndFlag[0]) {
			m_drawDestorySmallEnemyRatio += addRatio;
			if (m_drawDestorySmallEnemyRatio >= m_destorySmallEnemyRatio) {
				m_drawDestorySmallEnemyRatio = m_destorySmallEnemyRatio;
				if(m_count < timing2 - 10)m_count = timing2 - 10;
				m_countupFinishEndFlag[0] = true;
			}
		}
	}
	if (m_count >= timing2) {
		if (m_drawDestoryMediumEnemyRatio <= m_destoryMediumEnemyRatio && !m_countupFinishEndFlag[1]) {
			m_drawDestoryMediumEnemyRatio += addRatio;
			if (m_drawDestoryMediumEnemyRatio >= m_destoryMediumEnemyRatio) {
				m_drawDestoryMediumEnemyRatio = m_destoryMediumEnemyRatio;
				if (m_count < timing3 - 10)m_count = timing3 - 10;
				m_countupFinishEndFlag[1] = true;
			}
		}
	}
	if (m_count >= timing3) {
		if (m_drawDestoryLargeEnemyRatio <= m_destoryLargeEnemyRatio && !m_countupFinishEndFlag[2]) {
			m_drawDestoryLargeEnemyRatio += addRatio;
			if (m_drawDestoryLargeEnemyRatio >= m_destoryLargeEnemyRatio) {
				m_drawDestoryLargeEnemyRatio = m_destoryLargeEnemyRatio;
				if (m_count < timing4 - 10)m_count = timing4 - 10;
				m_countupFinishEndFlag[2] = true;
			}
		}
	}

	if (m_count >= timing4) {
		if (m_drawGetItemRatio <= m_getItemRatio && !m_countupFinishEndFlag[3]) {
			m_drawGetItemRatio += addRatio;
			if (m_drawGetItemRatio >= m_getItemRatio) {
				m_drawGetItemRatio = m_getItemRatio;
				if (m_count < timing5 - 10)m_count = timing5 - 10;
				m_countupFinishEndFlag[3] = true;
			}
		}
	}
	if (m_count >= draw2Start) {
		DrawDestoryRatio(m_drawDestorySmallEnemyRatio, resultUI_x + 800, resultUI_y + span_y * 1);
		DrawDestoryRatio(m_drawDestoryMediumEnemyRatio, resultUI_x + 800, resultUI_y + span_y * 2);
		DrawDestoryRatio(m_drawDestoryLargeEnemyRatio, resultUI_x + 800, resultUI_y + span_y * 3);
		DrawDestoryRatio(m_drawGetItemRatio, resultUI_x + 800, resultUI_y + span_y * 4);
	}


	CImage* drawNumberImage[] = {
		m_resultUI_N0,m_resultUI_N1,m_resultUI_N2,m_resultUI_N3,m_resultUI_N4,
		m_resultUI_N5,m_resultUI_N6,m_resultUI_N7,m_resultUI_N8,m_resultUI_N9,
		m_resultUI_Np,m_resultUI_Nd,
	};


	if (m_count >= timing5) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_usedBombAlpha * m_allAlphaRatio);
		CDxFunc::MyDrawRotaGraph(resultUI_x + 800, resultUI_y + span_y * 5, m_usedBombSize, 0.0, drawNumberImage[m_usedBomb]->m_iamge);
		if (m_usedBombSize > 1.0) {
			m_usedBombSize -= 1.0 / smallInvTime;
		}
		if (m_usedBombAlpha < 255) {
			m_usedBombAlpha += 255.0 / smallInvTime;
		}
	}
	if (m_count >= timing6) {
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_missCountAlpha * m_allAlphaRatio);
		CDxFunc::MyDrawRotaGraph(resultUI_x + 800, resultUI_y + span_y * 6, m_missCountSize, 0.0, drawNumberImage[m_missCount]->m_iamge);
		if (m_missCountSize > 1.0) {
			m_missCountSize -= 1.0 / smallInvTime;
		}
		if (m_missCountAlpha < 255) {
			m_missCountAlpha += 255.0 / smallInvTime;
		}
	}

	CImage* resultRankArray[6] = { m_resultUI_rankS	,m_resultUI_rankA	,m_resultUI_rankB	,m_resultUI_rankC	,m_resultUI_rankD	,m_resultUI_rankE };
	int timing7 = timing6 + draw2SCountupSpan;
	if (m_count >= timing7) {
		SetDrawBlendMode(DX_BLENDMODE_ADD, 96 * m_rankDrawAlpha/255.0 * m_allAlphaRatio);
		if (m_resultRank == 0 || m_resultRank == 1) { // S A
			CDxFunc::MyDrawRotaGraph(resultUI_x + 800 - 350, resultUI_y + span_y * 7 + 50, 0.5, m_rankAuraAngle[0], m_resultUI_uiaura->m_iamge);
			CDxFunc::MyDrawRotaGraph(resultUI_x + 800 - 350, resultUI_y + span_y * 7 + 50, 0.4, m_rankAuraAngle[1], m_resultUI_uiaura->m_iamge);
			CDxFunc::MyDrawRotaGraph(resultUI_x + 800 - 350, resultUI_y + span_y * 7 + 50, 0.5, m_rankAuraAngle[2], m_resultUI_uiaura->m_iamge);
			CDxFunc::MyDrawRotaGraph(resultUI_x + 800 - 350, resultUI_y + span_y * 7 + 50, 0.4, m_rankAuraAngle[3], m_resultUI_uiaura->m_iamge);
			m_rankAuraAngle[0] += 0.005;
			m_rankAuraAngle[1] += 0.0022;
			m_rankAuraAngle[2] -= 0.004;
			m_rankAuraAngle[3] -= 0.0013;
		} else { // B C D E
			CDxFunc::MyDrawRotaGraph(resultUI_x + 800 - 350, resultUI_y + span_y * 7 + 50, 0.5, m_rankAuraAngle[0], m_resultUI_uiaura->m_iamge);
			CDxFunc::MyDrawRotaGraph(resultUI_x + 800 - 350, resultUI_y + span_y * 7 + 50, 0.5, m_rankAuraAngle[2], m_resultUI_uiaura->m_iamge);
			m_rankAuraAngle[0] += 0.005;
			m_rankAuraAngle[2] -= 0.004;
		}

		if (m_rankDrawSize > 1.0) {
			m_rankDrawSize -= 1.0 / smallInvTime;
		}
		if (m_rankDrawAlpha < 255) {
			m_rankDrawAlpha += 255.0 / (smallInvTime*2);
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_rankDrawAlpha * m_allAlphaRatio);
		CDxFunc::MyDrawRotaGraph(resultUI_x + 800 - 350, resultUI_y + span_y * 7 + 50, m_rankDrawSize, 0.0, resultRankArray[m_resultRank]->m_iamge);
	}

	int timing8 = timing7 + draw2SCountupSpan * 2;
	int timing9 = timing8 + draw2SCountupSpan * 1;
	int timing10 = timing9 + draw2SCountupSpan * 2;
	int timing9CountDown = 120;
	double addCountDownRatio = (m_getScore / timing9CountDown + 3.0);
	if (m_count >= timing9) {
		
		if (m_getScoreDrawSize > 1.0) {
			m_getScoreDrawSize -= 1.0 / smallInvTime;
		}
		if (m_getScoreDrawAlpha < 255) {
			m_getScoreDrawAlpha += 255.0 / (smallInvTime * 2);
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_getScoreDrawAlpha * m_allAlphaRatio);
		double posX = resultUI_x + m_getScoreImageSizeX / 2;
		double posY = resultUI_y + 9 * span_y;
		CDxFunc::MyDrawRotaGraph(posX, posY, m_getScoreDrawSize, 0.0, m_resultUI_E->m_iamge);
	}
	if (m_count >= timing9) {
		DrawDigitNumber2(m_drawGetScore, resultUI_x + 800, resultUI_y + span_y * 9);
	}
	if (m_count >= timing10) {
		if (m_drawGetScore >= addCountDownRatio) {
			m_drawGetScore -= addCountDownRatio;
			CBattleScene::AddScore(addCountDownRatio);
		}
		else if (addCountDownRatio - m_drawGetScore > 0 && m_drawGetScore > 0) {
			CBattleScene::AddScore(m_drawGetScore);
			m_drawGetScore = 0;
		}
	}

	constexpr double timing11FeedOut = 20.0;
	int timing11 = timing10 + timing9CountDown + draw2SCountupSpan * 2;
	if (m_count >= timing11) {
		if (m_count == timing11) {
			status = FeedOut;
		}
		m_allAlphaRatio -= 1.0 / timing11FeedOut;
		
	}
	int timing12 = timing11 + draw2SCountupSpan * 1;
	if (m_count == timing11) {
		status = Finish;
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}


void CBattleResultUI::DrawDigitNumber2(int number, int posX, int posY)
{

	CImage* drawNumberImage[] = {
		m_resultUI_N0,m_resultUI_N1,m_resultUI_N2,m_resultUI_N3,m_resultUI_N4,
		m_resultUI_N5,m_resultUI_N6,m_resultUI_N7,m_resultUI_N8,m_resultUI_N9,
		m_resultUI_Np,m_resultUI_Nd,m_resultUI_Nc
	};

	std::vector<int> indexAry;
	CFunc::GetDigitArray(8, number, indexAry);

	for (int ii = 0; ii < indexAry.size(); ii++) {
		double x = posX - ii * NumberSpanX;
		double y = posY;
		CImage* drawImage = drawNumberImage[indexAry[ii]];
		CDxFunc::MyDrawRotaGraph(x, y, m_getScoreDrawSize, 0.0, drawImage->m_iamge);
	}
}

void CBattleResultUI::DrawDestoryRatio(int destoryEnemyRatio, int posX, int posY)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * m_allAlphaRatio);

	CImage* drawNumberImage[] = {
		m_resultUI_N0,m_resultUI_N1,m_resultUI_N2,m_resultUI_N3,m_resultUI_N4,
		m_resultUI_N5,m_resultUI_N6,m_resultUI_N7,m_resultUI_N8,m_resultUI_N9,
		m_resultUI_Np,m_resultUI_Nd,
	};

	std::vector<int> indexAry;
	int hiDigit = CFunc::GetDigit(destoryEnemyRatio);

	indexAry.push_back(10); // %
	if (destoryEnemyRatio == 0) {
		indexAry.push_back(0);
		indexAry.push_back(0);
		indexAry.push_back(11); // .
		indexAry.push_back(0);
	}
	else {
		if (hiDigit == 3) {
			indexAry.push_back(destoryEnemyRatio % 10);
			indexAry.push_back(destoryEnemyRatio  / 10 % 10);
			indexAry.push_back(11); // .
			indexAry.push_back(destoryEnemyRatio / 100);
		}
		else if (hiDigit == 4) {
			indexAry.push_back(destoryEnemyRatio % 10);
			indexAry.push_back(destoryEnemyRatio / 10 % 10);
			indexAry.push_back(11); // .
			indexAry.push_back(destoryEnemyRatio / 100 % 10);
			indexAry.push_back(destoryEnemyRatio / 1000);
		}
		else {
			indexAry.push_back(0);
			indexAry.push_back(0);
			indexAry.push_back(11); // .
			indexAry.push_back(0);
			indexAry.push_back(0);
			indexAry.push_back(1);
		}

	}

	for (int ii = 0; ii < indexAry.size(); ii++) {
		double x = posX - ii * NumberSpanX;
		double y = posY;
		CImage* drawImage = drawNumberImage[indexAry[ii]];
		CDxFunc::MyDrawRotaGraph(x, y, 1.0, 0.0, drawImage->m_iamge);
	}
	
}