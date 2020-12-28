
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
	m_image06 = (CBulletImage*)CGame::GetResource(10006);
}
CBackGroundPatternA::~CBackGroundPatternA(){

}
void CBackGroundPatternA::Action() {

	{
		static int count2 = 0;
		count2++;
		if ( 0 && count2 == 1) {
			CPos pp(CFunc::RandF(0, 800), CFunc::RandF(0, 400));
			//double ang = CFunc::RandF(180 + 45, 360 - 45);
			double ang = CFunc::RandF(0 + 45, 180 - 45);
			CBaseEffect* eff = new CBaseEffect(EDirType::Abs, pp, CFunc::RandF(5.0, 7.5), ang, 0, 0, 0, 0, 10005);
			eff->SetSize(CFunc::RandF(1.0, 2.0), CFunc::RandF(0.1, 0.05));
			eff->SetBlend(CFunc::RandI(16, 64), +0.0);
			eff->SetAnimeEndDelFlg(false);	//アニメーション終了後削除するか
			eff->SetRemoveCount(CFunc::RandI(180, 360));	//60frで削除
			eff->SetBlendType(DX_BLENDMODE_SUB);
			m_effectManager.Add(eff);
		}
	}
	{
		static int count = 0;
		count++;
		if (count % 480 == 1) {
			CPos pp(400, 0);
			double ang = 90.0;
			CBaseEffect* eff = new CBaseEffect(EDirType::Abs, pp, 2.0, ang, 0, 0, 0, 0, 10006);
			eff->SetBlend(0, 32, 214);
			eff->SetAnimeEndDelFlg(false);	//アニメーション終了後削除するか
			eff->SetRemoveCount(180);	//60frで削除
			eff->SetSize(2.0, 0.0);
			eff->SetDeleteTiming(-4.0, 0);
			//DX_BLENDMODE_NOBLEND　:　ノーブレンド（デフォルト）
			//DX_BLENDMODE_ALPHA　　:　αブレンド
			//DX_BLENDMODE_ADD　　　:　加算ブレンド
			//DX_BLENDMODE_SUB　　　:　減算ブレンド
			//DX_BLENDMODE_MULA　　　:　乗算ブレンド
			//DX_BLENDMODE_INVSRC　　:　反転ブレンド
			eff->SetBlendType(DX_BLENDMODE_SUB);

			m_effectManager.Add(eff);
		}
	}


	m_effectManager.Action();
}

void CBackGroundPatternA::Draw() {
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CDxFunc::DrawRotaGraph(400, 300, 1.0, 0.0, m_image01->m_images[0]);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	m_effectManager.Draw();
}

