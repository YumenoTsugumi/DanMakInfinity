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
	CItemManager(int m_num = 1024);
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

	//輝き　画像データへのポインタ　総合画像データから保存しておく
	static CBulletImage* m_shineImage;
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
	//削除
	virtual void Remove();

	// プレイヤーの範囲に入るまでの時間
	int m_survivalTime;
	int m_itemRank; // 1（取るまでに時間が掛かった）～3（かなり早く取った）

	double m_size;
	double m_plusSize;
	double m_maxSize;
	void SetSize(double size, double plusSize, double maxSize);

	int GetSurvivalTimeRank();
	void SetRemove();
	void GetItemAddScore();
	void SetRecovery();
	void SetPreRecovery(); // 45fr経過するまでにアイテムの取得範囲にはいると、45fr後に自動回収にする
	bool m_recoveryFlag; // アイテム回収状態フラグ
	bool m_preRecoveryFlag; // アイテム事前回収状態フラグ
	bool GetRecoveryFlag();

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

	effectManager1->Add(eff);


*/

