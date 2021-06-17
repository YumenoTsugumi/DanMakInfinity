#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"

#include <functional>

constexpr int SelectItemNum = 5;
constexpr int SelectRankItemNum = 6;

class CTitleScene : public CScene{
public:
	//------------------
	//	���܂��Ȃ�
	//------------------
	CTitleScene(int InTime);
	virtual ~CTitleScene();

	//�s��	����isTopScene�͈�ԏ�̕`�悩�ǂ����ł���B�ォ��n�����̂ŋC�ɂ��Ȃ��ėǂ�
	virtual void Main(CInputAllStatus *input);

	//------------------
	//	�������牺�͎��R��
	//------------------
	void Init(CGame* gameP);



	// �֐��|�C���^
	std::function<void(CTitleScene*)> m_selectItemFunc[SelectItemNum];

	static void Play(CTitleScene* thisScene);
	static void Extra(CTitleScene* thisScene);
	static void Data(CTitleScene* thisScene);
	static void Config(CTitleScene* thisScene);
	static void Exit(CTitleScene* thisScene);

	CGame *m_game; // ��ʂ̃Q�[���N���X


	//------------------------------------------------------
	void Draw();
	void Action(CInputAllStatus* input);
	// ���S�ް�
	CPos m_titleLog1Pos;
	double m_titleLog1Size;
	double m_titleLog1Alpha;

	CImage* m_titleLog1;

	CPos m_titleLog2Pos;
	double m_titleLog2Size;
	double m_titleLog2Alpha;
	double m_titleLog2Angle;
	CPos m_titleLog3Pos;
	double m_titleLog3Size;
	double m_titleLog3Alpha;
	CImage* m_titleLog2;
	CImage* m_titleLog3;

	// �w�i
	CImage* m_background;
	CImage* m_planet1;
	CImage* m_planet2;
	CImage* m_planet3;

	// �I����
	int m_selectIndex;
	CImage* m_selectItem[SelectItemNum][2]; // ��A�N�e�B�u�A�A�N�e�B�u
	int m_selectItemSizeWigth[SelectItemNum];
	int m_selectItemSizeMaxWigth;

	int m_topTitleAnimeCount;
	bool m_top_animetionFinishFlag;
	int m_top_textBlinkAnimeCount;
	//------------------------------------------------------
	void Draw_PlayStandby();
	void Action_PlayStandby(CInputAllStatus* input);
	int m_playStandby_selectIndex;
	CImage* m_playStandby_selectItem[SelectRankItemNum][2]; // ��A�N�e�B�u�A�A�N�e�B�u
	int m_playStandby_selectItemSizeWigth[SelectItemNum];
	int m_playStandby_selectItemSizeMaxWigth;

	// ��ʂ̏�
	enum Status{
		Top = 0, // TOP���
		PlayStandby, // Play�����āA�����N�⎩�@��I��
		ExtraStandby,

	};
	Status m_status;
};