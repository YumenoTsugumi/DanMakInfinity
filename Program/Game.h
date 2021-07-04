#pragma once

#include "Func.h"
#include "DXFunc.h"
#include "Input.h"
#include "Resource.h"
#include "Scene.h"
#include "GameDefine.h"


class SaveDatus;
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

	static std::vector<SaveDatus> m_saveData;
	static void Load_SaveData();
	static void Save_SaveData();


	// メニュー関連
public:
	bool m_exitFlag;
	void SetExit() { m_exitFlag = true; }
};

const int GameVer1 = 100;
class SaveDatus {
public:
	static void Load(const char* filename, SaveDatus& datus);
	static void Save(int startRank, int endRank,
		int stage, int item, int liveTime,
		int rapidShot, int rapidspeed,
		int slowShot, int slowspeed,
		long long score,
		const struct tm& local,
		char* name,
		int gameMode
	);
	static void SaveImage(const struct tm& local);
	int m_ver;
	int m_startRank;
	int m_endRank;
	int m_stage;
	int m_item;
	int m_liveTime;
	int m_rapidShot;
	int m_rapidspeed;
	int m_slowShot;
	int m_slowspeed;

	long long m_score; // 8byte

	int checkA; // scoreより上のチェック
	int checkB; // scoreのチェック

	int m_year;
	int m_mon;
	int m_day;
	int m_hour;
	int m_min;
	int m_sec;

	char m_name[12];

	int m_gameMode; // ノーマルかEXか
	int buf1;
	int buf2;
	int buf3;
	int buf4;
	int buf5;
	int buf6;
	int buf7;
	int buf8;
	int buf9;
};