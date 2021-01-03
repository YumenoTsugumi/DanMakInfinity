
#include "BackGround.h"
#include "Game.h"

CBackGround::CBackGround() : m_effectManager(512){
}
CBackGround::~CBackGround() {
}
void CBackGround::Action() {
}
void CBackGround::Draw() {
}


char fileNameSpaceBG[11][20] = {
	"X_bg_space_00",	"X_bg_space_01",	"X_bg_space_02",	"X_bg_space_03",
	"X_bg_space_04",	"X_bg_space_05",	"X_bg_space_06",	"X_bg_space_07",
	"X_bg_space_08",	"X_bg_space_09",	"X_bg_space_10"
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
	int spaceBGIndex = CFunc::RandI(0, 10);
	m_imageFarwaySpace = (CImage*)CGame::GetResource(fileNameSpaceBG[spaceBGIndex]);
	m_scrollFarwayY = 0.4;
	if (CFunc::RandI(0, 1)) {
		m_posSpaceFarwayY = CPos(400.0 - (1000.0 - 500.0), 300.0 - (1000.0 - 300.0));
	}
	else {
		m_posSpaceFarwayY = CPos(400.0 + (1000.0 - 500.0), 300.0 - (1000.0 - 300.0));
	}

	int spaceSpaceBaseIndex = CFunc::RandI(0, 6);
	m_imageNearSpace = (CImage*)CGame::GetResource(fileNameSpaceBase[spaceSpaceBaseIndex]);
	m_scrollNearY = m_scrollFarwayY * 0.5;
	if (CFunc::RandI(0, 1)) {
		m_posSpaceNear = CPos(400.0 - (1000.0 - 500.0), 300.0 - (1000.0 - 300.0));
	}
	else {
		m_posSpaceNear = CPos(400.0 + (1000.0 - 500.0), 300.0 - (1000.0 - 300.0));
	}


	m_initPlayerPos = CPos(0,0);
	m_movedPlayerPos = CPos(0, 0);
}
CBackGroundPatternA::~CBackGroundPatternA()
{

}
void CBackGroundPatternA::Action() 
{
	m_posSpaceFarwayY.y += m_scrollFarwayY;
	m_posSpaceNear.y += m_scrollNearY;

}

void CBackGroundPatternA::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	CPos subPos; // 背景とプレイヤーの位置の位相
	subPos.x = (m_initPlayerPos.x - m_movedPlayerPos.x) / 5.0;
	subPos.y = (m_initPlayerPos.y - m_movedPlayerPos.y) / 5.0;
	CDxFunc::MyDrawRotaGraph(m_posSpaceFarwayY.x + subPos.x, m_posSpaceFarwayY.y + subPos.y, 1.0, 0.0, m_imageFarwaySpace->m_iamge, TRUE, FALSE);
	CDxFunc::MyDrawRotaGraph(m_posSpaceFarwayY.x + subPos.x, m_posSpaceFarwayY.y - 2000.0 + subPos.y, 1.0, 180.0 / CFunc::RAD, m_imageFarwaySpace->m_iamge, TRUE, TRUE);


	SetDrawBlendMode(DX_BLENDMODE_ADD, 255);
	CDxFunc::MyDrawRotaGraph(m_posSpaceNear.x + subPos.x, m_posSpaceNear.y + subPos.y, 1.0, 0.0, m_imageNearSpace->m_iamge, TRUE, FALSE);
	CDxFunc::MyDrawRotaGraph(m_posSpaceNear.x + subPos.x, m_posSpaceNear.y - 2000.0 + subPos.y, 1.0, 180.0 / CFunc::RAD, m_imageNearSpace->m_iamge, TRUE, FALSE);

}

void CBackGroundPatternA::SetInitPlayerPos(CPos& initPos) {
	m_initPlayerPos = initPos;
}
void CBackGroundPatternA::SetPlayerMovedPos(CPos& movedPos) {
	m_movedPlayerPos = movedPos;
}
