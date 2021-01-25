#pragma once

#include "Func.h"
#include "BaseBullet.h"


class CBaseItem;
class CItemManager;

//---------------------------------------------------------------------------------
//	CItemManager
//---------------------------------------------------------------------------------
//弾の管理マネージャと同じ　名前的に継承しただけ

class CItemManager : public CBulletManager{
public:

	//コンストラクタ
	//発射最大数を設定する
	//デフォルト1024
	CItemManager(int m_num = 512);
	virtual ~CItemManager();

	//メイン描画
	virtual void Draw();
};


//---------------------------------------------------------------------------------
//	CBaseItem
//---------------------------------------------------------------------------------
//エフェクトも弾と似ている
class CBaseItem : public CBaseBullet{

public:
	// 待つ時間
	int m_waitTime;
	int m_waitCount;
	void SetWaitTime(int waitTime);

private:
	//削除中か
	bool m_removeNow;
public:

	//後は適当に何かさせる用に
	void (*funcP)(CBaseItem* eff);
	void SetFuncP(void (*FuncP)(CBaseItem* eff));

	//コンストラクタ	弾作成
	CBaseItem(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//CBaseItem(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName);

	//デストラクタ
	virtual ~CBaseItem();
	virtual void Set();
	//virtual void SetImage(int image);

	//とりあえず全部継承しとく
	//メイン行動
	virtual void Action();
	//メイン描画
	virtual void Draw();

	virtual void RectOut();

	double m_gravity; // 重力　落ちる速度

	//移動制御	弾のを流用
	//virtual void Move();
	//範囲外	コールしない
	//virtual void RectOut();
	//削除
	virtual void Remove();

	double m_size;
	double m_plusSize;
	double m_maxSize;
	void SetSize(double size, double plusSize, double maxSize);

	void SetRemove();

	void SetRecovery();
	bool m_recoveryFlag;

	// 出てからアイテムを一定時間は取れないようにするための判定
	bool IsTakeTime();
};

//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*

普通の弾幕クラスと同じ使い方ができる


	CPos pp(CFunc::RandF(0,WindowX), CFunc::RandF(0,400));
	double ang = CFunc::RandF(0,360);
	CBaseItem* eff = new CBaseItem(EDirType::Abs, pp, 2.0, ang, 0,0,0, 206);
	eff->SetSize(3.0, +0.05);
	eff->SetBlend(192, +3.0);

	eff->SetAnimeEndDelFlg(false);	//アニメーション終了後削除するか
	eff->SetRemoveCount(60);	//60frで削除

	effectManeger1->Add(eff);


*/

