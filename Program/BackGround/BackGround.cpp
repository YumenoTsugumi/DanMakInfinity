
#include "BackGround.h"
#include "BattleScene.h"
#include "Game.h"



CBackGround::CBackGround() : m_effectManager(512){
}
CBackGround::~CBackGround() {
}
void CBackGround::Action() {
}
void CBackGround::Draw() {
}


char fileNameSpaceBG[8][20] = {
	"X_bg_space_00",	"X_bg_space_01",	"X_bg_space_02",	"X_bg_space_03",
	"X_bg_space_04",	"X_bg_space_05",	"X_bg_space_06",	"X_bg_space_07"
};
// "X_bg_space_08",見ずらい　 "X_bg_space_09" ボスっぽい
char fileNameSpaceBase[6][20] = {
	"X_bg_space_b00",	"X_bg_space_b01",	"X_bg_space_b02",
	"X_bg_space_b03",	"X_bg_space_b04",	"X_bg_space_b05"
};
char fileNamePlanet[8][20] = {
	"bg_planet_00",	"bg_planet_01",	"bg_planet_02",	"bg_planet_03",	"bg_planet_04",	
	"bg_planet_05",	"bg_planet_06", "bg_planet_09"
};

std::vector<int> CBackGroundPatternA::m_usedBgIndex;
std::vector<int> CBackGroundPatternA::m_usedPlanetIndex;

CBackGroundPatternA::CBackGroundPatternA()
{
	m_usedBgIndex.clear();
	m_usedPlanetIndex.clear();
}
CBackGroundPatternA::~CBackGroundPatternA()
{
}

void CBackGroundPatternA::Set()
{
	// 今まで選ばれていないインデックスを抽出する(雑コード
	do {
		m_bgIndex = CFunc::RandI(0, 7);

		bool dupFlag = false;
		for (auto index : m_usedBgIndex) {
			if (index == m_bgIndex) {
				dupFlag = true;
				break;
			}
		}
		if (!dupFlag) {
			break;
		}
	} while (1);
	m_usedBgIndex.push_back(m_bgIndex);
	if (m_usedBgIndex.size() >= 8) {
		m_usedBgIndex.clear();
		m_usedBgIndex.push_back(m_bgIndex);
	}

	// 遠い方　遅い
	m_imageFarwaySpace = (CImage*)CGame::GetResource(fileNameSpaceBG[m_bgIndex]);
	m_scrollFarwayY = 0.5;
	if (CFunc::RandI(0, 0)) {
		m_posSpaceFarwayY[0] = CPos(1500 - 200, 100); // 200はズレの分
		m_SpaceFarwayLR = CBackGroundPatternA::ImagePosLR::L; // イメージの左側
	}
	else {
		m_posSpaceFarwayY[0] = CPos(500 + 200, 100); // 200はズレの分
		m_SpaceFarwayLR = CBackGroundPatternA::ImagePosLR::R; // イメージの右側
	}
	m_posSpaceFarwayY[1] = CPos(500 + 200, 100 - 2000); // 200はズレの分



	// 近い方（早い）
	int spaceSpaceBaseIndex = CFunc::RandI(0, 5);
	m_imageNearSpace = (CImage*)CGame::GetResource(fileNameSpaceBase[spaceSpaceBaseIndex]);
	m_scrollNearY = m_scrollFarwayY*1.1;
	if (CFunc::RandI(0, 0)) {
		m_posSpaceNear[0] = CPos(1500 - 200, 100); // 200はズレの分
		m_SpaceNearLR = CBackGroundPatternA::ImagePosLR::L; // イメージの左側
	}
	else {
		m_posSpaceNear[0] = CPos(500 + 200, 100); // 200はズレの分
		m_SpaceNearLR = CBackGroundPatternA::ImagePosLR::R; // イメージの右側
	}
	m_posSpaceNear[1] = CPos(500 + 200, 100 - 2000);


	//----------------------------------------------------------------------------
	// 惑星
	// 今まで選ばれていないインデックスを抽出する(雑コード
	do {
		m_planetIndex = CFunc::RandI(0, 7);

		bool dupFlag = false;
		for (auto index : m_usedPlanetIndex) {
			if (index == m_planetIndex) {
				dupFlag = true;
				break;
			}
		}
		if (!dupFlag) {
			break;
		}
	} while (1);
	m_usedPlanetIndex.push_back(m_planetIndex);
	if (m_usedPlanetIndex.size() >= 8) {
		m_usedPlanetIndex.clear();
		m_usedPlanetIndex.push_back(m_planetIndex);
	}
	m_imagePlanet = (CImage*)CGame::GetResource(fileNamePlanet[m_planetIndex]);
	m_angleAngle = CFunc::ToRad(CFunc::RandD(0, 360));
	m_sizePlanet = CFunc::RandD(0.5, 0.8);
	m_scrollPlanetY = m_scrollFarwayY;
	m_posPlanet.x = CFunc::RandI(CGame::GetGamePosLT().x, CGame::GetGamePosRB().x);
	m_posPlanet.y = CFunc::RandI(CGame::GetGamePosRB().y / 4, -CGame::GetGamePosRB().y*2);

	m_initPlayerPos = CPos(0, 0);
	m_movedPlayerPos = CPos(0, 0);

	m_feedAlpha = 0.0;
}

void CBackGroundPatternA::SetFeedAlpha(double alpha)
{
	m_feedAlpha = alpha;
}

void CBackGroundPatternA::Action() 
{
	m_posSpaceFarwayY[0].y += m_scrollFarwayY;
	m_posSpaceFarwayY[1].y += m_scrollFarwayY;
	if (m_posSpaceFarwayY[0].y >= 100 + 2000) {
		m_posSpaceFarwayY[0].y = 100 - 2000;
	}
	if (m_posSpaceFarwayY[1].y >= 100 + 2000) {
		m_posSpaceFarwayY[1].y = 100 - 2000;
	}
	m_posSpaceNear[0].y += m_scrollNearY;
	m_posSpaceNear[1].y += m_scrollNearY;
	if (m_posSpaceNear[0].y >= 100 + 2000) {
		m_posSpaceNear[0].y = 100-2000;
	}
	if (m_posSpaceNear[1].y >= 100 + 2000) {
		m_posSpaceNear[1].y = 100 - 2000;
	}

	m_posPlanet.y += m_scrollPlanetY;
}

void CBackGroundPatternA::Draw() {
	int alpha = 255 * m_feedAlpha;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);

	// 背景とプレイヤーの位置の位相
	assert(m_scene != nullptr);
	CPos subPos = m_scene->GetBackGroundscrollSmall();
	CPos subPos2 = m_scene->GetBackGroundscrollLarge();



	CDxFunc::MyDrawRotaGraph(m_posSpaceNear[0].x + subPos.x, m_posSpaceNear[0].y + subPos.y, 1.0, 0.0, m_imageNearSpace->m_iamge, TRUE, FALSE);
	CDxFunc::MyDrawRotaGraph(m_posSpaceNear[1].x + subPos.x, m_posSpaceNear[1].y + subPos.y, 1.0, 180.0 / CFunc::RAD, m_imageNearSpace->m_iamge, TRUE, TRUE);


	SetDrawBlendMode(DX_BLENDMODE_ADD, alpha);



	CDxFunc::MyDrawRotaGraph(m_posSpaceFarwayY[0].x + subPos2.x, m_posSpaceFarwayY[0].y + subPos2.y, 1.0, 0.0, m_imageFarwaySpace->m_iamge, TRUE, FALSE);
	if (m_SpaceFarwayLR == CBackGroundPatternA::ImagePosLR::R) {
		CDxFunc::MyDrawRotaGraph(m_posSpaceFarwayY[1].x + subPos2.x, m_posSpaceFarwayY[1].y + subPos2.y, 1.0, 180.0 / CFunc::RAD, m_imageFarwaySpace->m_iamge, TRUE, TRUE);
	}
	else {
		CDxFunc::MyDrawRotaGraph(m_posSpaceFarwayY[1].x + subPos2.x, m_posSpaceFarwayY[1].y + subPos2.y, 1.0, 180.0 / CFunc::RAD, m_imageFarwaySpace->m_iamge, TRUE, TRUE);
	}



	// 惑星
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	CDxFunc::MyDrawRotaGraph(m_posPlanet.x + subPos2.x, m_posPlanet.y + subPos2.y, m_sizePlanet, m_angleAngle, m_imagePlanet->m_iamge, TRUE, TRUE);


	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
}

void CBackGroundPatternA::SetInitPlayerPos(CPos& initPos) {
	m_initPlayerPos = initPos;
}
void CBackGroundPatternA::SetPlayerMovedPos(CPos& movedPos) {
	m_movedPlayerPos = movedPos;
}
