#pragma once

#include "resource.h"
#include "BaseEffect.h"
class CBattleScene;

class CBackGround {
public:
	CBackGround();
	~CBackGround();

	CEffectManager m_effectManager;
	virtual void Action();
	virtual void Draw();

};


class CBackGroundPatternA : public CBackGround {
public:
	CBackGroundPatternA();
	~CBackGroundPatternA();
	virtual void Action();
	virtual void Draw();

	void Set();

	int m_bgIndex; // 使用している背景のインデックス
	static std::vector<int> m_usedBgIndex; // 使用した背景のインデックス

	int m_planetIndex; // 使用している背景のインデックス
	static std::vector<int> m_usedPlanetIndex; // 使用した背景のインデックス

	double m_feedAlpha;
	void SetFeedAlpha(double alpha);
	//---------------------------------------------

	double m_scrollNearY; // 近い背景
	double m_scrollFarwayY; // 遠く背景

	enum ImagePosLR {
		L = 0,
		R = 1,
	};
	ImagePosLR m_SpaceNearLR; // 背景がLかR
	ImagePosLR m_SpaceFarwayLR; // 背景がLかR
	CPos m_posSpaceNear[2]; // 近くの背景の座標
	CPos m_posSpaceFarwayY[2]; // 遠くの背景の座標
	CImage* m_imageNearSpace; // 近くの背景のイメージ(星だけのシンプルな方)
	CImage* m_imageFarwaySpace; // 遠くの背景のイメージ(もやもやしている方)


	double m_scrollPlanetY; // 星のスクロール速度
	double m_sizePlanet;
	double m_angleAngle;
	CPos m_posPlanet; // 星の座標
	CImage* m_imagePlanet; // 星のイメージ


	CPos m_initPlayerPos; // プレイヤーの初期位置
	CPos m_movedPlayerPos; // プレイヤーの移動後の初期位置
	virtual void SetInitPlayerPos(CPos& initPos);
	virtual void SetPlayerMovedPos(CPos& movedPos);

	void SetBattleScene(CBattleScene* scene) {
		m_scene = scene;
	}
	CBattleScene* m_scene;
};
