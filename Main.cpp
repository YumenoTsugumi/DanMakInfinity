#include "DxLib.h"
#include "math.h"

#include "Func.h"
#include "DXFunc.h"
#include "Game.h"

#include "GameDefine.h"

#include <random>


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	double gameRatio = 1.0;
	CGame::SetAllGameRect(gameRatio);
	CDxFunc::SetWindowRatio(gameRatio);

	SetGraphMode(WindowX * gameRatio, WindowY * gameRatio, 32); // ウィンドウサイズ
	ChangeWindowMode(true); //ウィンドウモード
	SetOutApplicationLogValidFlag(false); //ログを吐かない
	SetAlwaysRunFlag(TRUE); //アクティブでなくても処理を行う
	SetMainWindowText("DanMakInfinity"); // ウィンドウタイトル
	
	if (DxLib_Init() == -1) {//DXライブラリ初期化
		return -1;
	}

	CPos p1_s(-10, -5);
	CPos p1_e(10, 20);
	CPos p2_s(0, -25);
	CPos p2_e(0, -10);
	CPos xp;
	double l, r;
	bool chk = CFunc::CalcIntersectionPoint(p1_s, p1_e, p2_s, p2_e, xp, l, r);

	SetDrawScreen(DX_SCREEN_BACK); //ダブルバッファを有効に
	srand((unsigned)time(nullptr)); //乱数初期化
	SetDrawMode(DX_DRAWMODE_BILINEAR);//バイナリア描画を使用する

	CGame* game = new CGame();
	game->Init();

	//クライアント領域矩形
	//game->SetRect(CRect(0, 0, windowX, windowY));

	game->CreateBattleScene();

	game->Main();

	DxLib_End(); // 終了処理

	return 0;				// ソフトの終了 
}
