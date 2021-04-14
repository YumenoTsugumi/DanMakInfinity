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
#include "GameDefine.h"
#include "BattleSceneUI.h"
#include "StageManager.h"

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

	static CEnemyManager m_enemyManager; // �G
	static CBulletManager m_bulletManager; // �G�̒e
	static CBeamManager m_beamManager; // �G�̃r�[��

	StageManager m_stageManager; // �G�̃X�|�i�[

	CBackGroundPatternA m_bg; // �w�i

	CBattleSceneUI m_ui;

	
	// �擾�A�C�e�����v��
	static int m_takeItemRankCount[3]; // ���ꂼ��̃����N���ɂƂ�����
	static void AddItem(int itemRank);
	static void GetItemCount(int& total, int& rank3Item, int& rank2Item, int& rank1Item) {
		rank3Item = m_takeItemRankCount[2];
		rank2Item = m_takeItemRankCount[1];
		rank1Item = m_takeItemRankCount[0];
		total = rank3Item * 3 + rank2Item * 2 + rank1Item;
	}

	// �X�R�A
	static long long m_hiScore;
	static long long m_score;
	static int m_rank;
	static long long GetHiScore() {	return m_hiScore;	}
	static long long GetScore() { return m_score; }
	static void AddScore(int addScore);
	static int GetRank() { return m_rank; }
	static void AddRank(int delta);
	
	static CPos m_playerPos;
	static CPos GetPlayerPos() {return m_playerPos;	}
	static void SetPlayerPos(const CPos pos) {m_playerPos = pos;}

	// ���̑�
	//CPos testLauncherPos; // �e�X�g�C��ʒu
	//CLauncher999* testLauncher; // �e�X�g�C��

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


	// �G�o�����[�`��
	void DebugAllEnemyDirection();

	// �f�o�b�O�R�}���h
	void DebugCommand();

	static bool m_enemyHitSizeDraw; // �G�̓����蔻���\������
	static bool GetEnemyHitSizeDraw() {
		return m_enemyHitSizeDraw;
	}
	static bool m_enemyLauncherDraw; // �G�̖C��̈ʒu��\������
	static bool GetEnemyLauncherDraw() {
		return m_enemyLauncherDraw;
	}
	static bool m_enableDebugCommand; // �f�o�b�O�R�}���h��L���ɂ���
};