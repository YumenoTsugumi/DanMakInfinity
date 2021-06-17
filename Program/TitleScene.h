#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"

#include <functional>

constexpr int SelectItemNum = 5;
constexpr int SelectRankItemNum = 6;

class CTitleScene : public CScene{
public:
	//------------------
	//	おまじない
	//------------------
	CTitleScene(int InTime);
	virtual ~CTitleScene();

	//行動	引数isTopSceneは一番上の描画かどうかである。上から渡されるので気にしなくて良い
	virtual void Main(CInputAllStatus *input);

	//------------------
	//	ここから下は自由に
	//------------------
	void Init(CGame* gameP);



	// 関数ポインタ
	std::function<void(CTitleScene*)> m_selectItemFunc[SelectItemNum];

	static void Play(CTitleScene* thisScene);
	static void Extra(CTitleScene* thisScene);
	static void Data(CTitleScene* thisScene);
	static void Config(CTitleScene* thisScene);
	static void Exit(CTitleScene* thisScene);

	CGame *m_game; // 上位のゲームクラス


	//------------------------------------------------------
	void Draw();
	void Action(CInputAllStatus* input);
	// ロゴﾄﾞｰﾝ
	CPos m_titleLog1Pos;
	double m_titleLog1Size;
	double m_titleLog1Alpha;

	CImage* m_titleLog1;

	CPos m_titleLog2Pos;
	double m_titleLog2Size;
	double m_titleLog2Alpha;
	double m_titleLog2Angle;
	CPos m_titleLog3Pos;
	double m_titleLog3Size;
	double m_titleLog3Alpha;
	CImage* m_titleLog2;
	CImage* m_titleLog3;

	// 背景
	CImage* m_background;
	CImage* m_planet1;
	CImage* m_planet2;
	CImage* m_planet3;

	// 選択肢
	int m_selectIndex;
	CImage* m_selectItem[SelectItemNum][2]; // 非アクティブ、アクティブ
	int m_selectItemSizeWigth[SelectItemNum];
	int m_selectItemSizeMaxWigth;

	int m_topTitleAnimeCount;
	bool m_top_animetionFinishFlag;
	int m_top_textBlinkAnimeCount;
	//------------------------------------------------------
	void Draw_PlayStandby();
	void Action_PlayStandby(CInputAllStatus* input);
	int m_playStandby_selectIndex;
	CImage* m_playStandby_selectItem[SelectRankItemNum][2]; // 非アクティブ、アクティブ
	int m_playStandby_selectItemSizeWigth[SelectItemNum];
	int m_playStandby_selectItemSizeMaxWigth;

	// 画面の状況
	enum Status{
		Top = 0, // TOP画面
		PlayStandby, // Play押して、ランクや自機を選ぶ
		ExtraStandby,

	};
	Status m_status;
};