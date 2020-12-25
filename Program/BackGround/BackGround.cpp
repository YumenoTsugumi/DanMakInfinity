
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


CBackGroundPatternA::CBackGroundPatternA() {
	m_images = (CImages*)CGame::GetResource(10000);
	m_image01 = (CBulletImage*)CGame::GetResource(10001);
	m_image02 = (CBulletImage*)CGame::GetResource(10002);
	m_image03 = (CBulletImage*)CGame::GetResource(10003);
	m_image04 = (CBulletImage*)CGame::GetResource(10004);
	m_image05 = (CBulletImage*)CGame::GetResource(10005);
}
CBackGroundPatternA::~CBackGroundPatternA(){

}
void CBackGroundPatternA::Action() {

	CPos pp(CFunc::RandF(0, 800), CFunc::RandF(0, 400));
	double ang = CFunc::RandF(180+45, 360-45);
	CBaseEffect* eff = new CBaseEffect(EDirType::Abs, pp, CFunc::RandF(1.0, 1.5), ang, 0, 0, 0, 0, 10005);
	eff->SetSize(CFunc::RandF(1.0, 2.0), CFunc::RandF(0.1, 0.05));
	eff->SetBlend(CFunc::RandI(16, 64), +0.0);

	eff->SetAnimeEndDelFlg(false);	//アニメーション終了後削除するか
	eff->SetRemoveCount(CFunc::RandI(180, 360));	//60frで削除

	m_effectManager.Add(eff);

	m_effectManager.Action();
}

void CBackGroundPatternA::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::DrawRotaGraph(400, 300, 1.0, 0.0, m_image01->m_images[0]);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	m_effectManager.Draw();

}