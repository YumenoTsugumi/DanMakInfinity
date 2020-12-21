
#pragma once

#include "Game.h"
#include "Resource.h"

const int MOVE_HI = 0;
const int MOVE_SLOW = 1;

class CPlayer {
public:
	CPos m_pos;

	CImage* m_playerImage;
	int m_slowMove;	//0(MOVE_HI)通常　1(MOVE_SLOW)低速

	CPlayer();
	~CPlayer();
	void Init();
	void Action(CInputAllStatus* input);
	void Draw();
};