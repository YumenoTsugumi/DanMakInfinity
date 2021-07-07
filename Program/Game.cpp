#include "Game.h"

#include "Input.h"
#include "BattleScene.h"
#include "TitleScene.h"
#include "BaseBeam.h"

#include <thread>
#include <chrono>
#include <stdio.h>
#include <direct.h>

CResourceManager CGame::m_resourceManager;
std::vector<SaveDatus> CGame::m_saveData;
CPos CGame::m_allGameRect; // 全体　(1920,1080)
CPos CGame::m_gameRectLT; // ゲーム内 (480,20)
CPos CGame::m_gameRectRB; // ゲーム内 (1440,1060)
double CGame::m_windowRatio;
CGame::CGame() :
	m_fpsManager(),
	m_input(),
	m_sceneManager(),
	m_exitFlag(false)
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

	CGame::ImageLoadByThread(); // 不具合が多いので普通に読み込む
	//std::thread th(CGame::ImageLoadByThread);
	//th.detach();

	ImageLoad();

	// ゲーム弾が見える範囲
	//CGame::SetAllGameRect(0.5);

}

void CGame::CreateBattleScene()
{
	CBattleScene* battleScene = new CBattleScene(0);
	battleScene->Init(this, 50, 7,0, 4,1);	//シーンにゲームクラスポインタを渡す
	m_sceneManager.AddScene(battleScene);
}

void CGame::CreateTitleScene()
{
	CTitleScene* titleScene = new CTitleScene(0);
	titleScene->Init(this);	//シーンにゲームクラスポインタを渡す
	m_sceneManager.AddScene(titleScene);
}

void CGame::Main() {

	m_input.SetKeyboard(INPUT_DEF_SLOW, INPUT_KEY_LSHIFT);
	m_input.SetKeyboard(INPUT_DEF_ESC, INPUT_KEY_ESCAPE);

	CScene::SetSceneManager(&m_sceneManager);

	// メインループ処理
	while (ProcessMessage() == 0) {
		m_fpsManager.Start(); //fpsStart管理
		ClearDrawScreen();//描画範囲削除

		//入力
		m_input.GetState();
		m_input.DebugPrint();
		CInputAllStatus* inputData = m_input.GetInputStatus();

		// 入力のデバッグ情報表示
		//m_input.DebugPrint();

		// シーンの実行
		m_sceneManager.Main(inputData);

		printfDx("%f",1000.0 / m_fpsManager.GetFPS());

		

		ScreenFlip(); //描画範囲反映
		
		clsDx(); //printfDx削除
		m_fpsManager.End(); //fpsEnd管理

		if (m_exitFlag) {
			break;
		}
	}
}

void CGame::ImageLoad()
{

	//弾幕画像(試作型)
	//ふぁいる、総数、X、Y、サイズX、サイズY、アニメ速度、回転速度
		

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet01.png", 6, 6, 1, 48, 48, 5, 0.0), "bullet01", 01);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet02.png", 6, 6, 1, 48, 48, 5, 0.0), "bullet02", 02);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet11.png", 4, 4, 1, 24, 48, 5, 0.0), "bullet11", 11);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet12.png", 4, 4, 1, 24, 48, 5, 0.0), "bullet12", 12);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet21.png", 4, 4, 1, 48, 72, 6, 0.0), "bullet21", 21);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet22.png", 4, 4, 1, 48, 72, 6, 0.0), "bullet22", 22);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet31.png", 1, 1, 1, 32, 48, 0, 4.0), "bullet31", 31);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet32.png", 1, 1, 1, 32, 48, 0, 4.0), "bullet32", 32);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet41.png", 1, 1, 1, 32, 64, 0, 0.0), "bullet41", 41);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet42.png", 1, 1, 1, 32, 64, 0, 0.0), "bullet42", 42);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet51.png", 1, 1, 1, 18, 90, 0, 0.0), "bullet51", 51);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet52.png", 1, 1, 1, 18, 90, 0, 0.0), "bullet52", 52);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet61.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet61", 61);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet62.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet62", 62);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet81.png", 1, 1, 1, 28, 60, 0, 0.0), "bullet81", 81);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet82.png", 1, 1, 1, 25, 60, 0, 0.0), "bullet82", 82);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet91.png", 2, 2, 1, 37, 54, 6, 0.0), "bullet91", 91);
	m_resourceManager.Add(new CBulletImage("ResourceX\\Bullet\\bullet92.png", 2, 2, 1, 37, 54, 6, 0.0), "bullet92", 92);


	//m_resourceManager.Add(new CBulletImage("Resource\\bullet02.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet02", 02);

	//m_resourceManager.Add(new CBulletImage("Resource\\bullet10.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet10", 10);
	//m_resourceManager.Add(new CBulletImage("Resource\\bullet11.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet11", 11);
	//m_resourceManager.Add(new CBulletImage("Resource\\bullet12.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet12", 12);

	//m_resourceManager.Add(new CBulletImage("Resource\\bullet20.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet20", 20);
	//m_resourceManager.Add(new CBulletImage("Resource\\bullet21.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet21", 21);
	//m_resourceManager.Add(new CBulletImage("Resource\\bullet22.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet22", 22);

	m_resourceManager.Add(new CImage("Resource\\auraBlue.png"), "auraBlue", 500);
	m_resourceManager.Add(new CImage("Resource\\auraRed.png"), "auraRed", 501);


	m_resourceManager.Add(new CImage("ResourceX\\player\\playerA.png"), "playerA", 800);
	m_resourceManager.Add(new CImage("ResourceX\\player\\BitR.png"), "BitR", 801);
	m_resourceManager.Add(new CImage("ResourceX\\player\\playerB.png"), "playerB", 810);
	m_resourceManager.Add(new CImage("ResourceX\\player\\playerC.png"), "playerC", 820);

	m_resourceManager.Add(new CImage("ResourceX\\player\\hitMaker.png"), "hitMaker.png", 850);

	m_resourceManager.Add(new CImage("ResourceX\\player\\playerBarrier1.png"), "playerBarrier1", 980);
	m_resourceManager.Add(new CImage("ResourceX\\player\\playerBarrier2.png"), "playerBarrier2", 981);

	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\fireball_0.png", 1, 1, 1, 256, 256, 0, 0.0), "fireball_0", 982);
	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\fireball_1.png", 1, 1, 1, 256, 256, 0, 0.0), "fireball_1", 983);
	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\fireball_2.png", 1, 1, 1, 256, 256, 0, 0.0), "fireball_2", 984);
	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\fireball_3.png", 1, 1, 1, 256, 256, 0, 0.0), "fireball_3", 985);

	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\bombLine.png", 1, 1, 1, 64, 576, 0, 0.0), "bombLine", 986);
	m_resourceManager.Add(new CImage("ResourceX\\player\\BombIcon.png"), "BombIcon", 987);
	m_resourceManager.Add(new CImage("ResourceX\\player\\BombText.png"), "BombText", 988);

	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\PlayerBulletA.png", 1, 1, 1, 40, 40, 0, 0.0), "playerBulletA", 990);
	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\playerBullet.png", 1, 1, 1, 64, 128, 0, 0.0), "playerBullet", 998);
	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\playerBulletM.png", 1, 1, 1, 100, 200, 0, 0.0), "playerBulletM", 997);

	m_resourceManager.Add(new CBulletImage("ResourceX\\player\\rens.png", 1, 1, 1, 175, 175, 0, 0.0), "rens", 998);


	//m_resourceManager.Add(new CImage("Resource\\pChan.png"), "pChan", 999);

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


	// UI関連 1000～
	for (int ii = 0; ii < 10; ii++) {
		std::string format1 = MyFormat("Resource\\UI_fullhd2\\%d.png", ii);
		std::string format2 = MyFormat("%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 1000+ii);	
	}
	for (int ii = 0; ii < 10; ii++) {
		std::string format1 = MyFormat("Resource\\UI_fullhd2\\b%d.png", ii);
		std::string format2 = MyFormat("b%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 1010 + ii);
	}
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\0g.png"), "0g", 1110);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\comma.png"), "comma", 1111);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\commag.png"), "commag", 1112);

	for (int ii = 0; ii < 10; ii++) {
		std::string format1 = MyFormat("Resource\\UI_fullhd2\\s%d.png", ii);
		std::string format2 = MyFormat("s%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 1020 + ii);
	}
	m_resourceManager.Add(new CImages("Resource\\UI_fullhd2\\scoreText.png", 10, 10, 1, 80, 80), "scoreText", 1030);

	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\TextHiScore_p.png"), "TextHiScore_p", 1031);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\TextHiScore_c.png"), "TextHiScore_c", 1032);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\TextHiScore_d.png"), "TextHiScore_d", 1033);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\RankRatio.png"), "RankRatio", 1034);

	for (int ii = 0; ii < 10; ii++) {
		std::string format1 = MyFormat("Resource\\UI_fullhd2\\c%d.png", ii);
		std::string format2 = MyFormat("c%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 1040 + ii);
	}

	for (int ii = 0; ii < 10; ii++) {
		std::string format1 = MyFormat("Resource\\UI_fullhd2\\UIStageNumber%d.png", ii);
		std::string format2 = MyFormat("UIStageNumber%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 1050 + ii);
	}
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\UIStageText.png"), "UIStageText", 1060);


	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\TextScore.png"), "TextScore", 1150);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\TextHiScore.png"), "TextHiScore", 1151);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\RankWaku.png"), "RankWaku", 1152);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\RankWakuAura.png"), "RankWakuAura", 1153);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\RankWaku2.png"), "RankWaku2", 1154);

	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_A.png"), "ResultUI_A", 1201);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_AA.png"), "ResultUI_AA", 1202);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_AB.png"), "ResultUI_AB", 1203);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_AC.png"), "ResultUI_AC", 1204);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_B.png"), "ResultUI_B", 1205);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_C.png"), "ResultUI_C", 1206);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_D.png"), "ResultUI_D", 1207);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_E.png"), "ResultUI_E", 1208);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_F.png"), "ResultUI_F", 1209);
	for (int ii = 0; ii < 10; ii++) {
		std::string format1 = MyFormat("Resource\\UI_fullhd2\\ResultUI_N%d.png", ii);
		std::string format2 = MyFormat("ResultUI_N%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 1210 + ii);
	}
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_Np.png"), "ResultUI_Np", 1220);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_Nc.png"), "ResultUI_Nc", 1221);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\ResultUI_Nd.png"), "ResultUI_Nd", 1222);

	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\rankS.png"), "rankS", 1250);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\rankA.png"), "rankA", 1251);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\rankB.png"), "rankB", 1252);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\rankC.png"), "rankC", 1253);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\rankD.png"), "rankD", 1254);
	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\rankE.png"), "rankE", 1255);

	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\uiaura.png"), "uiaura", 1260);

	m_resourceManager.Add(new CImage("Resource\\UI_fullhd2\\UIFoundation.png"), "UIFoundation", 1500);

	m_resourceManager.Add(new CBulletImage("ResourceX\\Maker\\maker1.png", 1, 1, 1, 150, 150, 5, 0), "maker1", 1600);
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
	m_resourceManager.Add(new CImage("ResourceX\\BackGround\\bg_planet_09.png"), "bg_planet_09", 10109);

	// タイトル
	m_resourceManager.Add(new CImage("ResourceX\\Title\\TitleLog1.png"), "TitleLog1", 15001);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\TitleLog2.png"), "TitleLog2", 15002);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\TitleLog3.png"), "TitleLog3", 15003);

	m_resourceManager.Add(new CImage("ResourceX\\Title\\1.png"), "TitleLog_str1.", 15004);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\1a.png"),"TitleLog_str1a", 15005);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\2.png"), "TitleLog_str2.", 15006);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\2a.png"),"TitleLog_str2a", 15007);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\3.png"), "TitleLog_str3.", 15008);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\3a.png"),"TitleLog_str3a", 15009);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\4.png"), "TitleLog_str4.", 15010);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\4a.png"),"TitleLog_str4a", 15011);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\5.png"), "TitleLog_str5.", 15012);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\5a.png"),"TitleLog_str5a", 15013);

	m_resourceManager.Add(new CImage("ResourceX\\Title\\r01.png"), "TitleLog_strRank01", 15014);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r01a.png"), "TitleLog_strRank01a", 15015);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r20.png"), "TitleLog_strRank20", 15016);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r20a.png"), "TitleLog_strRank20a", 15017);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r40.png"), "TitleLog_strRank40", 15018);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r40a.png"), "TitleLog_strRank40a", 15019);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r60.png"), "TitleLog_strRank60", 15020);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r60a.png"), "TitleLog_strRank60a", 15021);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r80.png"), "TitleLog_strRank80", 15022);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r80a.png"), "TitleLog_strRank80a", 15023);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r100.png"), "TitleLog_strRank100", 15024);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\r100a.png"), "TitleLog_strRank100a", 15025);

	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Left.png"), "s_Left", 15026);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Right.png"), "s_Right", 15027);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Rapid.png"), "s_Rapid", 15028);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Slow.png"), "s_Slow", 15029);

	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_MoveSpeed.png"), "s_MoveSpeed", 15030);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_MoveSpeeda.png"), "s_MoveSpeeda", 15031);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_ShotType.png"), "s_ShotType", 15032);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_ShotTypea.png"), "s_ShotTypea", 15033);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Sortie.png"), "s_Sortie", 15034);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Sortiea.png"), "s_Sortiea", 15035);

	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Forward.png"), "s_Forward", 15036);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Forwarda.png"), "s_Forwarda", 15037);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Wide.png"), "s_Wide", 15038);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Widea.png"), "s_Widea", 15039);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Trace.png"), "s_Trace", 15040);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Tracea.png"), "s_Tracea", 15041);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Tilt.png"), "s_Tilt", 15042);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_Tilta.png"), "s_Tilta", 15043);


	for (int ii = 0; ii < 10; ii++) {
		std::string format1 = MyFormat("ResourceX\\Title\\s%d.png", ii);
		std::string format2 = MyFormat("sss%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 15044 + ii);
	}
	for (int ii = 0; ii < 10; ii++) {
		std::string format1 = MyFormat("ResourceX\\Title\\s%da.png", ii);
		std::string format2 = MyFormat("sss%da", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 15054 + ii);
	}

	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_StartRank.png"), "s_StartRank", 15065);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\s_SelectWeapon.png"), "s_SelectWeapon", 15066);

	m_resourceManager.Add(new CImage("ResourceX\\Title\\b_resume.png"), "b_resume", 15070);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\b_resumea.png"), "b_resumea", 15071);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\b_restart.png"), "b_restart", 15072);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\b_restarta.png"), "b_restarta", 15073);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\b_end.png"), "b_end", 15074);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\b_enda.png"), "b_enda", 15075);


	m_resourceManager.Add(new CImage("ResourceX\\timecounter.png"), "timecounter", 15076);
	m_resourceManager.Add(new CImage("ResourceX\\timecounter_off.png"), "timecounter_off", 15077);

	m_resourceManager.Add(new CImage("ResourceX\\resume3.png"), "resume3", 15078);
	m_resourceManager.Add(new CImage("ResourceX\\resume2.png"), "resume2", 15079);
	m_resourceManager.Add(new CImage("ResourceX\\resume1.png"), "resume1", 15080);

	m_resourceManager.Add(new CImage("ResourceX\\GameOver.png"), "GameOver", 15081);

	// リザルト画面
	m_resourceManager.Add(new CImage("ResourceX\\Result\\End.png"), "End", 15082);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\EndRank.png"), "EndRank", 15083);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\Item.png"), "Item", 15084);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\LiveTime.png"), "LiveTime", 15085);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\Name.png"), "Name", 15086);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\Score.png"), "Score", 15087);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\Small_s.png"), "Small_s", 15088);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\Stage.png"), "Stage", 15089);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\StartRank.png"), "StartRank", 15090);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\Enda.png"), "Enda", 15091);

	m_resourceManager.Add(new CImage("ResourceX\\Title\\Normal.png"), "Normal", 15092);

	m_resourceManager.Add(new CImage("ResourceX\\Title\\gold1.png"), "gold1", 15093);
	m_resourceManager.Add(new CImage("ResourceX\\Title\\silver2.png"), "silver2", 15094);
	for (int ii = 0; ii < (10); ii++) {
		std::string format1 = MyFormat("ResourceX\\Title\\bronze%d.png", ii);
		std::string format2 = MyFormat("bronze_%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 15095 + ii);
	}
	m_resourceManager.Add(new CImage("ResourceX\\Result\\coron.png"), "coron", 15110);
	m_resourceManager.Add(new CImage("ResourceX\\Result\\slash.png"), "slash", 15111);


	//            1234567890123456789012345678901234567
	char str[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_";
	for (int ii = 0; ii < (NameEntryFontMax); ii++) {
		std::string format1 = MyFormat("ResourceX\\Result\\%c.png", str[ii]);
		std::string format2 = MyFormat("Result_%c", str[ii]);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 16000 + ii);
	}
	for (int ii = 0; ii < (NameEntryFontMax); ii++) {
		std::string format1 = MyFormat("ResourceX\\Result\\%ca.png", str[ii]);
		std::string format2 = MyFormat("Result_%ca", str[ii]);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 16050 + ii);
	}

	// 敵 small
	for (int ii = 1; ii <= 7; ii++) {
		std::string format1 = MyFormat("ResourceX\\Enemy\\red\\s%d.png", ii);
		std::string format2 = MyFormat("enemyS%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 20000 + ii);
	}
	for (int ii = 1; ii <= 10; ii++) {
		std::string format1 = MyFormat("ResourceX\\Enemy\\red\\m%d.png", ii);
		std::string format2 = MyFormat("enemyM%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 20100 + ii);
	}
	for (int ii = 1; ii <= 4; ii++) {
		std::string format1 = MyFormat("ResourceX\\Enemy\\red\\l%d.png", ii);
		std::string format2 = MyFormat("enemyL%d", ii);
		m_resourceManager.Add(new CImage(format1.c_str()), format2.c_str(), 20200 + ii);
	}

	{
		std::vector<std::string> filenames;
		for (int ii = 0; ii < 35; ii++) {
			std::string format1 = MyFormat("ResourceX\\Fireball Explosion 2g\\FireballExplosion2_%04d.png", ii);
			filenames.push_back(format1);
		}
		m_resourceManager.Add(new CBulletImage(filenames, 256, 256, 1, 0), "FireballExplosion2", 20500);
	}
	{
		std::vector<std::string> filenames;
		for (int ii = 0; ii < 57; ii++) {
			std::string format1 = MyFormat("ResourceX\\Fireball Explosion 3g\\FireballExplosion3_%04d.png", ii);
			filenames.push_back(format1);
		}
		m_resourceManager.Add(new CBulletImage(filenames, 256, 256, 1, 0), "FireballExplosion3", 20501);
	}
	{
		std::vector<std::string> filenames;
		for (int ii = 0; ii < 47; ii++) {
			std::string format1 = MyFormat("ResourceX\\Fireball Explosion 7g\\FireballExplosion7_%04d.png", ii);
			filenames.push_back(format1);
		}
		m_resourceManager.Add(new CBulletImage(filenames, 256, 256, 1, 0), "FireballExplosion7", 20502);
	}
	{
		std::vector<std::string> filenames;
		for (int ii = 0; ii < 48; ii++) {
			std::string format1 = MyFormat("ResourceX\\Fireball Explosion 10g\\FireballExplosion10_%04d.png", ii);
			filenames.push_back(format1);
		}
		m_resourceManager.Add(new CBulletImage(filenames, 256, 256, 1, 0), "FireballExplosion10", 20503);
	}
	// ヒットエフェクト
	m_resourceManager.Add(new CBulletImage("ResourceX\\HitEffect\\0_.png", 16, 4, 4, 512, 512, 1, 0), "HitEffect0", 20600);
	m_resourceManager.Add(new CBulletImage("ResourceX\\HitEffect\\1.png", 16, 4, 4, 512, 512, 1, 0), "HitEffect1", 20601);
	m_resourceManager.Add(new CBulletImage("ResourceX\\HitEffect\\2.png", 16, 4, 4, 512, 512, 1, 0), "HitEffect2", 20602);
	m_resourceManager.Add(new CBulletImage("ResourceX\\HitEffect\\8.png", 16, 4, 4, 512, 512, 3, 0), "HitEffect8", 20608);


	//金塊
	for (int ii = 0; ii < 6; ii++) {
		std::string format1 = MyFormat("ResourceX\\Item\\ingot%d.png", ii);
		std::string format2 = MyFormat("ingot%d", ii);
		m_resourceManager.Add(new CBulletImage(format1.c_str(), 1, 1, 1, 400, 400, 0, 0), format2.c_str(), 20700 + ii);
	}
	// スコアアイテム
	m_resourceManager.Add(new CBulletImage("ResourceX\\scoreItem.png", 1, 1, 1, 64, 64, 0, 2), "ScoreItem", 20720);
	m_resourceManager.Add(new CBulletImage("ResourceX\\scoreitemAura.png", 1, 1, 1, 64, 64, 0, 2), "ScoreItemAura", 20721);


	// 弾が消えるエフェクト
	m_resourceManager.Add(new CBulletImage("ResourceX\\BulletDeleteEffect0.png", 4, 4, 1, 16, 16, 5, 0), "BulletDeleteEffect0", 20801);
	m_resourceManager.Add(new CBulletImage("ResourceX\\BulletDeleteEffect1.png", 4, 4, 1, 16, 16, 5, 0), "BulletDeleteEffect1", 20802);
	m_resourceManager.Add(new CBulletImage("ResourceX\\BulletDeleteEffect2.png", 4, 4, 1, 16, 16, 5, 0), "BulletDeleteEffect2", 20803);





	auto end = std::chrono::system_clock::now();       // 計測終了時刻を保存
	auto dur = end - start;        // 要した時間を計算
	auto msec = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
	// 要した時間をミリ秒（1/1000秒）に変換して表示
	printfDx(" milli sec %d", msec);
}


// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
// FulHDなら (480,20)(1440,1060)の範囲
CPos CGame::ToGamePos(CPos ratioPos)
{
	return ToGamePos(ratioPos.x, ratioPos.y);
}
CPos CGame::ToGamePos(double ratioPosX, double ratioPosY)
{
	return CPos(CGame::ToGamePosX(ratioPosX), CGame::ToGamePosY(ratioPosY));
}
// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
// FulHDなら (480～1440)の範囲
double CGame::ToGamePosX(double ratioPosX)
{
	int min = GameWindowAreaLeft * CGame::GetWindowRatio();
	int max = GameWindowAreaRight * CGame::GetWindowRatio();
	return 	min + (max - min) * (ratioPosX);
}
// 画面の位置を返却する 0.0～1.0で現在のウィンドウサイズでの位置が返ってくる
// FulHDなら (20～1060)の範囲
double CGame::ToGamePosY(double ratioPosY)
{
	int min = GameWindowAreaTop * CGame::GetWindowRatio();
	int max = GameWindowAreaBottom * CGame::GetWindowRatio();
	return 	min + (max - min) * (ratioPosY);
}

// 0.0～1.0で画面サイズに比例する大きさが返ってくる
double CGame::ToGameSizeX(double ratioPosX)
{
	int min = GameWindowAreaLeft * CGame::GetWindowRatio();
	int max = GameWindowAreaRight * CGame::GetWindowRatio();
	return (max - min) * (ratioPosX);
}
// 0.0～1.0で画面サイズに比例する大きさが返ってくる
double CGame::ToGameSizeY(double ratioPosY)
{
	int min = GameWindowAreaTop * CGame::GetWindowRatio();
	int max = GameWindowAreaBottom * CGame::GetWindowRatio();
	return 	(max - min) * (ratioPosY);
}

// 0.0～1.0で画面全体に比例する大きさが返ってくる
double CGame::ToAllSizeX(double ratioPosX)
{
	int max = WindowX * CGame::GetWindowRatio();
	return max * ratioPosX;
}
double CGame::ToAllSizeY(double ratioPosY)
{
	int max = WindowY * CGame::GetWindowRatio();
	return max * ratioPosY;
}

#include <algorithm>
void CGame::Load_SaveData()
{
	m_saveData.clear();

	std::vector<std::string> filepaths;
	CFunc::GetFileNames("resultData","*.dat", filepaths);
	for (auto path : filepaths) {
		SaveDatus saveData;
		SaveDatus::Load(path.c_str(), saveData);
		m_saveData.push_back(saveData);
	}

	std::sort(m_saveData.begin(), m_saveData.end(), [](SaveDatus a, SaveDatus b) {
		return a.m_score > b.m_score;
	});
}

void SaveDatus::Load(const char* filename, SaveDatus& datus)
{
	FILE *fp;
	std::string sss = "resultData\\";
	sss += filename;
	errno_t err = fopen_s(&fp, sss.c_str(), "rb");
	if (err == 0) {
		fread(&datus, sizeof(datus), 1, fp);
		fclose(fp);
	}
}

void SaveDatus::Save(
	int startRank,
	int endRank,
	int stage,
	int item,
	int liveTime,
	int rapidShot,
	int rapidspeed,
	int slowShot,
	int slowspeed,
	long long score,
	const struct tm &local,
	char *name,
	int gameMode
){
	SaveDatus datus;
	
	datus.m_ver = GameVer1;
	datus.m_startRank = startRank;
	datus.m_endRank = startRank;
	datus.m_stage = stage;
	datus.m_item = item;
	datus.m_liveTime = liveTime;
	datus.m_rapidShot = rapidShot;
	datus.m_rapidspeed = rapidspeed;
	datus.m_slowShot = slowShot;
	datus.m_slowspeed = slowspeed;
		
	datus.m_score = score;
		
	datus.m_year = local.tm_year + 1900;
	datus.m_mon = local.tm_mon + 1;
	datus.m_day = local.tm_mday;
	datus.m_hour = local.tm_hour;
	datus.m_min = local.tm_min;
	datus.m_sec = local.tm_sec;
		
	strcpy_s(datus.m_name, name);
		
	datus.m_gameMode = gameMode;
	datus.buf1 = -1;datus.buf2 = -1;datus.buf3 = -1;datus.buf4 = -1;datus.buf5 = -1;
	datus.buf6 = -1;datus.buf7 = -1;datus.buf8 = -1;datus.buf9 = -1;
	
	int a = datus.m_score;
	int b = datus.m_score;
	datus.checkA = a % 293; // scoreより上のチェック
	datus.checkB = a / 293; // scoreのチェック

	const char* dirname = "resultData";
	_mkdir(dirname);

	char fileName[256];
	sprintf_s(fileName, "resultData\\%04dy%02dm%02dd_%02dh%02dm%02d.dat",
		(local.tm_year + 1900), (local.tm_mon + 1), local.tm_mday,
		local.tm_hour, local.tm_min, local.tm_sec);

	FILE* fp;
	errno_t err = fopen_s(&fp, fileName, "wb");
	if (err == 0) {
		fwrite(&datus, sizeof(datus), 1, fp);
		fclose(fp);
	}
}

void SaveDatus::SaveImage(const struct tm& local)
{
	const char* dirname = "resultData";
	_mkdir(dirname);

	char dateStr[256];
	sprintf_s(dateStr, "resultData\\%04dy%02dm%02dd_%02dh%02dm%02d.jpg",
		(local.tm_year + 1900), (local.tm_mon + 1), local.tm_mday,
		local.tm_hour, local.tm_min, local.tm_sec);
	SaveDrawScreen(0, 0, CGame::GetAllGameRect().x, CGame::GetAllGameRect().y, dateStr, DX_IMAGESAVETYPE_JPEG);
}