#include "BattleScene.h"

#include "Player.h"
//#include "Inheritance.h"
//#include "Area.h"
//#include "OtherInfo.h"
#include "Game.h"

//静的なのを使うにはコレがいる？
//CResourceManager* CScene::resManager;

CBattleScene::CBattleScene(int InTime) : 
	CScene(InTime){
	//シーン,	フェードイン時間60, フェードアウト時間60, 
	//			フェードイン濃淡0.0, フェードアウト濃淡0.0
	Set(InTime);
}

CBattleScene::~CBattleScene(){
	//delete(bulletManeger);
}

void CBattleScene::Init(CGame* gameP){
	//m_game = gameP;

	//bulletManeger = new CInhBulletManager(1024);
	//CCustomBullet::SetBulletManagerPointer(bulletManeger);

	//player = new CPlayer();
	//otherInfo = new COtherInfo();
	//area = new CArea();

	//pointerSet.player = player;
	//pointerSet.otherInfo = otherInfo;
	//pointerSet.area = area;
	//pointerSet.resManager = CScene::resManager;

	//player->setPointerSet(&pointerSet);
	//otherInfo->setPointerSet(&pointerSet);
	//area->setPointerSet(&pointerSet);
	//bulletManeger->setPointerSet(&pointerSet);
	//CInhBullet::setPointerSet(&pointerSet);

	//otherInfo->init(game->m_rect);
	//player->init();
	//area->init();
}

void CBattleScene::Main(CInputAllStatus *input){
	//フェード中でなければ
	if(!NowFeed()){
		//入力参照
		if(input->m_btnStatus[INPUT_DEF_ENTER] == INPUT_PUSH){
			SetFeedOut(120);
			SetBackScene();
		}
	}

	//int bulletA[] = {00,01,02, 10,11,12, 20,21,22,23,24,25, 30,31,32, 40,41,42, 50,51,52};

	//CPos ppp(m_count % 100,m_count % 100);
	//CInhBullet *bu = new CInhBullet(ABS, ppp, (m_count%5)+1.0, m_count, 0,0,0, bulletA[CFunc::iRand(18,18)] );
	////bu->SetBlendType(BLEND_ADD);
	//bulletManeger->Add(bu);

	////どんな状態でもアクションする処理
	//player->Action(input);
	//bulletManeger->Action();
	//area->Action();

	//area->Draw();
	//player->Draw();
	//bulletManeger->Draw();
	

	//m_count++;

}
