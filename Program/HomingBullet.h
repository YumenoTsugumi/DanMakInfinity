#pragma once

#include "Func.h"
#include "BaseBullet.h"

//ホーミング情報
class CLimitInfo{
public:
	int m_count;
	double m_limitAngle;	//制限角度
	//m_countフレームまでm_limitAngleである

	//m_countは小さい順に並んでいること
	//CVoidArrayでCHomingLaserに渡すこと

	//ex)
	//m_count:10 8.0
	//m_count:30 3.0
	//m_count:MAXINT 0.5
};

class CHomingLaser : public CBaseBullet{
public:

	//値設定　主にコンストラクタから呼ぶ
	virtual void Set();
	//コンストラクタ　初期化する物が増えてる
	CHomingLaser(EDirType type, CPos P, int NUM, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//CHomingLaser(EDirType type, CPos P, int NUM, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName);
	~CHomingLaser();

	int m_num;
	CPos *m_posA;
	double *m_angleA;

	std::vector<CLimitInfo*> m_limit;

	void AddLimitInfo(int Count, double Limit);

	//メイン行動
	virtual void Action();
	virtual void Draw();

	virtual void Move();
	virtual void RectOut();
	virtual void Remove();
};

/*

CPos pp(CFunc::RandF(400,400), CFunc::RandF(100,100));
for(int i=0;i<12;i++){
	CHomingLaser *hl = new CHomingLaser(EDirType::Player,		//射出方向
										pp,			//座標
										20,			//レーザー長さ
										12,			//速度
										i*30.0,		//射出角度
										0,0,0,		//加速は有効
										"hLaser01");//画像
	hl->AddLimitInfo(30, 5.0);						//30frまでは角度制限5.0
	hl->AddLimitInfo(60, 3.0);						//60frまでは角度制限3.0
	hl->AddLimitInfo(MAXINT, 0.5);					//以降は角度制限0.5
	bulletManager->Add( hl );
}

*/















