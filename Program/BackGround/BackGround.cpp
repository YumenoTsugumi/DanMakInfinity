
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


char fileNameSpaceBG[10][20] = {
	"X_bg_space_00",	"X_bg_space_01",	"X_bg_space_02",	"X_bg_space_03",
	"X_bg_space_04",	"X_bg_space_05",	"X_bg_space_06",	"X_bg_space_07",
	"X_bg_space_08",	"X_bg_space_09"
};
char fileNameSpaceBase[6][20] = {
	"X_bg_space_b00",	"X_bg_space_b01",	"X_bg_space_b02",
	"X_bg_space_b03",	"X_bg_space_b04",	"X_bg_space_b05"
};
char fileNamePlanet[10][20] = {
	"bg_planet_00",	"bg_planet_01",	"bg_planet_02",	"bg_planet_03",	"bg_planet_04",	
	"bg_planet_05",	"bg_planet_06",	"bg_planet_07",	"bg_planet_08",	"bg_planet_09"
};

CBackGroundPatternA::CBackGroundPatternA()
{
	int spaceBGIndex = CFunc::RandI(0, 9);
	m_imageFarwaySpace = (CImage*)CGame::GetResource(fileNameSpaceBG[spaceBGIndex]);
	m_scrollFarwayY = 1.0;
	if (CFunc::RandI(0, 0)) {
		m_posSpaceFarwayY[0] = CPos(1500 - 200, 100); // 200はズレの分
		m_SpaceFarwayLR = CBackGroundPatternA::ImagePosLR::L; // イメージの左側
	} else {
		m_posSpaceFarwayY[0] = CPos(500 + 200, 100); // 200はズレの分
		m_SpaceFarwayLR = CBackGroundPatternA::ImagePosLR::R; // イメージの右側
	}
	m_posSpaceFarwayY[1] = CPos(500 + 200, 100-2000); // 200はズレの分

	int spaceSpaceBaseIndex = CFunc::RandI(0, 5);
	m_imageNearSpace = (CImage*)CGame::GetResource(fileNameSpaceBase[spaceSpaceBaseIndex]);
	m_scrollNearY = m_scrollFarwayY * 2;
	if (CFunc::RandI(0, 0)) {
		m_posSpaceNear[0] = CPos(1500 - 200, 100); // 200はズレの分
		m_SpaceNearLR = CBackGroundPatternA::ImagePosLR::L; // イメージの左側
	} else {
		m_posSpaceNear[0] = CPos(500 + 200, 100); // 200はズレの分
		m_SpaceNearLR = CBackGroundPatternA::ImagePosLR::R; // イメージの右側
	}
	m_posSpaceNear[1] = CPos(500 + 200, 100-2000);

	m_initPlayerPos = CPos(0,0);
	m_movedPlayerPos = CPos(0, 0);
}
CBackGroundPatternA::~CBackGroundPatternA()
{

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
}

void CBackGroundPatternA::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	//CPos subPos; // 背景とプレイヤーの位置の位相
	//subPos.x = (m_initPlayerPos.x - m_movedPlayerPos.x) / 5.0;
	//subPos.y = 0;
	assert(m_scene != nullptr);
	CPos subPos = m_scene->GetBackGroundscrollSmall();


	CDxFunc::MyDrawRotaGraph(m_posSpaceNear[0].x + subPos.x, m_posSpaceNear[0].y + subPos.y, 1.0, 0.0, m_imageNearSpace->m_iamge, TRUE, FALSE);
	CDxFunc::MyDrawRotaGraph(m_posSpaceNear[1].x + subPos.x, m_posSpaceNear[1].y + subPos.y, 1.0, 180.0 / CFunc::RAD, m_imageNearSpace->m_iamge, TRUE, TRUE);

	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);

	//CPos subPos2; // 背景とプレイヤーの位置の位相
	//subPos2.x = (m_initPlayerPos.x - m_movedPlayerPos.x) / 10.0;
	//subPos2.y = 0;
	CPos subPos2 = m_scene->GetBackGroundscrollLarge();

	CDxFunc::MyDrawRotaGraph(m_posSpaceFarwayY[0].x + subPos2.x, m_posSpaceFarwayY[0].y + subPos2.y, 1.0, 0.0, m_imageFarwaySpace->m_iamge, TRUE, FALSE);
	if (m_SpaceFarwayLR == CBackGroundPatternA::ImagePosLR::R) {
		CDxFunc::MyDrawRotaGraph(m_posSpaceFarwayY[1].x + subPos2.x, m_posSpaceFarwayY[1].y + subPos2.y, 1.0, 180.0 / CFunc::RAD, m_imageFarwaySpace->m_iamge, TRUE, TRUE);
	}
	else {
		CDxFunc::MyDrawRotaGraph(m_posSpaceFarwayY[1].x + subPos2.x, m_posSpaceFarwayY[1].y + subPos2.y, 1.0, 180.0 / CFunc::RAD, m_imageFarwaySpace->m_iamge, TRUE, TRUE);
	}

}

void CBackGroundPatternA::SetInitPlayerPos(CPos& initPos) {
	m_initPlayerPos = initPos;
}
void CBackGroundPatternA::SetPlayerMovedPos(CPos& movedPos) {
	m_movedPlayerPos = movedPos;
}
