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
#include "TitleScene.h"

constexpr int nameKeta = 10;
constexpr int nameKetaEnd = 11;
constexpr int keyInputCount = 26 + 10+1;
class CResultScene : public CScene{
public:
	//------------------
	//	おまじない
	//------------------
	CResultScene(int InTime);
	virtual ~CResultScene();

	//行動	引数isTopSceneは一番上の描画かどうかである。上から渡されるので気にしなくて良い
	virtual void Main(CInputAllStatus *input);
	void Draw2();
	//------------------
	//	ここから下は自由に
	//------------------
	void Init(CGame* gameP);

	CTitleScene* m_titleScene;
	void SetTitleScene(CTitleScene* scene) {
		m_titleScene = scene;
	}

	void SetData(
		int startRank, int endRank,
		int stage, int item, int liveTime,
		int rapidShot, int rapidspeed, int slowShot, int slowspeed, long long score
	);
	int m_startRank;
	int m_endRank;
	int m_stage;
	int m_item;
	int m_liveTime;
	int m_rapidShot;
	int m_rapidspeed;
	int m_slowShot;
	int m_slowspeed;
	long long m_score;


	CImage* m_gameOver;
	double m_gameOverAlpha;
	int m_phase1_count;

	// スコア表示用
	CImage* m_strScoreImage[10][2];
	int m_strScoreImageMaxX;
	int m_strScoreImageMaxY;
	// ネームエントリー用
	CImage* m_strImage[37][2];
	int m_strImageMaxX;
	int m_strImageMaxY;

	// リザルト用
	CImage* m_numberImage[10];

	CImage* m_imageEnd;
	CImage* m_imageEndA;
	CImage* m_imageEndRank;
	CImage* m_imageItem;
	CImage* m_imageLiveTime;
	CImage* m_imageName;
	CImage* m_imageScore;
	CImage* m_imageSmall_s;
	CImage* m_imageStage;
	CImage* m_imageStartRank;

	CImage* m_imageRapid;
	CImage* m_imageSlow;
	CImage* m_imageShotType;
	CImage* m_imageMoveSpeed;
	CImage* m_imageShots[4];

	CImage* m_imagePlanet;
	CImage* m_imageSpace;

	CImage* m_imageArrowA;
	CImage* m_imageArrowB;

	int m_nameEntryIndex;
	int m_nameEntry[nameKeta];

	int m_keyInputCount[keyInputCount];


	void Conver_NameIndexToChar(char name[12]);
};