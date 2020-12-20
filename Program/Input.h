#pragma once

#include "DxLib.h"

#include "DXFunc.h"
#include "Func.h"

#include <stdio.h>

const int INPUT_DEF_DOWN = 0;
const int INPUT_DEF_LEFT = 1;
const int INPUT_DEF_UP = 2;
const int INPUT_DEF_RIGHT = 3;
const int INPUT_DEF_ENTER = 4;
const int INPUT_DEF_CANCEL = 5;

// 入力状態のステータス
const int	INPUT_NULL = (-1);	//無効
const int	INPUT_OFF = (0);	//押されていない
const int	INPUT_ON = (1);	//押されている
const int	INPUT_PUSH = (2);	//押された瞬間　（次離して押す
const int	INPUT_CAST = (3);	//離された
const int	INPUT_DOUBLE = (4);	//ダブルクリック	マウス専用


//-------------------------------------------------------
//	SetKeyboard,SetPadで使用できる値
//-------------------------------------------------------
//const int	INPUT_PAD_DOWN	= (0);
//const int	INPUT_PAD_LEFT	= (1);
//const int	INPUT_PAD_UP	= (2);
//const int	INPUT_PAD_RIGHT	= (3);
//const int	INPUT_PAD_BTN1	= (4);       //KEY_INPUT_LSHIFT;
//const int	INPUT_PAD_BTN2	= (5);		 //KEY_INPUT_X;
//const int	INPUT_PAD_BTN3	= (6);		 //KEY_INPUT_Z;
//const int	INPUT_PAD_BTN4	= (7);		 //KEY_INPUT_A;
//const int	INPUT_PAD_BTN5	= (8);		 //KEY_INPUT_S;
//const int	INPUT_PAD_BTN6	= (9);		 //KEY_INPUT_D;
//const int	INPUT_PAD_BTN7	= (10);		 //KEY_INPUT_C;
//const int	INPUT_PAD_BTN8	= (11);		 //KEY_INPUT_ESCAPE;
const int	INPUT_PAD_DOWN = (PAD_INPUT_DOWN);
const int	INPUT_PAD_LEFT = (PAD_INPUT_LEFT);
const int	INPUT_PAD_UP = (PAD_INPUT_RIGHT);
const int	INPUT_PAD_RIGHT = (PAD_INPUT_UP);
const int	INPUT_PAD_BTN1 = (PAD_INPUT_1);       //KEY_INPUT_LSHIFT;
const int	INPUT_PAD_BTN2 = (PAD_INPUT_2);		 //KEY_INPUT_X;
const int	INPUT_PAD_BTN3 = (PAD_INPUT_3);		 //KEY_INPUT_Z;
const int	INPUT_PAD_BTN4 = (PAD_INPUT_4);		 //KEY_INPUT_A;
const int	INPUT_PAD_BTN5 = (PAD_INPUT_5);		 //KEY_INPUT_S;
const int	INPUT_PAD_BTN6 = (PAD_INPUT_6);		 //KEY_INPUT_D;
const int	INPUT_PAD_BTN7 = (PAD_INPUT_7);		 //KEY_INPUT_C;
const int	INPUT_PAD_BTN8 = (PAD_INPUT_8);		 //KEY_INPUT_ESCAPE;

// キー定義
const int	INPUT_KEY_BACK = KEY_INPUT_BACK;
const int	INPUT_KEY_TAB = KEY_INPUT_TAB;
const int	INPUT_KEY_RETURN = KEY_INPUT_RETURN;

const int	INPUT_KEY_LSHIFT = KEY_INPUT_LSHIFT;
const int	INPUT_KEY_RSHIFT = KEY_INPUT_RSHIFT;
const int	INPUT_KEY_LCONTROL = KEY_INPUT_LCONTROL;
const int	INPUT_KEY_RCONTROL = KEY_INPUT_RCONTROL;
const int	INPUT_KEY_ESCAPE = KEY_INPUT_ESCAPE;
const int	INPUT_KEY_SPACE = KEY_INPUT_SPACE;
const int	INPUT_KEY_PGUP = KEY_INPUT_PGUP;
const int	INPUT_KEY_PGDN = KEY_INPUT_PGDN;
const int	INPUT_KEY_END = KEY_INPUT_END;
const int	INPUT_KEY_HOME = KEY_INPUT_HOME;
const int	INPUT_KEY_LEFT = KEY_INPUT_LEFT;		//--------------------よく使いそう
const int	INPUT_KEY_UP = KEY_INPUT_UP;		//--------------------よく使いそう
const int	INPUT_KEY_RIGHT = KEY_INPUT_RIGHT;	//--------------------よく使いそう
const int	INPUT_KEY_DOWN = KEY_INPUT_DOWN;		//--------------------よく使いそう
const int	INPUT_KEY_INSERT = KEY_INPUT_INSERT;
const int	INPUT_KEY_DELETE = KEY_INPUT_DELETE;

const int	INPUT_KEY_MINUS = KEY_INPUT_MINUS;
const int	INPUT_KEY_YEN = KEY_INPUT_YEN;
const int	INPUT_KEY_PREVTRACK = KEY_INPUT_PREVTRACK;
const int	INPUT_KEY_PERIOD = KEY_INPUT_PERIOD;
const int	INPUT_KEY_SLASH = KEY_INPUT_SLASH;
const int	INPUT_KEY_LALT = KEY_INPUT_LALT;
const int	INPUT_KEY_RALT = KEY_INPUT_RALT;
const int	INPUT_KEY_SCROLL = KEY_INPUT_SCROLL;
const int	INPUT_KEY_SEMICOLON = KEY_INPUT_SEMICOLON;
const int	INPUT_KEY_COLON = KEY_INPUT_COLON;
const int	INPUT_KEY_LBRACKET = KEY_INPUT_LBRACKET;
const int	INPUT_KEY_RBRACKET = KEY_INPUT_RBRACKET;
const int	INPUT_KEY_AT = KEY_INPUT_AT;
const int	INPUT_KEY_BACKSLASH = KEY_INPUT_BACKSLASH;
const int	INPUT_KEY_COMMA = KEY_INPUT_COMMA;
const int	INPUT_KEY_KANJI = KEY_INPUT_KANJI;
const int	INPUT_KEY_CONVERT = KEY_INPUT_CONVERT;
const int	INPUT_KEY_NOCONVERT = KEY_INPUT_NOCONVERT;
const int	INPUT_KEY_KANA = KEY_INPUT_KANA;
const int	INPUT_KEY_APPS = KEY_INPUT_APPS;
const int	INPUT_KEY_CAPSLOCK = KEY_INPUT_CAPSLOCK;
const int	INPUT_KEY_SYSRQ = KEY_INPUT_SYSRQ;
const int	INPUT_KEY_PAUSE = KEY_INPUT_PAUSE;
const int	INPUT_KEY_LWIN = KEY_INPUT_LWIN;
const int	INPUT_KEY_RWIN = KEY_INPUT_RWIN;

const int	INPUT_KEY_NUMLOCK = KEY_INPUT_NUMLOCK;
const int	INPUT_KEY_NUMPAD0 = KEY_INPUT_NUMPAD0;
const int	INPUT_KEY_NUMPAD1 = KEY_INPUT_NUMPAD1;
const int	INPUT_KEY_NUMPAD2 = KEY_INPUT_NUMPAD2;
const int	INPUT_KEY_NUMPAD3 = KEY_INPUT_NUMPAD3;
const int	INPUT_KEY_NUMPAD4 = KEY_INPUT_NUMPAD4;
const int	INPUT_KEY_NUMPAD5 = KEY_INPUT_NUMPAD5;
const int	INPUT_KEY_NUMPAD6 = KEY_INPUT_NUMPAD6;
const int	INPUT_KEY_NUMPAD7 = KEY_INPUT_NUMPAD7;
const int	INPUT_KEY_NUMPAD8 = KEY_INPUT_NUMPAD8;
const int	INPUT_KEY_NUMPAD9 = KEY_INPUT_NUMPAD9;
const int	INPUT_KEY_MULTIPLY = KEY_INPUT_MULTIPLY;
const int	INPUT_KEY_ADD = KEY_INPUT_ADD;
const int	INPUT_KEY_SUBTRACT = KEY_INPUT_SUBTRACT;
const int	INPUT_KEY_DECIMAL = KEY_INPUT_DECIMAL;
const int	INPUT_KEY_DIVIDE = KEY_INPUT_DIVIDE;
const int	INPUT_KEY_NUMPADENTER = KEY_INPUT_NUMPADENTER;

const int	INPUT_KEY_F1 = KEY_INPUT_F1;
const int	INPUT_KEY_F2 = KEY_INPUT_F2;
const int	INPUT_KEY_F3 = KEY_INPUT_F3;
const int	INPUT_KEY_F4 = KEY_INPUT_F4;
const int	INPUT_KEY_F5 = KEY_INPUT_F5;
const int	INPUT_KEY_F6 = KEY_INPUT_F6;
const int	INPUT_KEY_F7 = KEY_INPUT_F7;
const int	INPUT_KEY_F8 = KEY_INPUT_F8;
const int	INPUT_KEY_F9 = KEY_INPUT_F9;
const int	INPUT_KEY_F10 = KEY_INPUT_F10;
const int	INPUT_KEY_F11 = KEY_INPUT_F11;
const int	INPUT_KEY_F12 = KEY_INPUT_F12;

const int	INPUT_KEY_A = KEY_INPUT_A;
const int	INPUT_KEY_B = KEY_INPUT_B;
const int	INPUT_KEY_C = KEY_INPUT_C;
const int	INPUT_KEY_D = KEY_INPUT_D;
const int	INPUT_KEY_E = KEY_INPUT_E;
const int	INPUT_KEY_F = KEY_INPUT_F;
const int	INPUT_KEY_G = KEY_INPUT_G;
const int	INPUT_KEY_H = KEY_INPUT_H;
const int	INPUT_KEY_I = KEY_INPUT_I;
const int	INPUT_KEY_J = KEY_INPUT_J;
const int	INPUT_KEY_K = KEY_INPUT_K;
const int	INPUT_KEY_L = KEY_INPUT_L;
const int	INPUT_KEY_M = KEY_INPUT_M;
const int	INPUT_KEY_N = KEY_INPUT_N;
const int	INPUT_KEY_O = KEY_INPUT_O;
const int	INPUT_KEY_P = KEY_INPUT_P;
const int	INPUT_KEY_Q = KEY_INPUT_Q;
const int	INPUT_KEY_R = KEY_INPUT_R;
const int	INPUT_KEY_S = KEY_INPUT_S;
const int	INPUT_KEY_T = KEY_INPUT_T;
const int	INPUT_KEY_U = KEY_INPUT_U;
const int	INPUT_KEY_V = KEY_INPUT_V;
const int	INPUT_KEY_W = KEY_INPUT_W;
const int	INPUT_KEY_X = KEY_INPUT_X;
const int	INPUT_KEY_Y = KEY_INPUT_Y;
const int	INPUT_KEY_Z = KEY_INPUT_Z;

const int	INPUT_KEY_0 = KEY_INPUT_0;
const int	INPUT_KEY_1 = KEY_INPUT_1;
const int	INPUT_KEY_2 = KEY_INPUT_2;
const int	INPUT_KEY_3 = KEY_INPUT_3;
const int	INPUT_KEY_4 = KEY_INPUT_4;
const int	INPUT_KEY_5 = KEY_INPUT_5;
const int	INPUT_KEY_6 = KEY_INPUT_6;
const int	INPUT_KEY_7 = KEY_INPUT_7;
const int	INPUT_KEY_8 = KEY_INPUT_8;
const int	INPUT_KEY_9 = KEY_INPUT_9;

const int	INPUT_MOUSE_LEFT = MOUSE_INPUT_LEFT;
const int	INPUT_MOUSE_RIGHT = MOUSE_INPUT_RIGHT;
const int	INPUT_MOUSE_CENTER = MOUSE_INPUT_MIDDLE;




//パッドのマックス数
const int	PAD_MAX = 16;
//ダブルクリック受付時間
const int	DOUBLECHKTIME = 20;

//キー結合マックス数
const int	UNION_MAX = 10;


class CInputAllStatus {
public:
	//---------------------------------------
	// ゲームパッド
	//---------------------------------------
	//入力のステータスがこの中に入る
	//INPUT_STATUS_UPが
	//INPUT_ON(0)	INPUT_OFF(1)	INPUT_PUSH(2)	INPUT_CAST(3)	INPUT_DOUBLE(4)
	int m_padStatus[PAD_MAX];

	//軸角度
	int m_padAnalogLeftX, m_padAnalogLeftY;
	int m_padAnalogRightX, m_padAnalogRightY;
	//十字入力
	int m_padPov;	//上0	右9000	下18000	左27000

	//---------------------------------------
	// キーボード
	//---------------------------------------
	//キーボード256個のステータスがこの中に入る
	char m_keyStatus[256];
	//INPUT_ON(0)	INPUT_OFF(1)	INPUT_PUSH(2)	INPUT_CAST(3)	INPUT_DOUBLE(4)

	//---------------------------------------
	// マウス
	//---------------------------------------
	//マウスの座標
	int m_mouseX;
	int m_mouseY;
	//入力のステータスがこの中に入る
	//MOUSE_INPUT_LEFTが
	//INPUT_ON(0)	INPUT_OFF(1)	INPUT_PUSH(2)	INPUT_CAST(3)	INPUT_DOUBLE(4)
	char m_mouseStatus[3];


	//入力中のステータス(キーボードとパッドの結合)
	int m_btnStatus[UNION_MAX];
	//0～UNION_MAXは使用側が自由に決める
	//使用側が0123を上下左右と設定したい場合setKeyで設定する
	//INPUT_ON(0)	INPUT_OFF(1)	INPUT_PUSH(2)	INPUT_CAST(3)	INPUT_DOUBLE(4)
	//int BtnCount[UNION_MAX];

	// 指定したボタンの入力が押下されているか返す
	bool GetBtnOnOff(int index);

};


class CInput {
private:
	//キーボードの入力をカウント
	int m_keyCount[256];
	int m_keyOldCount[256];
	bool m_keyDoubleFlg[256];
	int m_keyDoubleCount[256];

	//マウスの入力をカウント
	int m_mouseInputCount[3];
	int m_mouseOldInputCount[3];
	bool mouseDoubleFlg[3];
	int m_mouseDoubleCount[3];

	//PADの入力をカウント
	int m_padCount[PAD_MAX];
	int m_padOldCount[PAD_MAX];
	bool m_padDoubleFlg[PAD_MAX];
	int m_padDoubleCount[PAD_MAX];

	//union系	対応するキーを記憶する
	int m_keyboardTable[UNION_MAX];
	int m_padTable[UNION_MAX];
	int m_mouseTable[UNION_MAX];


	void GetPadStatus();
	void GetMouseStatus();
	void GetKeyBoardStatus();
	void GetUnionStatus();

	//入力中のステータスをこれで管理
	CInputAllStatus m_inputStatus;

public:
	void SetKeyboard(int Idx, int Keyboard);
	void SetPad(int Idx, int Pad);
	void SetMouse(int Idx, int Mouse);

	CInputAllStatus* GetInputStatus();

	void GetState();
	//void init();
	CInput();
	~CInput();

	void DebugPrint();
};


/*

使い方


○ループ前


○メインループの中の最初
｜GetState
｜
｜
｜CInputData *input;
｜input = inputMain.GetInputStatus();

	if(	input->m_btnStatus[INPUT_STATUS_UP] == INPUT_ON){

	}
	if(input->m_btnStatus[INPUT_STATUS_DOWN] == INPUT_ON){

	}
	if(input->m_btnStatus[INPUT_STATUS_RIGHT] == INPUT_ON){

	}
	if(input->m_btnStatus[INPUT_STATUS_LEFT] == INPUT_ON){

	}
／





*/


