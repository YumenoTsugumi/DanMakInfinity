
#pragma once

#include "Game.h"
#include "Resource.h"
#include "BaseBullet.h"
#include <functional>
const int MOVE_HI = 0;
const int MOVE_SLOW = 1;
constexpr int TraceBitNum = 60;
class CBulletManager;
class CPlayer {
public:
	CPos m_pos;

	// 無敵時間
	int m_mutekiCount;
	void MutekiTime();
	bool IsMuteki();
	void SetMuteki(int mutekiTime);

	CPos m_bomberPos;
	double m_bomberDist;
	bool m_bombOn;
	int m_bombCount;
	void Bomber();
	void BomberSet();

	CImage* m_playerImage;
	CImage* m_bitRImage;
	int m_slowMove;	//0(MOVE_HI)通常　1(MOVE_SLOW)低速

	double m_rapidSpeed;
	double m_slowSpeed;

	CImage* m_hitMakerImage;
	double m_hitMakerRotateAngle; // ヒットマーカーの回転角度

	int m_bulletShotCount;
	int m_bulletMainShotCount;
	CBulletManager* m_playerBullet; // 上位のシーンからポインタをもらう

	// バリア中のイメージ
	double m_barrierAlpha;
	double m_barrierAngle;
	CImage* m_playerBarrier1;
	CImage* m_playerBarrier2;

	// 当たり判定
	double m_hitSize;

	// 被弾した
	int m_dieAnimetion;
	void Die();
	bool IsDieAnimetion();

	CPlayer();
	~CPlayer();
	void Init();
	void SetBulletManager(CBulletManager* playerBullet);

	void Action(CInputAllStatus* input);
	void Draw();
	
	void MainShot();
	

	enum ShotType {
		Forward = 0,
		Wide,
		Trace,
		Tilt,
	};
	void SetSpeed(int rapid, int slow);
	void SetShotType(ShotType rapid, ShotType slow);
	ShotType m_rapidShotType;
	ShotType m_slowShotType;
	CPos m_subShotPos[4];
	double m_subShotAngle[4];
	int m_shotChangeCount;
	void SubShotDraw();
	void SubShotAction();
	void SubShotFire();
	std::function<void(CPlayer* player, CPos[4], double[4])> m_rapidSubShotGetFunc;
	std::function<void(CPlayer* player, CPos[4], double[4])> m_slowSubShotGetFunc;

	void SubShot_Forward_Action();
	CPos m_forwardPos[4];
	double m_forwardAngle[4];
	double m_forwardAngles[4];
	static void GetSubShotForward(CPlayer* player, CPos forwardPos[4], double forwardAngle[4]);

	void SubShot_Wide_Action();
	CPos m_widePos[4];
	double m_wideAngle[4];
	static void GetSubShotWide(CPlayer* player, CPos widePos[4], double wideAngle[4]);

	void SubShot_Trace_Action();
	CPos m_tracePos[4];
	CPos m_traceSubPos[TraceBitNum];
	CPos m_tracePrePos;
	double m_traceAngle[4];
	static void GetSubShotTrace(CPlayer* player, CPos tracePos[4], double traceAngle[4]);

	void SubShot_Tilt_Action();
	double m_tiltAngle;
	CPos m_tiltPos[4];
	double m_tiltAngles[4];
	static void GetSubShotTilt(CPlayer* player, CPos tiltPos[4], double tiltAngle[4]);


	void SetBattleScene(CBattleScene* scene) {
		m_scene = scene;
	}
	CBattleScene* m_scene;
};

//---------------------------------------------------------------------------------
//	CBaseBullet
//---------------------------------------------------------------------------------
class CPlayerBullet : public CBaseBullet {
public:
	//コンストラクタ	弾作成
	CPlayerBullet(int damage, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image);
	//CPlayerBullet(int damage, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* imageName);

	double m_hitSize; // 当たり判定
	int m_damage; // 与えるダメージ

	// 弾が何かに当たった時
	void virtual Hit();

	//メイン描画
	virtual void Draw();
};