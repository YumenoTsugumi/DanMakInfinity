#include "Game.h"

#include "Input.h"
#include "BattleScene.h"
#include "BaseBeam.h"

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


	//弾幕画像(試作型)
	//ふぁいる、総数、X、Y、サイズX、サイズY、アニメ速度、回転速度
	m_resourceManager.Add(new CBulletImage("Resource\\bullet00.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet00", 00);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet01.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet01", 01);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet02.png", 1, 1, 1, 32, 32, 0, 0.0), "bullet02", 02);

	m_resourceManager.Add(new CBulletImage("Resource\\bullet10.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet10", 10);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet11.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet11", 11);
	m_resourceManager.Add(new CBulletImage("Resource\\bullet12.png", 2, 2, 1, 16, 64, 10, 0.0), "bullet12", 12);

	m_resourceManager.Add(new CImage("Resource\\pChan.png"), "pChan", 1000);

	//画像
	//レーザー用の画像読み込み
	{	
		const char fileA[][RESOURCENAMEMAX] = { "Resource/bulletstand_b.png","Resource/childLaser_b.png" };
		m_resourceManager.Add(new CImageSet(2, fileA), "laser01", 100);	
	}
	{	
		const char fileA[][RESOURCENAMEMAX] = { "Resource/bulletstand_r.png","Resource/childLaser_r.png" };
		m_resourceManager.Add(new CImageSet(2, fileA), "laser02", 101);	
	}

	m_resourceManager.Add(new CBulletImage("Resource\\h00.png", 1, 1, 1, 64, 576, 0, 0.0), "hLaser01", 00);
	m_resourceManager.Add(new CBulletImage("Resource\\h01.png", 1, 1, 1, 64, 576, 0, 0.0), "hLaser02", 01);
	m_resourceManager.Add(new CBulletImage("Resource\\h02.png", 1, 1, 1, 64, 576, 0, 0.0), "hLaser03", 02);
	//m_resourceManager.Add(new CBulletImage("Resource\\h00.png", 1, 1, 1, 16, 64, 10, 0.0), "hLaser01", 500);
	//m_resourceManager.Add(new CBulletImage("Resource\\h01.png", 1, 1, 1, 16, 64, 10, 0.0), "hLaser02", 501);
	//m_resourceManager.Add(new CBulletImage("Resource\\h02.png", 1, 1, 1, 16, 64, 10, 0.0), "hLaser03", 502);
	//m_resourceManager.Add(new CImage("Resource\\h00.png"), "hLaser00", 500);
	//m_resourceManager.Add(new CImage("Resource\\h01.png"), "hLaser01", 501);
	//m_resourceManager.Add(new CImage("Resource\\h02.png"), "hLaser02", 502);


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

