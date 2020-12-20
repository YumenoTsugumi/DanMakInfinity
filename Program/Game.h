#pragma once

#include "Func.h"
#include "DXFunc.h"
#include "Input.h"
#include "Resource.h"
#include "Scene.h"

class CGame {
public:
	CFPSManager* m_fpsManager;	//fps管理
	CInput* m_input;	//インプット
	CResourceManager* m_resourceManager;	//リソース管理
	CSceneManager* m_sceneManager;	//シーン管理

	CGame();
	~CGame();

	void Init();
	void CreateBattleScene();
	void Main();
};
