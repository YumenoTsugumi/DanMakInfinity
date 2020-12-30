#pragma once

#include "Func.h"
#include "DXFunc.h"
#include "Input.h"
#include "Resource.h"
#include "Scene.h"

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
	// ゲームのエリアサイズ（弾が消える範囲）
	static CRect m_battleRect;
	static CRect GetBattleRect() {
		return m_battleRect;
	}
	static CRect SetBattleRect(const CRect rect) {
		return m_battleRect = rect;
	}

	//--------------------------------------------------------------
	// めんどくさいので共通化
	//--------------------------------------------------------------
	CGame();
	~CGame();

	void Init();
	void CreateBattleScene();
	void Main();

	void ImageLoad();
	static void ImageLoadByThread();
};
