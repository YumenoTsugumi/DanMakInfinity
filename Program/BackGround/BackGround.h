#pragma once

#include "resource.h"
#include "BaseEffect.h"

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

	double m_scrollNearY; // 近い背景
	double m_scrollFarwayY; // 遠く背景

	CPos m_posSpaceNear; // 近くの背景の座標
	CPos m_posSpaceFarwayY; // 遠くの背景の座標

	CImage* m_imageNearSpace; // 近くの背景のイメージ(星だけのシンプルな方)
	CImage* m_imageFarwaySpace; // 遠くの背景のイメージ(もやもやしている方)


	double m_scrollPlanetY; // 星のスクロール速度
	CPos m_posPlanet; // 星の座標
	CImage* m_imagePlanet; // 星のイメージ


	CPos m_initPlayerPos; // プレイヤーの初期位置
	CPos m_movedPlayerPos; // プレイヤーの移動後の初期位置
	virtual void SetInitPlayerPos(CPos& initPos);
	virtual void SetPlayerMovedPos(CPos& movedPos);
};
