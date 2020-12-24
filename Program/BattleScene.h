#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"
#include "BaseBeam.h"

#include "Launcher.h"
#include "BaseEnemy.h"

class CBattleScene : public CScene{
public:
	//------------------
	//	���܂��Ȃ�
	//------------------
	CBattleScene(int InTime);
	virtual ~CBattleScene();

	//�s��	����isTopScene�͈�ԏ�̕`�悩�ǂ����ł���B�ォ��n�����̂ŋC�ɂ��Ȃ��ėǂ�
	virtual void Main(CInputAllStatus *input);

	//------------------
	//	�������牺�͎��R��
	//------------------
	void Init(CGame* gameP);

	CGame *m_game;
	CPlayer m_player;
	CBulletManager m_bulletManeger;
	CBeamManager m_beamManeger;


	CBaseBeam* m_beam1;
	CPos m_pos;


	std::vector<BaseLauncher*> m_launcher;

	std::vector<BaseEnemy*> m_enemys;
};