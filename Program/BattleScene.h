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

	//PointerSet pointerSet;

	//CGame *game;
	//CPlayer *player;
	//COtherInfo *otherInfo;
	//CArea *area;
	//CInhBulletManager *bulletManeger;

};