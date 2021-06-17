#pragma once

#include "Func.h"
#include "DXFunc.h"
#include "Input.h"
#include "Resource.h"
#include "Scene.h"
#include "GameDefine.h"

class CGame {
public:
	CFPSManager m_fpsManager;	//fps管理
	CInput m_input;	//インプット
	CSceneManager m_sceneManager;	//シーン管理

	//--------------------------------------------------------------
	// 以下はどこでもアクセスするはずなのでstaticで共通化
	//--------------------------------------------------------------
	//リソースマネージャ
	static CResourceManager m_resourceManager;	
	static CResourceManager* GetResourceManager() {
		return &m_resourceManager;
	}
	static void* GetResource(int index) {
		return m_resourceManager.GetResource(index);
	}
	static void* GetResource(const char* name) {
		return m_resourceManager.GetResource(name);
	}
	// ゲームのエリアサイズ（弾が消える範囲） あとで消したい
	//static CRect m_battleRect;
	//static CRect GetGameRect() {
	//	return m_battleRect;
	//}
	//static CRect SetBattleRect(const CRect rect) {
	//	return m_battleRect = rect;
	//}

	// FullHD(1920*1080)なら
	static CPos m_allGameRect; // 全体　(1920,1080)
	static CPos m_gameRectLT; // ゲーム内 (480,20)
	static CPos m_gameRectRB; // ゲーム内 (1440,1060)
	static double m_windowRatio;
	static void SetAllGameRect(double ratio) {
		m_windowRatio = ratio;
		m_allGameRect = CPos(WindowX * ratio, WindowY * ratio); //1920 1080
		m_gameRectLT = CPos(GameWindowAreaLeft* ratio,  // 480
							GameWindowAreaTop * ratio); // 20
		m_gameRectRB = CPos(GameWindowAreaRight * ratio,  // 1440
							GameWindowAreaBottom * ratio); // 1060
	}
	static double GetWindowRatio() { return m_windowRatio; }; // 画面サイズの割合
	static CPos GetAllGameRect(){ return m_allGameRect; } // 全体　(1920,1080)
	static CPos GetGamePosLT(){ return m_gameRectLT; } // ゲーム内 (480,20)
	static CPos GetGamePosRB(){ return m_gameRectRB; } // ゲーム内 (1440,1060)
	//static CRect GetGameRect() { return CRect(m_gameRectLT, m_gameRectRB); }
	static CRect GetGameRect() { return CRect(CPos(GameWindowAreaLeft, GameWindowAreaTop), CPos(GameWindowAreaRight, GameWindowAreaBottom)); }

	static CPos ToGamePos(CPos ratioPos);
	static CPos ToGamePos(double ratioPosX, double ratioPosY);
	static double ToGamePosX(double ratioPosX);
	static double ToGamePosY(double ratioPosY);
	static double ToGameSizeX(double ratioPosX);
	static double ToGameSizeY(double ratioPosY);

	static double ToAllSizeX(double ratioPosX);
	static double ToAllSizeY(double ratioPosY);

	//--------------------------------------------------------------
	// めんどくさいので共通化
	//--------------------------------------------------------------
	CGame();
	~CGame();

	void Init();
	void CreateBattleScene();
	void CreateTitleScene();
	void Main();

	void ImageLoad();
	static void ImageLoadByThread();

	// メニュー関連
public:
	bool m_exitFlag;
	void SetExit() { m_exitFlag = true; }
};
