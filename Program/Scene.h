#pragma once

#include "DxLib.h"
#include "DXFunc.h"
#include "Input.h"
#include "Func.h"
//#include "CBaseEffect.h"
//#include "CCustomBullet.h"
#include "Resource.h"

class CSceneManager;
class CScene;
class CMenu;

//フェードステータス		
const int PREFEED = 0;	//フェード前(0)
const int NOWFEED = 1;	//フェード中(1)
const int ENDFEED = 2;	//フェード終了(2)

//------------------------------
//	シーンはシングルトンにしときましょ
//------------------------------
class CSceneManager{
public:
	CSceneManager();
	~CSceneManager();

	//シーンを積み上げていく
	//一番後ろのシーンが現在のシーン
	//その後ろのシーンの後ろにあるメニューは描画する

	//ex)シーン1→シーン2→シーン3→メニュ1→メニュ2
	//この場合　シーン3描画　メニュ1描画　メニュ2描画という感じ
	//要素を動的に確保する
	//CVoidArray m_scene;
	std::vector<CScene*> m_scene;

	//メインループ
	void Main(CInputAllStatus *input);

	//シーン追加	一番後ろに追加される
	void AddScene(CScene* Scene);

};


//場面
//タイトル場面だったり、ゲーム画面だったり
class CScene{
public:
	CScene(int InTime);
	virtual ~CScene();


	//画像データ設定　（渡すモノはnewで確保すること　解放はこのクラス(実際には上位)がする
	static void SetResource(CResourceManager *ResManager);
	//画像総合データのポインタ　コンストラクタで画像の読み込みを行う
	static CResourceManager *m_resManager;

	//シーンマネージャー　これが設定されていないとシーンがシーンを追加できない
	//staticなのは不服だけど良い案なし・・・
	static CSceneManager* m_sceneManager;
	static void SetSceneManager(CSceneManager *SceneManager);

	//行動	引数isTopSceneは一番上の描画かどうか
	virtual void Main(CInputAllStatus *input);

	void Init();
	void Set(int InTime);

	//---------------------
	//フェードイン、アウト
	//---------------------

	//フェードに掛ける時間
	int m_inTime;
	int m_outTime;

	//フェードの時間を記憶しておく
	int m_inNowTime;
	int m_outNowTime;

	//フェードの状態について
	//フェード前(0)	フェード中(1)	フェード終了(2)
	//const int PREFEED = 0;	//フェード前(0)
	//const int NOWFEED = 1;	//フェード中(1)
	//const int ENDFEED = 2;	//フェード終了(2)
	char m_inStatus;
	char m_outStatus;

	//フェード用の濃度(0.0～1.0)
	//これで暗転やたらいろいろ表現する
	double m_inBlend;
	double m_outBlend;

	virtual void FeedIn();
	virtual void FeedOut();
	//フェード中か
	bool NowFeed() const;

	//フェードアウトをするかどうか
	//フェードアウトしたくなったらtrueにして、時間を設定する
	bool m_outFlg;
	void SetFeedOut(int OutTime);

	//次のシーンに以降する
	void SetNextScene(CScene* NextScene);
	//現在のシーンを削除して１つ手前に戻る
	void SetBackScene();

	//どちらかが設定されている場合、もう片方は設定できない。
	bool m_backScene;
	CScene* m_nextScene;


	//フェードアウトが終わっている　か　フェードアウトが設定されておらず
	//かつ　m_nextSceneが設定されている場合
	//シーンを遷移させる
	void ChangeScene();

	//---------------------
	//その他
	//---------------------

	//全シーンに必要そうなもの
	int m_count;

};


//----------------------------------------------------------
//	例
//----------------------------------------------------------

/*
//例）
CSceneExMain	+	CSceneExSub1	+ Menu
									+ Menu

				+	CSceneExSub2	+ Menu
				+	CSceneExSub3	+ Menu

*/
class CSceneExMain : public CScene{
public:
	//------------------
	//	おまじない
	//------------------
	CSceneExMain(int InTime);
	virtual ~CSceneExMain();

	//行動	引数isTopSceneは一番上の描画かどうかである。上から渡されるので気にしなくて良い
	virtual void Main(CInputAllStatus *input);

	//------------------
	//	ここから下は自由に
	//------------------

	int font1;
	int font2;

	//CEffectManager *effectManeger1;

	int selectPos;

};

class CSceneExSub1 : public CScene{
public:
	//------------------
	//	おまじない
	//------------------
	CSceneExSub1(int InTime);
	virtual ~CSceneExSub1();

	virtual void Main(CInputAllStatus *input);

	//------------------
	//	ここから下は自由に
	//------------------

	int m_font;

	//CBulletManager *bulletManeger1;

	int selectPos;
};



/*
How To Use

CSceneExMain
CSceneExSub1



*/