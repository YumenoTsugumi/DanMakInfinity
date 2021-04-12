#pragma once

#include "Func.h"
#include "Resource.h"

class CBaseBullet;
class CBulletImage;
class CBulletImageManager;
class CBulletManager;
class CBulletImageInfo;

class CResourceManager;




enum EDirType
{
	Player = 0,
	Abs = 1
};
//const bool EDirType::Player = false;
//const bool EDirType::Abs = true;

const int BLEND_NO = DX_BLENDMODE_NOBLEND;
const int BLEND_ALPHA = DX_BLENDMODE_ALPHA;
const int BLEND_ADD = DX_BLENDMODE_ADD;
const int BLEND_SUB = DX_BLENDMODE_SUB;
const int BLEND_MULA = DX_BLENDMODE_MULA;
const int BLEND_INVSRC = DX_BLENDMODE_INVSRC;


//---------------------------------------------------------------------------------
//	CBulletImageInfo
//---------------------------------------------------------------------------------

//画像関連情報
//各弾がこの情報を必ず１つ持つ
//イメージ元もこの構造体を持つ　その際使わない情報あり　*が使わない情報
class CBulletImageInfo{
public:
	int m_imageNo;//*画像No
	int m_sizeX, m_sizeY;	//1枚当たりの画像サイズ	(読み込み時に設定する

	bool m_animeFlg;	//アニメーションするか　false:しない	true:する
	char m_animeNum;	//アニメーション枚数	2～n
	char m_animePos;	//*現在のアニメーションの位置
	char m_animeSpeed;	//アニメ速度	アニメの画像を切り替えるfr

	bool m_rotationFlg;	//回転するか　false:しない	true:する
	double m_rotationSpeed;	//回転速度	fr毎の加算角度(rad)
	double m_rotationAngle;	//*現在の角度(rad)

	//コンストラクタ
	CBulletImageInfo();
	//全部初期化
	void Init();
	//入力引数を全部セット
	void Set(	bool m_animeFlg,char m_animeNum,char m_animePos,char m_animeSpeed,
				bool m_rotationFlg,double m_rotationSpeed,double m_rotationAngle);

	//すべて代入
	void operator=(const CBulletImageInfo& info){
		m_imageNo	= info.m_imageNo;
		m_sizeX	= info.m_sizeX;
		m_sizeY	= info.m_sizeY;
		m_animeFlg	= info.m_animeFlg;
		m_animeNum	= info.m_animeNum;
		m_animePos	= info.m_animePos;
		m_animeSpeed	= info.m_animeSpeed;
		m_rotationFlg	= info.m_rotationFlg;
		m_rotationSpeed	= info.m_rotationSpeed;
		m_rotationAngle	= info.m_rotationAngle;
	}
};

//---------------------------------------------------------------------------------
//	CBulletManager
//---------------------------------------------------------------------------------

//弾を管理するクラス
class CBulletManager{
public:

	//コンストラクタ
	//弾幕の発射最大数を設定する
	//デフォルト2048
	CBulletManager(int m_num = 2048);
	virtual ~CBulletManager();

	//発射最大数	1024ぐらいあれば弾幕できるんじゃないか
	//コンストラクタで設定後は変更不可
	int m_bulletTotalNum;

	//弾の挿入位置	徐々にずらし弾が重ならないようにするため
	int m_order;

	//弾データ	弾クラスを代入する
	//ポインタを入れる箱
	CBaseBullet** m_bullet;

	virtual void Action();
	virtual void Draw();

	int Add(CBaseBullet *bullet);
};

//---------------------------------------------------------------------------------
//	CBaseBullet
//---------------------------------------------------------------------------------
class CBaseBullet{
public:
	//-----------------------------------
	//	静的メンバー　クラスを利用する
	//	前に設定しておくこと
	//-----------------------------------
	//表示領域
	static CRect m_rect;
	static void SetRect(CRect &m_rect);

	//自機狙い用座標
	static CPos m_target;
	static void SetTarget(CPos pos);	//毎fr更新しないといけない

protected:
	//-----------------------------------
	//	画像データ関連
	//-----------------------------------

	//画像データへのポインタ　総合画像データから保存しておく
	int* m_image;
public:
	//画像情報
	CBulletImageInfo m_imageInfo;

	//DX_BLENDMODE_NOBLEND　:　ノーブレンド（デフォルト）
	//DX_BLENDMODE_ALPHA　　:　αブレンド
	//DX_BLENDMODE_ADD　　　:　加算ブレンド
	//DX_BLENDMODE_SUB　　　:　減算ブレンド
	//DX_BLENDMODE_MULA　　　:　乗算ブレンド
	//DX_BLENDMODE_INVSRC　　:　反転ブレンド
	//描画方法　ALPHAやADD
	int m_blendType;
	void SetBlendType(int BlendType);

	int m_count;	//カウント　毎フレーム刻む　いろいろ使う

	bool m_removeFlg;	//弾を消す時にtrueにすると、管理クラスが解放してくれる
	bool m_enableRemoveEffect; // 削除時のエフェクトを表示するか
	//-----------------------------------
	//	弾情報
	//-----------------------------------
	CPos m_pos;
	CPos m_vel;
	bool m_type;	//自機狙いかどうか
	double m_angle, m_speed;
	double m_corner, m_accelerate, m_maxsp;

	// 削除フラグ立てる
	void SetRemove();
public:
	//-----------------------------------
	//	メインループ
	//-----------------------------------
	//メイン行動
	virtual void Action();
	//メイン描画
	virtual void Draw();

	//移動制御
	virtual void Move();
	//範囲外
	virtual void RectOut();
	//削除
	virtual void Remove();

	//値設定　主にコンストラクタから呼ぶ
	virtual void Set(bool type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle);
	//画像関連
	virtual void SetImage(int image);
	//virtual void SetImage(const char* ImageName);
	virtual void SetImageInfo(int imageNo, CBulletImage* bulletImage);

	//コンストラクタ	弾作成
	CBaseBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//CBaseBullet(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName);
	//デストラクタ
	virtual ~CBaseBullet();

	// 弾が何かに当たった時
	void virtual Hit();
};



//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*
//●必要ヘッダー
	#include "\MyFunc\MyInclude.h"

//●メインループ前
	//----------------
	//	リソースメイン
	//----------------
	CResourceManager resManager;

	//弾幕画像
	//ふぁいる、総数、X、Y、サイズX、サイズY、アニメ速度、回転速度
	resManager.Add(new CBulletImage("Image/b0r.png",	1, 1, 1, 32, 32, 0, 0.0), "bullet00", 0);
	resManager.Add(new CBulletImage("Image/b7b.png",	1, 1, 1, 32, 32, 0, 4.0), "bullet01", 1);
	resManager.Add(new CBulletImage("Image/b5b.png",	2, 2, 1, 16, 64, 6, 0.0), "bullet02", 2);
	resManager.Add(new CBulletImage("Image/bullet5.png",4, 4, 1, 32, 32, 8, 0.0), "bullet03", 3);

	//--------------
	//	弾幕関係
	//--------------
	CBulletManager *bulletManager = new CBulletManager(4096);
	CBaseBullet::SetRect(CRect(0,0,WindowX,WindowY));
	CBaseBullet::SetResource(&resManager);
	CNormalBullet::SetBulletManagerPointer(bulletManager);

//●毎フレーム更新しなければならない
	CBaseBullet::SetTarget(CPos(p,0));

//●メイン使用方法
	bulletManager->Add( new CBaseBullet(EDirType::Abs, CPos(100,100), 3, abg, 0,0,0, "bullet00"));
	bulletManager->Add( new CBaseBullet(EDirType::Abs, CPos(200,100), 3, abg, 0,0,0, 1));

	bulletManager->Action();
	bulletManager->Draw();


//●解放
	delete bulletManager;

*/
