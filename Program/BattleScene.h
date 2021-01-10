#pragma once

#include "Game.h"
#include "BaseBullet.h"
#include "Scene.h"
#include "Player.h"
#include "BaseBeam.h"

#include "Launcher.h"
#include "BaseEnemy.h"
#include "BaseItem.h"
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
	static CItemManager m_itemManager; // �A�C�e���}�l�[�W���[

	// �e����
	enum BulletRemoveType {
		Nothing = 0,
		Item = 1
	};
	void RemoveBullet(); // �e��������
	static void SetBulletRemoveTime(BulletRemoveType type, int time);
	static BulletRemoveType m_bulletRemoveType;
	static int m_bulletRemoveTime;
	static int m_bulletRemoveCount;

	// BattleSceneCollision.cpp�ɂĎ���
	void Collision_Enemy_PulyerBullet(); // �G�@���@�̒e
	void Collision_Item_Player(); // �A�C�e���@���@

};