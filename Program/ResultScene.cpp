#include "ResultScene.h"
#include "Player.h"
#include "Game.h"
#include "BaseBullet.h"
#include "CustomBullet.h"
#include "HomingBullet.h"
#include "BaseEffect.h"
#include "EnemyL.h"

CResultScene::CResultScene(int InTime) :
	CScene(InTime)
{
	//シーン,	フェードイン時間60, フェードアウト時間60, 
	//			フェードイン濃淡0.0, フェードアウト濃淡0.0
	Set(InTime);
}

CResultScene::~CResultScene(){

}

void CResultScene::Init(CGame* gameP) {
	m_gameOver = (CImage*)CGame::GetResource(15081);
	m_gameOverAlpha = 0.0;
	m_phase1_count = 0;

	for (int ii = 0; ii < 10; ii++) {
		m_strScoreImage[ii][0] = (CImage*)CGame::GetResource(15044 + ii);
		m_strScoreImage[ii][1] = (CImage*)CGame::GetResource(15054 + ii);

		if (m_strScoreImage[ii][0]->m_sizeX > m_strScoreImageMaxX) {
			m_strScoreImageMaxX = m_strScoreImage[ii][0]->m_sizeX;
		}
		if (m_strScoreImage[ii][0]->m_sizeY > m_strScoreImageMaxY) {
			m_strScoreImageMaxY = m_strScoreImage[ii][0]->m_sizeY;
		}
	}

	for (int ii = 0; ii < NameEntryFontMax; ii++) {
		m_strImage[ii][0] = (CImage*)CGame::GetResource(16000 + ii);
		m_strImage[ii][1] = (CImage*)CGame::GetResource(16050 + ii);

		if (m_strImage[ii][0]->m_sizeX > m_strImageMaxX) {
			m_strImageMaxX = m_strImage[ii][0]->m_sizeX;
		}
		if (m_strImage[ii][0]->m_sizeY > m_strImageMaxY) {
			m_strImageMaxY = m_strImage[ii][0]->m_sizeY;
		}
	}

	//            0          10        20       30
	//            01234567890123456789012345678901234567
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
	for (int ii = 0; ii < 10; ii++) {
		m_numberImage[ii] = m_strImage[ii + 26][0];
	}

	m_imageEnd = (CImage*)CGame::GetResource(15082);
	m_imageEndRank = (CImage*)CGame::GetResource(15083);
	m_imageItem = (CImage*)CGame::GetResource(15084);
	m_imageLiveTime = (CImage*)CGame::GetResource(15085);
	m_imageName = (CImage*)CGame::GetResource(15086);
	m_imageScore = (CImage*)CGame::GetResource(15087);
	m_imageSmall_s = (CImage*)CGame::GetResource(15088);
	m_imageStage = (CImage*)CGame::GetResource(15089);
	m_imageStartRank = (CImage*)CGame::GetResource(15090);
	m_imageEndA = (CImage*)CGame::GetResource(15091);

	m_imageRapid = (CImage*)CGame::GetResource(15028);
	m_imageSlow = (CImage*)CGame::GetResource(15029);
	m_imageShotType = (CImage*)CGame::GetResource(15032);
	m_imageMoveSpeed = (CImage*)CGame::GetResource(15030);
	m_imageShots[0] = (CImage*)CGame::GetResource(15036);
	m_imageShots[1] = (CImage*)CGame::GetResource(15038);
	m_imageShots[2] = (CImage*)CGame::GetResource(15040);
	m_imageShots[3] = (CImage*)CGame::GetResource(15042);

	m_imageArrowA = (CImage*)CGame::GetResource(15026);
	m_imageArrowB = (CImage*)CGame::GetResource(15027);

	m_imagePlanet = (CImage*)CGame::GetResource("bg_planet_09");
	m_imageSpace = (CImage*)CGame::GetResource("X_bg_space_04");

	m_startRank = 0;
	m_endRank = 0;
	m_stage = 0;
	m_item = 0;
	m_liveTime = 0;
	m_rapidShot = 0;
	m_rapidspeed = 0;
	m_slowShot = 0;
	m_slowspeed = 0;
	m_score = 0;

	m_nameEntryIndex = 0;
	for (int ii = 0; ii < nameKeta;ii++) {
		m_nameEntry[ii] = keyInputCount; // 空白
	}
	for (int ii = 0; ii < (keyInputCount); ii++) {
		m_keyInputCount[ii] = 0;
	}
}

void CResultScene::SetData(
	int startRank, int endRank,
	int stage, int item, int liveTime,
	int rapidShot, int rapidspeed, int slowShot, int slowspeed, long long score
) {
	m_startRank= startRank;
	m_endRank = endRank;
	m_stage = stage;
	m_item = item;
	m_liveTime = liveTime;
	m_rapidShot = rapidShot;
	m_rapidspeed = rapidspeed;
	m_slowShot = slowShot;
	m_slowspeed = slowspeed;
	m_score = score;
}


void CResultScene::Main(CInputAllStatus *input){
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255 * m_gameOverAlpha);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.5), CGame::ToAllSizeY(0.5)), 1.0, 0, m_gameOver->m_iamge);
	
	constexpr int Phase1Time = 30;
	m_phase1_count++;
	if (m_phase1_count < Phase1Time) {
		m_gameOverAlpha += 1.0 / (double)Phase1Time;
	}

	constexpr int Phase2Time = 120 + Phase1Time;
	if (m_phase1_count < Phase2Time && m_phase1_count > Phase1Time) {

	}

	constexpr int Phase3Time = 60 + Phase2Time;
	if (m_phase1_count < Phase3Time && m_phase1_count > Phase2Time) {
		m_gameOverAlpha -= 1.0 / (double)60;
		if (m_gameOverAlpha < 0)m_gameOverAlpha = 0;
	}

	constexpr int Phase4Time = 60 + Phase3Time;
	if (m_phase1_count > Phase4Time) {
		Draw2();
	}

	if (input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH) {
		if (m_phase1_count < Phase3Time) {
			m_gameOverAlpha = 1.0;
			m_phase1_count = Phase3Time;
		}
		else {
			m_gameOverAlpha = 0.0;
			m_phase1_count = Phase4Time;
		}
	}

	if (m_phase1_count > Phase4Time) {
		const int key[keyInputCount] = {
			INPUT_KEY_A,INPUT_KEY_B,INPUT_KEY_C,INPUT_KEY_D,INPUT_KEY_E,
			INPUT_KEY_F,INPUT_KEY_G,INPUT_KEY_H,INPUT_KEY_I,INPUT_KEY_J,
			INPUT_KEY_K,INPUT_KEY_L,INPUT_KEY_M,INPUT_KEY_N,INPUT_KEY_O,
			INPUT_KEY_P,INPUT_KEY_Q,INPUT_KEY_R,INPUT_KEY_S,INPUT_KEY_T,
			INPUT_KEY_U,INPUT_KEY_V,INPUT_KEY_W,INPUT_KEY_X,INPUT_KEY_Y,
			INPUT_KEY_Z,
			INPUT_KEY_0,INPUT_KEY_1,INPUT_KEY_2,INPUT_KEY_3,INPUT_KEY_4,
			INPUT_KEY_5,INPUT_KEY_6,INPUT_KEY_7,INPUT_KEY_8,INPUT_KEY_9,
			INPUT_KEY_SPACE
		};
		char keyState[256];
		GetHitKeyStateAll(keyState);
		for (int ii = 0; ii < (keyInputCount); ii++) {

			if (keyState[key[ii]] == 1 && m_keyInputCount[ii] >= 0) {
				m_keyInputCount[ii]++;

				if (m_keyInputCount[ii] == 1) {
					if (ii == keyInputCount - 1) {
						m_nameEntry[m_nameEntryIndex] = keyInputCount;
					}
					else {
						m_nameEntry[m_nameEntryIndex] = ii;
					}
					m_nameEntryIndex++;
					if (m_nameEntryIndex > nameKeta)m_nameEntryIndex = nameKeta;
				}
			}
			else {
				m_keyInputCount[ii] = 0;
			}
		}
	}
	if (input->m_btnStatus[INPUT_DEF_LEFT] == INPUT_PUSH) {
		m_nameEntryIndex--;
		if (m_nameEntryIndex < 0)m_nameEntryIndex = nameKetaEnd - 1;
	}
	if (input->m_btnStatus[INPUT_DEF_RIGHT] == INPUT_PUSH) {
		m_nameEntryIndex++;
		if (m_nameEntryIndex >= nameKetaEnd)m_nameEntryIndex = 0;
	}

	if (input->m_btnStatus[INPUT_DEF_UP] == INPUT_PUSH) {
		m_nameEntry[m_nameEntryIndex]--;
		if (m_nameEntry[m_nameEntryIndex] < 0)m_nameEntry[m_nameEntryIndex] = NameEntryFontMax_s - 1;
	}
	if (input->m_btnStatus[INPUT_DEF_DOWN] == INPUT_PUSH) {
		m_nameEntry[m_nameEntryIndex]++;
		if (m_nameEntry[m_nameEntryIndex] >= NameEntryFontMax_s)m_nameEntry[m_nameEntryIndex] = 0;
	}

	if (input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH) {
		if (m_nameEntryIndex == nameKetaEnd - 1) {

			time_t t = time(NULL);
			struct tm local;
			localtime_s(&local, &t);

			SaveDatus::SaveImage(local);

			char name[12];
			Conver_NameIndexToChar(name);
			SaveDatus::Save(m_startRank, m_endRank, m_stage, m_item, m_liveTime,
				m_rapidShot, m_rapidspeed, m_slowShot, m_slowspeed, m_score,
				local, name, 0);

			m_titleScene->SetReturnStatus_GoResult();
			SetFeedOut(0);
			SetBackScene();
		}
	}

}

void CResultScene::Conver_NameIndexToChar(char name[12])
{
	for (int ii = 0; ii < nameKeta; ii++) {
		name[ii] = m_nameEntry[ii];
	}
	name[nameKeta] = 0;
}

void CResultScene::Draw2()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.5), CGame::ToAllSizeY(0.5)), 1.0, 0, m_imageSpace->m_iamge);
	CDxFunc::MyDrawRotaGraph(CPos(CGame::ToAllSizeX(0.5), CGame::ToAllSizeY(0.5)), 1.0, 0, m_imagePlanet->m_iamge);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	DrawBox(CGame::ToAllSizeX(0.0), CGame::ToAllSizeY(0.15),
			CGame::ToAllSizeX(1.0), CGame::ToAllSizeY(0.85), GetColor(0, 0, 0), TRUE);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//--------------------------------------------------
	// スコア
	CDxFunc::MyDrawRotaGraph(CGame::ToAllSizeX(0.15), CGame::ToAllSizeY(0.3), 1.0, 0, m_imageScore->m_iamge);
	std::vector<int> scoreAry;
	CFunc::GetDigitArray2(m_score, scoreAry);

	int co = 0;
	for (int ii = 0; ii < scoreAry.size(); ii++) {
		int index = scoreAry[ii];
		if (index < 0 || index > 9) {
			continue;
		}
		double x = (co * 100) + CGame::ToAllSizeX(0.3);
		double y = CGame::ToAllSizeY(0.3);
		CDxFunc::MyDrawRotaGraph(x, y, 1.5, 0, m_strScoreImage[index][0]->m_iamge);
		co++;
	}
	
	//--------------------------------------------------
	// ランク
	double x1 = CGame::ToAllSizeX(0.15) + m_imageStartRank->m_sizeX/2;
	double x2 = CGame::ToAllSizeX(0.15) + m_imageEndRank->m_sizeX / 2;
	double y1 = CGame::ToAllSizeY(0.40);
	double y2 = CGame::ToAllSizeY(0.47);
	CDxFunc::MyDrawRotaGraph(x1, y1, 1.0, 0, m_imageStartRank->m_iamge);
	CDxFunc::MyDrawRotaGraph(x2, y2, 1.0, 0, m_imageEndRank->m_iamge);
	
	std::vector<int> s_rankAry, e_rankAry;
	CFunc::GetDigitArray2(m_startRank, s_rankAry);
	CFunc::GetDigitArray2(m_endRank, e_rankAry);
	double x12_a = CGame::ToAllSizeX(0.35);
	for (int ii = 0; ii < s_rankAry.size(); ii++) {
		CDxFunc::MyDrawRotaGraph(x12_a + ii *50, y1, 1.0, 0, m_numberImage[s_rankAry[ii]]->m_iamge);
	}
	for (int ii = 0; ii < e_rankAry.size(); ii++) {
		CDxFunc::MyDrawRotaGraph(x12_a + ii * 50, y2, 1.0, 0, m_numberImage[e_rankAry[ii]]->m_iamge);
	}

	//--------------------------------------------------
	// ステージ
	double x3 = CGame::ToAllSizeX(0.15) + m_imageStage->m_sizeX / 2;
	double x4 = CGame::ToAllSizeX(0.15) + m_imageItem->m_sizeX / 2;
	double x5 = CGame::ToAllSizeX(0.15) + m_imageLiveTime->m_sizeX / 2;
	double y3 = CGame::ToAllSizeY(0.60);
	double y4 = CGame::ToAllSizeY(0.67);
	double y5 = CGame::ToAllSizeY(0.74);
	CDxFunc::MyDrawRotaGraph(x3, y3, 1.0, 0, m_imageStage->m_iamge);
	CDxFunc::MyDrawRotaGraph(x4, y4, 1.0, 0, m_imageItem->m_iamge);
	CDxFunc::MyDrawRotaGraph(x5, y5, 1.0, 0, m_imageLiveTime->m_iamge);
	

	std::vector<int> stageAry, itemAry, liveTimeAry;
	CFunc::GetDigitArray2(m_stage, stageAry);
	CFunc::GetDigitArray2(m_item, itemAry);
	CFunc::GetDigitArray2(m_liveTime, liveTimeAry);
	double x345_a = CGame::ToAllSizeX(0.35);
	for (int ii = 0; ii < stageAry.size(); ii++) {
		CDxFunc::MyDrawRotaGraph(x345_a + ii * 50, y3, 1.0, 0, m_numberImage[stageAry[ii]]->m_iamge);
	}
	for (int ii = 0; ii < itemAry.size(); ii++) {
		CDxFunc::MyDrawRotaGraph(x345_a + ii * 50, y4, 1.0, 0, m_numberImage[itemAry[ii]]->m_iamge);
	}
	for (int ii = 0; ii < liveTimeAry.size(); ii++) {
		CDxFunc::MyDrawRotaGraph(x345_a + ii * 50, y5, 1.0, 0, m_numberImage[liveTimeAry[ii]]->m_iamge);
	}
	CDxFunc::MyDrawRotaGraph(x345_a + (liveTimeAry.size()) * 50, y5+5, 0.9, 0, m_imageSmall_s->m_iamge);


	//--------------------------------------------------
	// rapid
	double size = 0.6;
	double x6 = CGame::ToAllSizeX(0.5) + (m_imageRapid->m_sizeX * size) / 2;
	double x7 = CGame::ToAllSizeX(0.55) + (m_imageShotType->m_sizeX * size) / 2;
	double x8 = CGame::ToAllSizeX(0.55) + (m_imageMoveSpeed->m_sizeX * size) / 2;
	double y6 = CGame::ToAllSizeY(0.4);
	double y7 = CGame::ToAllSizeY(0.47);
	double y8 = CGame::ToAllSizeY(0.54);
	CDxFunc::MyDrawRotaGraph(x6, y6, size, 0, m_imageRapid->m_iamge);
	CDxFunc::MyDrawRotaGraph(x7, y7, size, 0, m_imageShotType->m_iamge);
	CDxFunc::MyDrawRotaGraph(x8, y8, size, 0, m_imageMoveSpeed->m_iamge);

	double x678_a = CGame::ToAllSizeX(0.75);
	CDxFunc::MyDrawRotaGraph(x678_a, y7, 0.75, 0, m_imageShots[m_rapidShot]->m_iamge);
	CDxFunc::MyDrawRotaGraph(x678_a, y8, 1.0, 0, m_numberImage[m_rapidspeed]->m_iamge);
	//--------------------------------------------------
	// slow
	double x9 = CGame::ToAllSizeX(0.5) + (m_imageSlow->m_sizeX * size) / 2;
	double x10 = CGame::ToAllSizeX(0.55) + (m_imageShotType->m_sizeX * size) / 2;
	double x11 = CGame::ToAllSizeX(0.55) + (m_imageMoveSpeed->m_sizeX * size) / 2;
	double y9 = CGame::ToAllSizeY(0.6);
	double y10 = CGame::ToAllSizeY(0.67);
	double y11 = CGame::ToAllSizeY(0.74);
	CDxFunc::MyDrawRotaGraph(x9, y9, size, 0, m_imageSlow->m_iamge);
	CDxFunc::MyDrawRotaGraph(x10, y10, size, 0, m_imageShotType->m_iamge);
	CDxFunc::MyDrawRotaGraph(x11, y11, size, 0, m_imageMoveSpeed->m_iamge);

	double x91011_a = CGame::ToAllSizeX(0.75);
	CDxFunc::MyDrawRotaGraph(x91011_a, y10, 0.75, 0, m_imageShots[m_slowShot]->m_iamge);
	CDxFunc::MyDrawRotaGraph(x91011_a, y11, 1.0, 0, m_numberImage[m_slowspeed]->m_iamge);
	//--------------------------------------------------
	// name
	double x12 = CGame::ToAllSizeX(0.15) + m_imageName->m_sizeX / 2;
	double y12 = CGame::ToAllSizeY(0.9);
	CDxFunc::MyDrawRotaGraph(x12, y12, 1.0, 0, m_imageName->m_iamge);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
	for (int ii = 0; ii < nameKeta; ii++) {
		double x13 = CGame::ToAllSizeX(0.4) + ii * 64;
		double y13 = CGame::ToAllSizeY(0.9);
		double xx_s = x13 - 24;
		double xx_e = x13 + 24;
		double yy_s = y13 - 24;
		double yy_e = y13 + 24;
		DrawBox(xx_s, yy_s, xx_e, yy_e, GetColor(0,0,0), TRUE);
		if (ii == m_nameEntryIndex) {
			CDxFunc::MyDrawRotaGraph(x13, y13 - 42, 0.5, CFunc::ToRad(90), m_imageArrowA->m_iamge);
			CDxFunc::MyDrawRotaGraph(x13, y13 + 42, 0.5, CFunc::ToRad(90), m_imageArrowB->m_iamge);
		}
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	for (int ii = 0; ii < nameKeta; ii++) {
		double x13 = CGame::ToAllSizeX(0.4) + ii*64;
		double y13 = CGame::ToAllSizeY(0.9);
		int isAct = 0;
		if (ii == m_nameEntryIndex)isAct = 1;
		int index = m_nameEntry[ii];
		bool space = false;
		if (index == NameEntryFontMax_s - 1) {
			space = true;// 空白
		}
		if (!space) {
			CImage* img = m_strImage[index][isAct];
			CDxFunc::MyDrawRotaGraph(x13, y13, 1.0, 0, img->m_iamge);
		}
	}

	double x13 = CGame::ToAllSizeX(0.728);
	double y13 = CGame::ToAllSizeY(0.9150);
	if (m_nameEntryIndex == nameKetaEnd - 1) {
		CDxFunc::MyDrawRotaGraph(x13, y13, 0.5, 0, m_imageEndA->m_iamge);
	}
	else {
		CDxFunc::MyDrawRotaGraph(x13, y13, 0.5, 0, m_imageEnd->m_iamge);
	}


}