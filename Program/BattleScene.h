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
#include "BattleResultUI.h"
#include "StageManager.h"

constexpr int RankBasedDigit = 10000;

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
	CPos m_initPlayerPos; // �v���C���[�̏����ʒu
	CPos GetBackGroundscrollSmall() {
		CPos subPos; // �w�i�ƃv���C���[�̈ʒu�̈ʑ�
		subPos.x = (m_initPlayerPos.x - m_player.m_pos.x) / 5.0;
		subPos.y = 0;
		return subPos;
	}
	CPos GetBackGroundscrollLarge() {
		CPos subPos; // �w�i�ƃv���C���[�̈ʒu�̈ʑ�
		subPos.x = (m_initPlayerPos.x - m_player.m_pos.x) / 10.0;
		subPos.y = 0;
		return subPos;
	}
	CPos GetPlayerPosByScroll() {
		return m_player.m_pos - GetBackGroundscrollSmall();
	}

	static CBattleScene* m_scene;
	static void SetBattleScene(CBattleScene* scene) {
		m_scene = scene;
	}
	static CBattleScene* GetBattleScene() {
		return m_scene;
	}

	CBulletManager m_playerBullet; // �v���C���[�̒e

	static CEnemyManager m_enemyManager; // �G
	static CBulletManager m_bulletManager; // �G�̒e
	static CBeamManager m_beamManager; // �G�̃r�[��

	StageManager m_stageManager; // �G�̃X�|�i�[
	bool m_battleResultUIReset;
	CBattleResultUI m_battleResultUI; // �������

	int m_feedinCount;
	int m_feedoutCount;
	CBackGroundPatternA* m_activeBg;
	CBackGroundPatternA* m_nonActiveBg;
	CBackGroundPatternA m_bgA; // �w�i1
	CBackGroundPatternA m_bgB; // �w�i2

	CBattleSceneUI m_ui;

	//-------------------------
	// �X�e�[�W��
	static int m_spawneSmallCount;
	static int m_spawneMediumCount;
	static int m_spawneLargeCount;
	static int m_spawneDropItemCount;
	static void AddSpawneSmallCount() { m_spawneSmallCount++; }
	static void AddSpawneMediumCount() { m_spawneMediumCount++; }
	static void AddSpawneLargeCount() { m_spawneLargeCount++; }
	static void AddDropItemCount() { m_spawneDropItemCount++; }

	static int m_destroySpawneSmallCount;
	static int m_destroySpawneMediumCount;
	static int m_destroySpawneLargeCount;
	static int m_getSpawneDropItemCount;
	static void AddDestorySpawneSmallCount() { m_destroySpawneSmallCount++; }
	static void AddDestorySpawneMediumCount() { m_destroySpawneMediumCount++; }
	static void AddDestorySpawneLargeCount() { m_destroySpawneLargeCount++; }
	static void AddGetDropItemCount() { m_getSpawneDropItemCount++; }

	static int m_usedBomb;
	static int m_missCount;
	static void AddUsedBomb() { m_usedBomb++; }
	static void AddMissCount() { m_missCount++; }

	static void StageCountReset();

	enum ClearRank {
		Rank_S = 0, Rank_A = 1, Rank_B = 2, Rank_C = 3, Rank_D = 4, Rank_E = 5,
	};
	void StageClearResult();
	std::tuple<ClearRank, int ,int> CalcClearBounus(double destoryLargeEnemyRatio,double destoryMediumEnemyRatio,double destorySmallEnemyRatio,
		double usedBomb,double missCount,double getItemRatio);

	// �擾�A�C�e�����v��
	static int m_takeItemRankCount[3]; // ���ꂼ��̃����N���ɂƂ�����
	static void AddItem(int itemRank);
	static void GetItemCount(int& total, int& rank3Item, int& rank2Item, int& rank1Item) {
		rank3Item = m_takeItemRankCount[2];
		rank2Item = m_takeItemRankCount[1];
		rank1Item = m_takeItemRankCount[0];
		total = rank3Item * 3 + rank2Item * 2 + rank1Item;
	}
	// �����N

	// �v���C���[�̏��
	static int m_haveBomb;
	static int m_haveLife;
	static int GetHaveBomb() { return m_haveBomb; }
	static int GetHaveLife() { return m_haveLife; }
	static void BombDecrement() { m_haveBomb--; }
	static void LifeDecrement() { m_haveLife--; }

	// �X�R�A
	static long long m_hiScore;
	static long long m_score;
	static int m_rank; // rank��10000��1���Ǘ�����
	static double m_rankRatio; // 1.0 0.5�`10.0
	static long long GetHiScore() {	return m_hiScore;	}
	static long long GetScore() { return m_score; }
	static void AddScore(int addScore);
	static int GetRank() { return m_rank / RankBasedDigit; }
	static void AddRank(int delta);
	static double GetRankRatio() { return m_rankRatio; }
	static void SetRankRatio(double ratio);
	static void AddRankRatio(double delta);
	static void AddRankRatioByStageSpan(); // �X�e�[�W����span
	static void AddRankRatioByStageClear(int resultrank); // S0 E5
	
	static CPos m_playerPos;
	static CPos GetPlayerPos() {return m_playerPos;	}
	static void SetPlayerPos(const CPos pos) {m_playerPos = pos;}

	// ���݃X�e�[�W
	static int m_nowStage;
	static int GetNowStage() { return m_nowStage; }
	static void AddNowStage() { m_nowStage++; };

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
	static void RemoveBulletByMidiumEnemy(int id); // �e��������
	static void SetBulletRemoveTime(BulletRemoveType type, int time); // �e����
	static BulletRemoveType m_bulletRemoveType;
	static int m_bulletRemoveTime;
	static int m_bulletRemoveCount;

	void DestoryAllEnemyNothingItemDrop(); // �S�G�j��
	void DamageAllEnemy(int damage);

	// BattleSceneCollision.cpp�ɂĎ���
	void Collision_Enemy_PulyerBullet(); // �G�@���@�̒e
	void Collision_Item_Player(); // �A�C�e���@���@
	void Collision_EnemyBullet_Pulyer(); // �G�̒e�@���@

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