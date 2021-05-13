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
	{
		std::vector<int> indexAry, indexAry1, indexAry2, indexAry3, indexAry4, indexAry5, indexAry6, indexAry7;
		CFunc::GetDigitArray(1, 12, indexAry1);
		CFunc::GetDigitArray(2, 12, indexAry2);
		CFunc::GetDigitArray(3, 12, indexAry3);
		CFunc::GetDigitArray(4, 12, indexAry4);
		CFunc::GetDigitArray(5, 12, indexAry5);
		CFunc::GetDigitArray(6, 12, indexAry6);
		CFunc::GetDigitArray(7, 12, indexAry7);
	}

	std::vector<int> indexAry, indexAry1, indexAry2, indexAry3, indexAry4, indexAry5, indexAry6, indexAry7;
	CFunc::GetDigitArray(8, 1234, indexAry1);
	CFunc::GetDigitArray(8, 12345, indexAry2);
	CFunc::GetDigitArray(8, 123456, indexAry3);
	CFunc::GetDigitArray(8, 1234567, indexAry4);
	CFunc::GetDigitArray(8, 12345678, indexAry5);
	CFunc::GetDigitArray(8, 0, indexAry6);

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
