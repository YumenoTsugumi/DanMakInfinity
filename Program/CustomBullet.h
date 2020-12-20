#pragma once

#include "Func.h"
#include "BaseBullet.h"

//変化弾で値を引き継ぐ場合の値
const double NOTRANS = 99999;

//汎用弾幕
//通常の機能に加え
//	変化弾、追加弾を搭載
//	弾の実態化m_wait、その描画も追加
class CCustomBullet : public CBaseBullet{
public:

	//値設定　主にコンストラクタから呼ぶ
	virtual void Set();
	//コンストラクタ　初期化する物が増えてる
	CCustomBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG);
	CCustomBullet(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME);

	//デストラクタ
	virtual~CCustomBullet();

	//自分自身を管理しているマネージャのポインタ(関数ポインタで参照するため
	static CBulletManager *m_manager;
	static void SetBulletManagerPointer(CBulletManager *Manager);

	//メイン行動
	virtual void Action();
	virtual void Draw();

	//---------------
	//変化弾の変数、機能
	//---------------
	//変化する時間
	int m_transFrame;
	//変化後の弾
	CCustomBullet* transBullet;
	//変化後の角度	コンストラクタでm_angleが変わるため(PLAYERの場合)計算できないので、この変数より求める
	double transAngle;
	void SetTransAngle(double Angle);	//PLAYERかつNOTRANSでない(＝変化弾である)場合にコールする
	void SetTransBullet(CCustomBullet* Bullet, int m_transFrame);
	void Trans();
	//座標値は必ず変化前を引き継ぐ
	//弾幕の対象がPLAYERだった場合、変化タイミングで再評価する
	//	再評価したくない場合は、ABSにして自力で計算した値を渡すこと
	//値を変化させたくない場合NOTRANSを指定する
	//画像を変化させたくない場合は同じIDXをしていしてやる（アニメとか引き継ぐ

	//---------------
	//追加弾の変数、機能
	//---------------
	//弾を吐く時間
	int m_addFrame;
	void (*addFuncP)(CCustomBullet* m_bullet);

	void SetAddBullet(void (*funcP)(CCustomBullet* m_bullet), int m_addFrame);
	//追加の弾
	void Add();


	//---------------
	//具現化
	//---------------
	//実態化する時間
	int m_standByTime;
	//実体化するまでのカウント	0～
	int m_standByCount;
	void SetStandByTime(int Time);
};



//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*

○基本的な操作
	CbaseBulletの初期化と
	initで弾管理クラスのポインタを設定しておくこと
	CCustomBullet::SetBulletManagerPointer(bulletManager);


	下記の２つの設定が必要
	//
	//	CBaseBulletクラスの設定しなければならないもの
	//
	CBaseBullet::SetRect(CRect(0,0,640,640));
	CBaseBullet::setImageData(m_imageManager);


○追加弾
	//適当な関数
	void addFuncA(CCustomBullet* m_bullet){
		for(int i=0;i<5;i++){
			m_bullet->m_manager->Add(new CBaseBullet(ABS, m_bullet->pos, 0, 90 + -20+i*10, 0,0.01,10, "bullet00"));
		}
	}
	void addFuncB(CCustomBullet* m_bullet){
		for(int i=0;i<10;i++){
			m_bullet->m_manager->Add(new CBaseBullet(PLAYER, m_bullet->pos, 2.5, i*(360.0/10), 0,0,0, "bullet00"));
		}
	}


	CCustomBullet *ppp = new CCustomBullet(ABS, CPos(320,240), 0.0, CFunc::RandF(180,360) ,0,0.02,10, 0);
	//20fr毎にaddFuncAを実行する
	ppp->SetAddBullet(addFuncA, 20);
	//弾を追加する
	bulletManager->Add(ppp);

○変化弾
	CCustomBullet *m_bullet = new CCustomBullet(ABS, CPos(320,240), 4, aaa, 0,0,0, "bullet00");
	CCustomBullet *Trans = new CCustomBullet(PLAYER, CPos(0,0), 0, 0, 0,0.25,10, "bullet00");
	CCustomBullet *trans2 = new CCustomBullet(ABS, CPos(0,0), NOTRANS, CFunc::RandF(0,360), 0,0,0, 1);

	//値を変化させたくないときはNOTRANSを設定する
	//角度は
	//ABS	NOTRANS		変化しない
	//ABS	任意の角度	
	//PLAYER	NOTRANS		変化しない
	//PLAYER	任意の角度	SetTransBulletしなければならない

	//座標
	//不要

	//m_bulletが30fr後にTransになる
	m_bullet->SetTransBullet(Trans, 60);

	//PLAYER狙いの時だけ、変化後の角度を再設定してあげないといけない
	Trans->SetTransAngle(0);

	//Transが20fr後にtrans2になる
	Trans->SetTransBullet(trans2, 60);

	//上記を設定後m_bulletを発射
	bulletManager->Add(m_bullet);



*/