#pragma once

//class COtherInfo;
//class CPlayer;
//class CArea;
//class CInhBulletManager;
class CGame;

#include "Scene.h"

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

	//PointerSet pointerSet;

	//CGame *game;
	//CPlayer *player;
	//COtherInfo *otherInfo;
	//CArea *area;
	//CInhBulletManager *bulletManeger;

};