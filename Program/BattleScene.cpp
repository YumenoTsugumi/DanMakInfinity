#include "BattleScene.h"

#include "Player.h"
//#include "Inheritance.h"
//#include "Area.h"
//#include "OtherInfo.h"
#include "Game.h"

//�ÓI�Ȃ̂��g���ɂ̓R��������H
//CResourceManager* CScene::resManager;

CBattleScene::CBattleScene(int InTime) : 
	CScene(InTime){
	//�V�[��,	�t�F�[�h�C������60, �t�F�[�h�A�E�g����60, 
	//			�t�F�[�h�C���Z�W0.0, �t�F�[�h�A�E�g�Z�W0.0
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
	//�t�F�[�h���łȂ����
	if(!NowFeed()){
		//���͎Q��
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

	////�ǂ�ȏ�Ԃł��A�N�V�������鏈��
	//player->Action(input);
	//bulletManeger->Action();
	//area->Action();

	//area->Draw();
	//player->Draw();
	//bulletManeger->Draw();
	

	//m_count++;

}
