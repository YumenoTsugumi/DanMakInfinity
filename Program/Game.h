#pragma once

#include "Func.h"
#include "DXFunc.h"
#include "Input.h"

class CGame {
public:
	CFPSManager* m_fpsManager;	//fps管理
	CInput* m_input;	//インプット
	CGame();
	~CGame();

	void Init();
	void Main();
};
