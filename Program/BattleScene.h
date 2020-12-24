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
	//	おまじない
	//------------------
	CBattleScene(int InTime);
	virtual ~CBattleScene();

	//行動	引数isTopSceneは一番上の描画かどうかである。上から渡されるので気にしなくて良い
	virtual void Main(CInputAllStatus *input);

	//------------------
	//	ここから下は自由に
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