#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"
#include "BaseBeam.h"

#include "Launcher.h"
#include "BaseEnemy.h"

#include "BackGround.h"

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

	CGame *m_game; // ��ʂ̃Q�[���N���X
	CPlayer m_player; // �v���C���[���g
	CBulletManager m_playerBullet; // �v���C���[�̒e

	//std::vector<CBaseEnemy*> m_enemys; // �G
	CEnemyManager m_enemyManager; // �G
	CBulletManager m_bulletManeger; // �G�̒e
	CBeamManager m_beamManeger; // �G�̃r�[��

	CBackGroundPatternA m_bg; // �w�i

	// �Q�Ƃ���͈͂��L���̂ŃO���[�o���ɂ���
	static CEffectManager m_effectManager; // �G�t�F�N�g


	// BattleSceneCollision.cpp�ɂĎ���
	void Collision_Enemy_BulyerBullet(); // �G�@���@�̒e

};