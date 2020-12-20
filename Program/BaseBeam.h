#pragma once

#include "Func.h"
#include "BaseBullet.h"

class CBeamImage;
class CBeamImageManager;
class CBaseBeamManager;
class CBaseBeam;
class CBaseBeamChild;

//---------------------------------------------------------------------------------
//	CBaseBeamManager
//---------------------------------------------------------------------------------
class CBaseBeamManager {
public:
	//コンストラクタ
	//弾幕の発射最大数を設定する
	//デフォルト64
	CBaseBeamManager(int num = 64);
	~CBaseBeamManager();

	//発射最大数	64
	//コンストラクタで設定後は変更不可
	int m_beamTotalNum;

	//弾の挿入位置	徐々にずらし弾が重ならないようにするため
	int m_order;

	//弾データ	弾クラスを代入する
	CBaseBeam** m_beam;

	virtual void Action();
	virtual void Draw();

	int Add(CBaseBeam* beam);

};

//---------------------------------------------------------------------------------
//	CBaseBeam
//---------------------------------------------------------------------------------

//レーザーはn個の弾で構成される
//Draw方法だけ描画を変える
class CBaseBeam : public CBulletManager {
	//表示角度(rad)
	double m_drawAngle;
	double m_drawSize;

public:
	//画像へのポインタ
	//CBeamImage *m_image;
	CImageSet* m_image;

	//画像データ設定　（渡すモノはnewで確保すること　解放はこのクラス(実際には上位)がする
	static void SetResource(CResourceManager* ImageManager);
	//画像総合データのポインタ　コンストラクタで画像の読み込みを行う
	static CResourceManager* m_imageManager;

	//ちょっと広めの範囲
	static CRect m_bigRect;
	static void SetBigRect(CRect& bigrect);

	//コンストラクタ
	//レーザー１本の発射最大数を設定する
	//デフォルト128で構成される
	//800*600で端から端まで　速度10間隔1なら128で十分
	CBaseBeam(CPos P, double ANGLE, int IMGNO, int num = 128);
	CBaseBeam(CPos P, double ANGLE, const char* IMGNAME, int num = 128);
	virtual ~CBaseBeam();
	virtual void Set(CPos pos, double m_speed, double m_angle, int length, double m_thick, int m_shotSpan);
	virtual void SetImage(int IMG);
	virtual void SetImage(const char* IMAGENAME);

	//位置
	CPos m_pos;
	//発射角度(deg)
	double m_angle;
	//長さ(距離では無く、カウント
	int m_length;
	void SetLength(int Length);
	//画像種類
	int m_imgNo;

	//ビーム照射までの待機時間
	int m_wait;
	void SetWait(int Wait);

	//削除時間　発生してから
	int m_removeTime;
	void SetRemoveTime(int DelTime);
	void SetRemove();
	//削除	Yesで削除中
	bool m_removeNow;
	//削除中に小さくしていくため　基本は1.0倍　→0.0倍まで
	double m_removeSize;

	//速度	いるかな…
	double m_speed;
	//太さ	1.0倍がデフォルト
	double m_thick;
	//発射間隔	いるかな…
	int m_shotSpan;

	//ビーム移動回転	移動先の座標	回転量　5.0度動く
	void Move(CPos& pos, double plusAngle);

	//削除フラグ	管理クラスが見る
	bool m_removeFlg;
	//生存時間
	int m_count;

	virtual void Action();
	virtual void Draw();

	//要素削除の前　エフェクト出したり
	void Remove();
	//要素削除
	void Clear();
};

//---------------------------------------------------------------------------------
//	CBaseBeamChild
//---------------------------------------------------------------------------------
//ビームの破片を弾で表現する
class CBaseBeamChild : public CBaseBullet {
public:

	//コンストラクタ	弾作成
	CBaseBeamChild(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG);
	//デストラクタ
	virtual~CBaseBeamChild();

	//範囲外
	virtual void RectOut();

	//エフェクト出したくないから
	virtual void Remove();

	//自分の位置（射出されるたびインクリメントされる
	//子供全体で増えていく
	int idx;
};



//--------------------------------------------------------------------------------------------------------------
//	How To Use
//--------------------------------------------------------------------------------------------------------------
/*

	//--------------
	//ビーム関係初期化
	//--------------
	//削除範囲は少し広めに取る
	CBaseBeamChild::SetBigRect(CRect(	0 - 100,
										0 - 100,
										800 + 100,
										600 + 100));
	//画像
	//レーザー用の画像読み込み
	{	const char fileA[][RESOURCENAMEMAX] = {	"Image/bulletstand_b.png","Image/childLaser_b.png"	};
		resManager.Add(new CImageSet(2, fileA), "laser01", 100);	}
	{	const char fileA[][RESOURCENAMEMAX] = {	"Image/bulletstand_r.png","Image/childLaser_r.png"	};
		resManager.Add(new CImageSet(2, fileA), "laser02", 101);	}

	//ビーム管理クラス
	CBaseBeamManager *beamManager = new CBaseBeamManager(64);


	//普通に発射
	CPos ppp(300,300);
	CPos vel(1.0,1.0);
	double ang = 0.0;
	CBaseBeam *beam1 = new CBaseBeam(ppp, ang, 0);
	beamManager->Add(beam1);

	//ループ処理
	while(ProcessMessage() == 0){
		ClearDrawScreen();

		//移動回転
		ppp += vel;
		beam1->Move(ppp, 1.0);



		CPos p(400, 100);
		CBaseBeam *m_beam = new CBaseBeam(p, m_angle, 0);

		//長さ設定 1～64まで	ピクセルで言うと*10まで伸びる
		m_beam->SetLength(32);
		//照射までの待機時間　fr単位
		m_beam->SetWait(24);
		//削除時間
		m_beam->SetRemoveTime(120);

		beamManager->Add(m_beam);
	}


//※


	//回転とかで毎フレーム参照されると落ちるので
	//ビームは管理側がSetRemoveしないと削除しない

	//削除時間を超えたら削除
	//消すには、deltimeを設定するか管理側がSetRemoveする

*/











