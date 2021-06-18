
#include "Input.h"
#include "DxLib.h"



bool CInputAllStatus::GetBtnOnOff(int getKeyIndex) {
	if (getKeyIndex >= 0 && getKeyIndex <= PAD_MAX) {
		if (m_btnStatus[getKeyIndex] == INPUT_ON ||
			m_btnStatus[getKeyIndex] == INPUT_PUSH ||
			m_btnStatus[getKeyIndex] == INPUT_DOUBLE) {
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}
void CInputAllStatus::Reset()
{
	for (int ii = 0; ii < UNION_MAX; ii++) {
		m_btnStatus[ii] = 0;
	}
}

//---------------------------------------------------------------------------------
//	CInput
//---------------------------------------------------------------------------------
void CInput::SetKeyboard(int index, int keyboard) {
	if (index < 0 || index >= UNION_MAX) {
		return;
	}
	m_keyboardTable[index] = keyboard;
}
void CInput::SetPad(int index, int Pad) {
	if (index < 0 || index >= UNION_MAX) {
		return;
	}
	//0x0001	->	0
	//0x0002	->	1
	//0x0004	->	2
	//0x0008	->	3
	m_padTable[index] = (int)CFunc::Logn(2, Pad);
}

void CInput::SetMouse(int index, int mouse) {
	if (index < 0 || index >= UNION_MAX) {
		return;
	}
	m_mouseTable[index] = (int)CFunc::Logn(2, mouse);
}

void CInput::GetPadStatus() {
	GetJoypadAnalogInput(&(m_inputStatus.m_padAnalogLeftX), &(m_inputStatus.m_padAnalogLeftY), DX_INPUT_PAD1);
	GetJoypadAnalogInputRight(&(m_inputStatus.m_padAnalogRightX), &(m_inputStatus.m_padAnalogRightY), DX_INPUT_PAD1);
	m_inputStatus.m_padPov = GetJoypadPOVState(DX_INPUT_PAD1, 0);

	int mul = 1;
	int padInput = GetJoypadInputState(DX_INPUT_PAD1);
	for (int i = 0; i < PAD_MAX; i++) {

		if (m_padDoubleFlg[i] == true) {
			m_padDoubleCount[i]++;
			if (m_padDoubleCount[i] > DOUBLECHKTIME) {
				m_padDoubleCount[i] = 0;
				m_padDoubleFlg[i] = false;
			}
		}

		m_padOldCount[i] = m_padCount[i];

		//------------------------------
		//m_padPovを考慮する	上0	右9000
		//------------------------------
		bool pov = false;	//入力があったか

		//下　左　右　上
		int povIdx[4] = { 5, 7, 3, 1 };	//参照先	
		int povAry[9] = { 31500, 0, 4500, 9000, 13500, 18000, 22500, 27000, 31500 };
		//0~4までが方向キーが入っている場所
		if (i >= 0 && i <= 4) {
			for (int h = 0; h < 4; h++) {
				if (i == h) {
					if (m_inputStatus.m_padPov == povAry[povIdx[h] - 1] ||
						m_inputStatus.m_padPov == povAry[povIdx[h]] ||
						m_inputStatus.m_padPov == povAry[povIdx[h] + 1]) {
						pov = true;
					}
				}
			}
		}

		if (padInput & mul || pov == true) {
			m_padCount[i]++;
			m_inputStatus.m_padStatus[i] = INPUT_ON;
			if (m_padOldCount[i] == 0) {
				m_padDoubleFlg[i] = true;

				m_inputStatus.m_padStatus[i] = INPUT_PUSH;
				if (m_padDoubleCount[i] > 0) {
					m_inputStatus.m_padStatus[i] = INPUT_DOUBLE;
					m_padDoubleCount[i] = 0;
					m_padDoubleFlg[i] = false;
				}
			}
		}
		else {
			m_padCount[i] = 0;
			m_inputStatus.m_padStatus[i] = INPUT_OFF;
			if (m_padOldCount[i] > 0) {
				m_inputStatus.m_padStatus[i] = INPUT_CAST;
			}
		}

		mul *= 2;
	}

}

void CInput::GetMouseStatus() {
	GetMousePoint(&(m_inputStatus.m_mouseX), &(m_inputStatus.m_mouseY));

	int mouseInput = GetMouseInput();

	int m_target[3] = { INPUT_MOUSE_LEFT, INPUT_MOUSE_RIGHT, INPUT_MOUSE_CENTER };

	for (int i = 0; i < 3; i++) {
		if (mouseDoubleFlg[i] == true) {
			m_mouseDoubleCount[i]++;
			if (m_mouseDoubleCount[i] > DOUBLECHKTIME) {
				m_mouseDoubleCount[i] = 0;
				mouseDoubleFlg[i] = false;
			}
		}
		m_mouseOldInputCount[i] = m_mouseInputCount[i];
		if ((mouseInput & m_target[i]) != 0) {
			m_mouseInputCount[i]++;
			m_inputStatus.m_mouseStatus[i] = INPUT_ON;

			if (m_mouseOldInputCount[i] == 0) {
				mouseDoubleFlg[i] = true;

				m_inputStatus.m_mouseStatus[i] = INPUT_PUSH;
				if (m_mouseDoubleCount[i] > 0) {
					m_inputStatus.m_mouseStatus[i] = INPUT_DOUBLE;
					m_mouseDoubleCount[i] = 0;
					mouseDoubleFlg[i] = false;
				}
			}
		}
		else {
			m_mouseInputCount[i] = 0;
			m_inputStatus.m_mouseStatus[i] = INPUT_OFF;
			if (m_mouseOldInputCount[i] > 0) {
				m_inputStatus.m_mouseStatus[i] = INPUT_CAST;
			}
		}
	}
}

void CInput::GetKeyBoardStatus() {
	char buf[256];
	GetHitKeyStateAll(buf);

	for (int i = 0; i < 256; i++) {
		if (m_keyDoubleFlg[i] == true) {
			m_keyDoubleCount[i]++;
			if (m_keyDoubleCount[i] > DOUBLECHKTIME) {
				m_keyDoubleCount[i] = 0;
				m_keyDoubleFlg[i] = false;
			}
		}

		m_keyOldCount[i] = m_keyCount[i];
		if (buf[i] == 1) {
			m_keyCount[i]++;
			m_inputStatus.m_keyStatus[i] = INPUT_ON;
			if (m_keyOldCount[i] == 0) {
				m_keyDoubleFlg[i] = true;

				m_inputStatus.m_keyStatus[i] = INPUT_PUSH;
				if (m_keyDoubleCount[i] > 0) {
					m_inputStatus.m_keyStatus[i] = INPUT_DOUBLE;
					m_keyDoubleCount[i] = 0;
					m_keyDoubleFlg[i] = false;
				}
			}
		}
		else {
			m_keyCount[i] = 0;
			m_inputStatus.m_keyStatus[i] = INPUT_OFF;
			if (m_keyOldCount[i] > 0) {
				m_inputStatus.m_keyStatus[i] = INPUT_CAST;
			}
		}
	}
}

void CInput::GetUnionStatus() {
	int keyBtnStatus[UNION_MAX] = { INPUT_NULL };
	int padBtnStatus[UNION_MAX] = { INPUT_NULL };
	int mouseBtnStatus[UNION_MAX] = { INPUT_NULL };

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < UNION_MAX; j++) {
			if (m_keyboardTable[j] == i) {
				keyBtnStatus[j] = m_inputStatus.m_keyStatus[i];
			}
		}
	}

	//int mul = 1;
	for (int i = 0; i < PAD_MAX; i++) {
		for (int j = 0; j < UNION_MAX; j++) {
			if (m_padTable[j] == i) {
				padBtnStatus[j] = m_inputStatus.m_padStatus[i];
			}
		}
		//mul *= 2;
	}

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < UNION_MAX; j++) {
			if (m_mouseTable[j] == i) {
				mouseBtnStatus[j] = m_inputStatus.m_mouseStatus[i];
			}
		}
	}

	//キーボード　と　パッドの結合の優先順位
	//	INPUT_DOUBLE(4) > INPUT_CAST(2) > INPUT_CAST(3) > INPUT_ON(0) > INPUT_OFF(1) > INPUT_NULL
	//	にしたかったけどめんどくさいので大きい順
	for (int j = 0; j < UNION_MAX; j++) {
		m_inputStatus.m_btnStatus[j] = CFunc::GetMax(CFunc::GetMax(keyBtnStatus[j],
			padBtnStatus[j]),
			mouseBtnStatus[j]);
	}

	//printfDx("keyBtnStatus:");
	//for(int j=0;j<UNION_MAX;j++){
	//	printfDx("%d", keyBtnStatus[j]);
	//}
	//printfDx("\n");
	//printfDx("padBtnStatus:");
	//for(int j=0;j<UNION_MAX;j++){
	//	printfDx("%d", padBtnStatus[j]);
	//}
	//printfDx("\n");
}
void CInput::GetState() {

	//マウスの入力ステータスを取得
	GetMouseStatus();

	//キーボードの入力ステータスを取得
	GetKeyBoardStatus();

	//キーボードとパッドの入力を取得
	GetPadStatus();

	//キーボードとパッドの結合
	GetUnionStatus();
}


CInput::CInput() {
	memset(m_keyCount, 0, sizeof(m_keyCount));
	memset(m_keyOldCount, 0, sizeof(m_keyOldCount));
	memset(m_keyDoubleFlg, 0, sizeof(m_keyDoubleFlg));
	memset(m_keyDoubleCount, 0, sizeof(m_keyDoubleCount));

	memset(m_mouseInputCount, 0, sizeof(m_mouseInputCount));
	memset(m_mouseOldInputCount, 0, sizeof(m_mouseOldInputCount));
	memset(mouseDoubleFlg, 0, sizeof(mouseDoubleFlg));
	memset(m_mouseDoubleCount, 0, sizeof(m_mouseDoubleCount));

	memset(m_padCount, 0, sizeof(m_padCount));
	memset(m_padOldCount, 0, sizeof(m_padOldCount));
	memset(m_padDoubleFlg, 0, sizeof(m_padDoubleFlg));
	memset(m_padDoubleCount, 0, sizeof(m_padDoubleCount));

	memset(m_keyboardTable, -1, sizeof(m_keyboardTable));
	memset(m_padTable, -1, sizeof(m_padTable));
	memset(m_mouseTable, -1, sizeof(m_mouseTable));
}
CInput::~CInput() {

}

CInputAllStatus* CInput::GetInputStatus() {
	return &m_inputStatus;
}



void CInput::DebugPrint() {
	printfDx("m_padStatus:");
	for (int i = 0; i < PAD_MAX; i++) {
		printfDx("%d", m_inputStatus.m_padStatus[i]);
	}
	printfDx("\n");

	printfDx("Mouse:(%d,%d)\n", m_inputStatus.m_mouseX, m_inputStatus.m_mouseY);

	printfDx("m_mouseStatus:(%d,%d,%d)\n", m_inputStatus.m_mouseStatus[0],
		m_inputStatus.m_mouseStatus[1],
		m_inputStatus.m_mouseStatus[2]);

	printfDx("PadAnalogLeft:(%d,%d)\n", m_inputStatus.m_padAnalogLeftX,
		m_inputStatus.m_padAnalogLeftY);
	printfDx("PadAnalogRgiht:(%d,%d)\n", m_inputStatus.m_padAnalogRightX,
		m_inputStatus.m_padAnalogRightY);

	printfDx("m_padPov:%d\n", m_inputStatus.m_padPov);


	printfDx("m_keyStatus:\n");
	for (int i = 0; i < 256; i++) {
		printfDx("%d", m_inputStatus.m_keyStatus[i]);
		if (i % 32 == 31) {
			printfDx("\n");
		}
	}
	printfDx("\n");

	printfDx("m_btnStatus:");
	for (int j = 0; j < UNION_MAX; j++) {
		printfDx("%d", m_inputStatus.m_btnStatus[j]);
	}
	printfDx("\n");

}