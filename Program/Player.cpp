
#include "Player.h"

CPlayer::CPlayer():
	m_pos(0,0),
	m_playerImage(nullptr),
	m_slowMove(0)
{
}

CPlayer::~CPlayer() 
{
}

void CPlayer::Init()
{
	m_pos.x = 400;
	m_pos.y = 500;

	m_playerImage = (CImage*)CGame::GetResource(1000);
}

void CPlayer::Action(CInputAllStatus* input)
{
	//移動処理
	double move = 6.0;
	int moveCo = 0;

	if (input->GetBtnOnOff(INPUT_DEF_UP) == true) {
		moveCo++;
	}
	if (input->GetBtnOnOff(INPUT_DEF_DOWN) == true) {
		moveCo++;
	}
	if (input->GetBtnOnOff(INPUT_DEF_RIGHT) == true) {
		moveCo++;
	}
	if (input->GetBtnOnOff(INPUT_DEF_LEFT) == true) {
		moveCo++;
	}
	if (moveCo >= 2) {
		move *= 1.0 / sqrt(2.0);
		m_slowMove = MOVE_SLOW;
	}
	else {
		m_slowMove = MOVE_HI;
	}

	if (input->GetBtnOnOff(INPUT_DEF_UP) == true) {
		m_pos.y -= move;
	}
	if (input->GetBtnOnOff(INPUT_DEF_DOWN) == true) {
		m_pos.y += move;
	}
	if (input->GetBtnOnOff(INPUT_DEF_RIGHT) == true) {
		m_pos.x += move;
	}
	if (input->GetBtnOnOff(INPUT_DEF_LEFT) == true) {
		m_pos.x -= move;
	}

}
void CPlayer::Draw()
{
	CDxFunc::DrawRotaGraph(m_pos.x, m_pos.y, 0.75, 0.0, m_playerImage->m_iamge);
}
