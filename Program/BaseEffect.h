#pragma once

#include "Func.h"
#include "BaseBullet.h"


class CBaseEffect;
class CEffectManager;

//---------------------------------------------------------------------------------
//	CEffectManager
//---------------------------------------------------------------------------------
//弾の管理マネージャと同じ　名前的に継承しただけ

class CEffectManager : public CBulletManager{
public:

	//コンストラクタ
	//弾幕の発射最大数を設定する
	//デフォルト1024
	CEffectManager(int m_num = 512);
	virtual ~CEffectManager();

};


//---------------------------------------------------------------------------------
//	CBaseEffect
//---------------------------------------------------------------------------------
//エフェクトも弾と似ている
class CBaseEffect : public CBaseBullet{

public:

	//その濃さ　0〜255	使用時にはintにキャストする 
	double m_blendDepth;

	//濃さの加減算
	double m_plusBlend;
	void SetBlend(int BlendDepth, double PlusBlend);

	//描画サイズ
	double m_size;
	//サイズの加減算
	double m_plusSize;
	//この値を超えたら削除する
	double m_deleteOverSize;
	void SetSize(double Size, double PlusSize);	//削除する際の急速縮小値
	void SetdeleteOverSize(double deleteOverSize);	//削除する際の急速濃度

	//CBaseBullet::SetBlendType
	//DX_BLENDMODE_NOBLEND　:　ノーブレンド（デフォルト）
	//DX_BLENDMODE_ALPHA　　:　αブレンド
	//DX_BLENDMODE_ADD　　　:　加算ブレンド
	//DX_BLENDMODE_SUB　　　:　減算ブレンド
	//DX_BLENDMODE_MULA　　　:　乗算ブレンド
	//DX_BLENDMODE_INVSRC　　:　反転ブレンド
private:
	//削除中か
	bool m_removeNow;
public:
	//削除する中の減算値
	double m_deleteTimingPlusAlpha;
	double m_deleteTimingPlusSize;
	void SetDeleteTiming(double DeleteTimingPlusAlpha, double DeleteTimingPlusSize);

	//アニメーションがある画像でアニメが終わったら削除するかどうか
	//true:削除する	false:しない
	bool m_animeEndDelFlg;
	void SetAnimeEndDelFlg(bool AnimeEndDelFlg);

	//時間で削除する
	int m_removeCount;
	void SetRemoveCount(int DelCount);

	//後は適当に何かさせる用に
	void (*funcP)(CBaseEffect* eff);
	void SetFuncP(void (*FuncP)(CBaseEffect* eff));

	//コンストラクタ	弾作成
	CBaseEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG);
	CBaseEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME);

	//デストラクタ
	virtual ~CBaseEffect();
	virtual void Set();
	//virtual void SetImage(int IMG);

	//とりあえず全部継承しとく
	//メイン行動
	virtual void Action();
	//メイン描画
	virtual void Draw();

	virtual void EffectAction();

	//移動制御	弾のを流用
	//virtual void Move();
	//範囲外	コールしない
	//virtual void RectOut();
	//削除
	virtual void Remove();
};

//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*

普通の弾幕クラスと同じ使い方ができる


	CPos pp(CFunc::RandF(0,800), CFunc::RandF(0,400));
	double ang = CFunc::RandF(0,360);
	CBaseEffect* eff = new CBaseEffect(ABS, pp, 2.0, ang, 0,0,0, 206);
	eff->SetSize(3.0, +0.05);
	eff->SetBlend(192, +3.0);

	eff->SetAnimeEndDelFlg(false);	//アニメーション終了後削除するか
	eff->SetRemoveCount(60);	//60frで削除

	effectManeger1->Add(eff);


*/







//---------------------------------------------------------------------------------
//	CBaseEffect
//---------------------------------------------------------------------------------
const int STRINGEFFECTMAX = 80;
//イメージ描画せずに文字を描画する
class CStringEffect : public CBaseEffect{

public:

private:
	char drawString[STRINGEFFECTMAX];
public:
	CFont *font;
	int fontCr;
	int edgeCr;

	//コンストラクタ	弾作成
	CStringEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, char* DRAWSTR, int FONT);
	CStringEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, char* DRAWSTR, const char* FONT);

	//デストラクタ
	virtual ~CStringEffect();
	//virtual void Set();
	//virtual void SetImage(int IMG);

	//とりあえず全部継承しとく
	//メイン行動
	virtual void Action();
	//メイン描画
	virtual void Draw();

	//virtual void EffectAction();

	//移動制御	弾のを流用
	//virtual void Move();
	//範囲外	コールしない
	//virtual void RectOut();
	//削除
	//virtual void Remove();
};


