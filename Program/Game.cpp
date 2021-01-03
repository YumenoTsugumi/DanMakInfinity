#include "Game.h"

#include "Input.h"
#include "BattleScene.h"
#include "BaseBeam.h"

#include <thread>
#include <chrono>

CResourceManager CGame::m_resourceManager;
CRect CGame::m_battleRect;

CGame::CGame() :
	m_fpsManager(),
	m_input(),
	m_sceneManager()
{

}

CGame::~CGame() {

}

void CGame::Init()
{

	// インプット管理クラスに、パッドによる操作と、キーボードによる操作を登録する
	m_input.SetPad(INPUT_DEF_DOWN, INPUT_PAD_DOWN);
	m_input.SetPad(INPUT_DEF_LEFT, INPUT_PAD_LEFT);
	m_input.SetPad(INPUT_DEF_UP, INPUT_PAD_UP);
	m_input.SetPad(INPUT_DEF_RIGHT, INPUT_PAD_RIGHT);
	m_input.SetPad(INPUT_DEF_ENTER, INPUT_PAD_BTN1);
	m_input.SetPad(INPUT_DEF_CANCEL, INPUT_PAD_BTN2);

	m_input.SetKeyboard(INPUT_DEF_DOWN, KEY_INPUT_DOWN);
	m_input.SetKeyboard(INPUT_DEF_LEFT, KEY_INPUT_LEFT);
	m_input.SetKeyboard(INPUT_DEF_UP, KEY_INPUT_UP);
	m_input.SetKeyboard(INPUT_DEF_RIGHT, KEY_INPUT_RIGHT);
	m_input.SetKeyboard(INPUT_DEF_ENTER, INPUT_KEY_Z);
	m_input.SetKeyboard(INPUT_DEF_CANCEL, INPUT_KEY_X);


	std::thread th(CGame::ImageLoadByThread);
	th.detach();

	ImageLoad();

	// ゲーム弾が見える範囲
	CGame::SetBattleRect(CRect(0, 0, 800, 600));

}

void CGame::CreateBattleScene()
{
	CBattleScene* battleScene = new CBattleScene(0);
	battleScene->Init(this);	//シーンにゲームクラスポインタを渡す
	m_sceneManager.AddScene(battleScene);
}

void CGame::Main() {

	// メインループ処理
	while (ProcessMessage() == 0) {
		m_fpsManager.Start(); //fpsStart管理
		ClearDrawScreen();//描画範囲削除

		//入力
		m_input.GetState();
		CInputAllStatus* inputData = m_input.GetInputStatus();

		// 入力のデバッグ情報表示
		//m_input.DebugPrint();

		// シーンの実行
		m_sceneManager.Main(inputData);

		ScreenFlip(); //描画範囲反映
		
		clsDx(); //printfDx削除
		m_fpsManager.End(); //fpsEnd管理

	}
}

void CGame::ImageLoad()
{

	//弾幕画像(試作型)
	//ふぁいる、総数、X、Y、サイズX、サイズY、アニメ速度、回転速度
	m_resourceManager.Add(new CBulletImage("Resource\\bullet00.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet00", 00);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet01.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet01", 01);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet02.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet02", 02);

	m_resourceManager.Add(new CBulletImage("Resource\\bullet10.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet10", 10);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet11.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet11", 11);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet12.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet12", 12);

	m_resourceManager.Add(new CBulletImage("Resource\\bullet20.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet20", 20);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet21.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet21", 21);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet22.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet22", 22);

	m_resourceManager.Add(new CImage("Resource\\pChan.png"), "pChan", 999);

	//画像
	//レーザー用の画像読み込み
	m_resourceManager.Add(new CImage("Resource\\bulletstand_b.png"), "laser01_stand", 100);
	m_resourceManager.Add(new CImage("Resource\\childLaser_b.png"), "laser01_laser", 101);

	m_resourceManager.Add(new CImage("Resource\\bulletstand_r.png"), "laser02_stand", 110);
	m_resourceManager.Add(new CImage("Resource\\childLaser_r.png"), "laser02_laser", 111);

	// homingLaser
	m_resourceManager.Add(new CBulletImage("Resource\\h00.png", 1, 1, 1, 64, 576, 0, 0.0), "hLaser01", 200);
	m_resourceManager.Add(new CBulletImage("Resource\\h01.png", 1, 1, 1, 64, 576, 0, 0.0), "hLaser02", 201);
	m_resourceManager.Add(new CBulletImage("Resource\\h02.png", 1, 1, 1, 64, 576, 0, 0.0), "hLaser03", 202);

}
void CGame::ImageLoadByThread()
{

	auto start = std::chrono::system_clock::now();      // 計測スタート時刻を保存

	// 背景
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_00.jpg"), "X_bg_space_00", 10000);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_01.jpg"), "X_bg_space_01", 10001);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_02.jpg"), "X_bg_space_02", 10002);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_03.jpg"), "X_bg_space_03", 10003);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_04.jpg"), "X_bg_space_04", 10004);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_05.jpg"), "X_bg_space_05", 10005);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_06.jpg"), "X_bg_space_06", 10006);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_07.jpg"), "X_bg_space_07", 10007);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_08.jpg"), "X_bg_space_08", 10008);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_09.jpg"), "X_bg_space_09", 10009);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_10.jpg"), "X_bg_space_10", 10010);

	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_b00.jpg"), "X_bg_space_b00", 10011);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_b01.jpg"), "X_bg_space_b01", 10012);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_b02.jpg"), "X_bg_space_b02", 10013);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_b03.jpg"), "X_bg_space_b03", 10014);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_b04.jpg"), "X_bg_space_b04", 10015);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_space_b05.jpg"), "X_bg_space_b05", 10016);

	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_00.png"), "bg_planet_00", 10100);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_01.png"), "bg_planet_01", 10101);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_02.png"), "bg_planet_02", 10102);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_03.png"), "bg_planet_03", 10103);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_04.png"), "bg_planet_04", 10104);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_05.png"), "bg_planet_05", 10105);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_06.png"), "bg_planet_06", 10106);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_07.png"), "bg_planet_07", 10107);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_08.png"), "bg_planet_08", 10108);
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_09.png"), "bg_planet_09", 10109);


	// 敵
	for (int ii = 0; ii < 24; ii++) {
		std::string format1 = MyFormat("ResourceX\\Enemy\\blue\\illumBlue%02d.png", ii + 1);
		std::string format2 = MyFormat("enemy%02db", ii + 1);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 20000 + ii);
		//m_resourceManager.Add(new CImage("ResourceX\\Enemy\\illumBlue01.png"), "enemy01b", 20001);
		//m_resourceManager.Add(new CImage("ResourceX\\Enemy\\illumBlue24.png"), "enemy24b", 20024);
	}
	for (int ii = 0; ii < 24; ii++) {
		std::string format1 = MyFormat("ResourceX\\Enemy\\green\\illumGreen%02d.png", ii + 1);
		std::string format2 = MyFormat("enemy%02dg", ii + 1);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 20050 + ii);
		//m_resourceManager.Add(new CImage("ResourceX\\Enemy\\illumGreen01.png"), "enemy01g", 20051);
		//m_resourceManager.Add(new CImage("ResourceX\\Enemy\\illumGreen24.png"), "enemy24g", 20074);
	}
	for (int ii = 0; ii < 24; ii++) {
		std::string format1 = MyFormat("ResourceX\\Enemy\\red\\illumRed%02d.png", ii + 1);
		std::string format2 = MyFormat("enemy%02dr", ii + 1);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 20100 + ii);
		//m_resourceManager.Add(new CImage("ResourceX\\Enemy\\illumRed01.png"), "enemy01r", 20101);
		//m_resourceManager.Add(new CImage("ResourceX\\Enemy\\illumRed24.png"), "enemy24r", 20124);
	}

	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
	auto dur = end - start;        // 要した時間を計算
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	// 要した時間をミリ秒（1/1000秒）に変換して表示
	printfDx(" milli sec %d", msec);
}