#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"

#include <functional>

constexpr int SelectItemNum = 5;
constexpr int SelectRankItemNum = 6;
constexpr int SelectRankItems[SelectRankItemNum] = {1,20,40,60,80,100};

constexpr int SelectWeaponMenuNum = 5;
constexpr int SelectWeaponNum = 4; // 武器自体は4つ

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
	static void Result(CTitleScene* thisScene);
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
	int m_selectItemSizeWidth[SelectItemNum];
	int m_selectItemSizeMaxWidth;

	int m_topTitleAnimeCount;
	bool m_top_animetionFinishFlag;
	int m_top_textBlinkAnimeCount;
	//------------------------------------------------------
	void Draw_PlayStandby();
	void Action_PlayStandby(CInputAllStatus* input);
	CImage* m_WeaponSelect_StartRank;
	int m_playStandby_selectIndex;
	CImage* m_playStandby_selectItem[SelectRankItemNum][2]; // 非アクティブ、アクティブ
	int m_playStandby_selectItemSizeWidth[SelectItemNum];
	int m_playStandby_selectItemSizeMaxWidth;

	// 武器選択画面
	void Draw_WeaponSelect();
	void Action_WeaponSelect(CInputAllStatus* input);
	CImage* m_WeaponSelect_SelectWeapon;
	CImage* m_WeaponSelect_Left;
	CImage* m_WeaponSelect_Right;
	CImage* m_WeaponSelect_Rapid;
	CImage* m_WeaponSelect_Slow;
	CImage* m_WeaponSelect_Speed[2]; // 移動速度
	CImage* m_WeaponSelect_ShotType[2]; // ショットタイプ
	CImage* m_WeaponSelect_Sortie[2]; // 出撃
	CImage* m_WeaponSelect_Weapon[4][2]; // 武器
	CImage* m_WeaponSelect_Number[10][2]; // 数値
	int m_WeaponSelect_selectIndex;
	int m_SelectedSpeed[2]; // ラピッド　スロー
	int m_SelectedWeapon[2]; // ラピッド　スロー

	// リザルト画面
	void Draw_Result();
	void Draw_ResultSub(int posY, int number, bool nulldata,SaveDatus& s);
	void Action_Result(CInputAllStatus* input);
	CImage* m_ResultImage;
	int m_resultIndex;
	CImage* m_strScoreImage[10];
	CImage* m_strRankingGold;
	CImage* m_strRankingSliver;
	CImage* m_strRankingBronzeImage[10];
	CImage* m_strImage[37];
	CImage* m_strNumber[10];
	CImage* m_colonImage;
	CImage* m_slashImage;


	// シーンに戻ってきた時の状態
	enum SceneReturnStatus {
		Nothing = 0, // 本当に最初
		Retry,
		GoTitle,
		GameOver,
		GoResult
	};
	SceneReturnStatus m_sceneReturnStatus;
	void SetReturnStatus_Retry();
	void SetReturnStatus_GoTitle();
	// 開始ランク,終了ランク,生存時間,ラピッドショット,スローショット,アイテム数,スコア
	void SetReturnStatus_GameOver(
		int startRank, int endRank, 
		int stage, int item, int liveTime, 
		int rapidShot, int rapidspeed, int slowShot, int slowspeed, long long score);
	void SetReturnStatus_GoResult();

	// 画面の状況
	enum Status{
		Top = 0, // TOP画面
		PlayStandby, // Play押して、ランクや自機を選ぶ
		PlayWeaponSelect, // ランクを選んだ後に自機を選ぶ
		ExtraStandby,
		ShowResult, // リザルト画面
	};
	Status m_status;
};