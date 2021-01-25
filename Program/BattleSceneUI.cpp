#include "BattleSceneUI.h"
#include "Game.h"
#include "GameDefine.h"

CBattleSceneUI::CBattleSceneUI() :
	m_score(0),
	m_hiScore(0)
{

}

CBattleSceneUI::~CBattleSceneUI(){

}

void CBattleSceneUI::Init() {
	// ����
	m_textScore = (CImage*)CGame::GetResource(1050);
	m_textHiScore = (CImage*)CGame::GetResource(1051);
	m_text0g = (CImage*)CGame::GetResource(1010);
	m_textComma = (CImage*)CGame::GetResource(1011);
	m_textCommag = (CImage*)CGame::GetResource(1012);
	m_numberMaxSize = CPos(0, 0);
	for (int ii = 0; ii < 10; ii++) {
		CImage* img = (CImage*)CGame::GetResource(1000 + ii);
		m_textNumber.push_back(img);

		if (img->m_sizeX > m_numberMaxSize.x) {
			m_numberMaxSize.x = img->m_sizeX;
		}
		if (img->m_sizeY > m_numberMaxSize.y) {
			m_numberMaxSize.y = img->m_sizeY;
		}
	}

	// �O��
	m_UIFoundation = (CImage*)CGame::GetResource(1500);

	m_score = 123456;
	m_hiScore = 123456789012;
}
void CBattleSceneUI::Draw() {
	DrawGameAreaUI();
	DrawOutArea();
}

// ��ʓ���UI�`��
void CBattleSceneUI::DrawGameAreaUI() {
	m_score += rand()*20;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);

	//GameWindowAreaLeft
	//GameWindowAreaTop


	const int maxDrawDigit = 12; // ���l�������Ă�8���܂ł͕\������
	//--------------------------------------------------------------------------
	// Score�̕\���ʒu
	//--------------------------------------------------------------------------
	double textScore_x = GameWindowAreaLeft + 3;
	double textScore_y = GameWindowAreaTop + 3;
	CDxFunc::MyDrawGraph(textScore_x, textScore_y, m_textScore->m_iamge);
	double textScore_endX = textScore_x + m_textScore->m_sizeX;

	// �X�R�A�̒l�̕\��
	int digit = CFunc::GetDigit(m_score); // 12345 �Ȃ� 5
	
	// 12�Ȃ� 3�̔{���ɂ��悤�I
	// 210987654321
	// 210,987,654,321

	// 00123456 �Ȃ�0����\�����Ă���
	double commaGap = 0;
	for (int ii = maxDrawDigit; ii > 0 ; ii--) {
		CImage* drawImage;
		if (digit >= ii) {
			// ��������ꍇ�@���l��\��
			// 4444 ��3���ڂȂ� 4444 / 10^2 % 10
			long long number = (m_score / (long long)(pow(10, (ii-1)))) % 10;
			drawImage = m_textNumber[number];
		}
		else {
			// ��������Ȃ��ꍇ�@��������0
			drawImage = m_text0g;
		}

		// �X�R�A�e�L�X�g�̏I�_����@�����̕�*1~8
		int drawWigth = m_numberMaxSize.x * 0.65;
		double textNumber_x = textScore_endX + 10 + drawWigth * (maxDrawDigit - ii);
		double gap = (drawWigth - drawImage->m_sizeX) / 2;

		// 3�̔{���̎���,��\������
		if (ii % 3 == 0 && maxDrawDigit != ii) {
			if (digit >= ii+1) {
				CDxFunc::MyDrawGraph(textNumber_x + gap + commaGap, textScore_y, m_textComma->m_iamge);
			}
			else {
				CDxFunc::MyDrawGraph(textNumber_x + gap + commaGap, textScore_y, m_textCommag->m_iamge);
			}
			commaGap += m_textComma->m_sizeX * 0.5;
		}
		CDxFunc::MyDrawGraph(textNumber_x + gap + commaGap, textScore_y, drawImage->m_iamge);
	}
	
	//--------------------------------------------------------------------------
	// HiScore�̕\���ʒu
	//--------------------------------------------------------------------------
	double textHiScore_x = GameWindowCenterX + 32 + 3;
	CDxFunc::MyDrawGraph(textHiScore_x, textScore_y, m_textHiScore->m_iamge);
	double textHiScore_endX = textHiScore_x + m_textHiScore->m_sizeX;

	// �X�R�A�̒l�̕\��
	int hiDigit = CFunc::GetDigit(m_hiScore); // 12345 �Ȃ� 5

	double hiCommaGap = 0;
	for (int ii = maxDrawDigit; ii > 0; ii--) {
		CImage* drawImage;
		if (hiDigit >= ii) {
			// ��������ꍇ�@���l��\��
			// 4444 ��3���ڂȂ� 4444 / 10^2 % 10
			long long number = (m_hiScore / (long long)(pow(10, (ii - 1)))) % 10;
			drawImage = m_textNumber[number];
		}
		else {
			// ��������Ȃ��ꍇ�@��������0
			drawImage = m_text0g;
		}

		// �X�R�A�e�L�X�g�̏I�_����@�����̕�*1~8
		int drawWigth = m_numberMaxSize.x * 0.65;
		double textNumber_x = textHiScore_endX + 10 + drawWigth * (maxDrawDigit - ii);
		double gap = (drawWigth - drawImage->m_sizeX) / 2;

		// 3�̔{���̎���,��\������
		if (ii % 3 == 0 && maxDrawDigit != ii) {
			if (hiDigit >= ii) {
				CDxFunc::MyDrawGraph(textNumber_x + gap + hiCommaGap, textScore_y, m_textComma->m_iamge);
			}
			else {
				CDxFunc::MyDrawGraph(textNumber_x + gap + hiCommaGap, textScore_y, m_textCommag->m_iamge);
			}
			hiCommaGap += m_textComma->m_sizeX * 0.5;
		}
		CDxFunc::MyDrawGraph(textNumber_x + gap + hiCommaGap, textScore_y, drawImage->m_iamge);
	}
}

// ��ʊO�̕`��
void CBattleSceneUI::DrawOutArea()
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	CDxFunc::MyDrawGraph(0, 0, m_UIFoundation->m_iamge);
	
}




