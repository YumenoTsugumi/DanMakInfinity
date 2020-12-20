#include "Game.h"

#include "Input.h"
#include "BattleScene.h"

CGame::CGame() :
	m_fpsManager(nullptr),
	m_input(nullptr)
{

}

CGame::~CGame() {

}

void CGame::Init()
{
	m_fpsManager = new CFPSManager();

	// インプット管理クラスに、パッドによる操作と、キーボードによる操作を登録する
	m_input = new CInput();
	m_input->SetPad(INPUT_DEF_DOWN, INPUT_PAD_DOWN);
	m_input->SetPad(INPUT_DEF_LEFT, INPUT_PAD_LEFT);
	m_input->SetPad(INPUT_DEF_UP, INPUT_PAD_UP);
	m_input->SetPad(INPUT_DEF_RIGHT, INPUT_PAD_RIGHT);
	m_input->SetPad(INPUT_DEF_ENTER, INPUT_PAD_BTN1);
	m_input->SetPad(INPUT_DEF_CANCEL, INPUT_PAD_BTN2);

	m_input->SetKeyboard(INPUT_DEF_DOWN, INPUT_KEY_S);
	m_input->SetKeyboard(INPUT_DEF_LEFT, INPUT_KEY_A);
	m_input->SetKeyboard(INPUT_DEF_UP, INPUT_KEY_W);
	m_input->SetKeyboard(INPUT_DEF_RIGHT, INPUT_KEY_D);
	m_input->SetKeyboard(INPUT_DEF_ENTER, INPUT_KEY_Q);
	m_input->SetKeyboard(INPUT_DEF_CANCEL, INPUT_KEY_E);

}

void CGame::CreateBattleScene()
{
	m_sceneManager = new CSceneManager();
	CBattleScene* battleScene = new CBattleScene(0);
	battleScene->Init(this);	//シーンにゲームクラスポインタを渡す
	m_sceneManager->AddScene(battleScene);
}

void CGame::Main() {

	// メインループ処理
	while (ProcessMessage() == 0) {
		m_fpsManager->Start(); //fpsStart管理
		ClearDrawScreen();//描画範囲削除

		//入力
		m_input->GetState();
		CInputAllStatus* inputData = m_input->GetInputStatus();

		// 入力のデバッグ情報表示
		m_input->DebugPrint();

		// シーンの実行
		m_sceneManager->Main(inputData);

		ScreenFlip(); //描画範囲反映
		
		clsDx(); //printfDx削除
		m_fpsManager->End(); //fpsEnd管理

	}
}

