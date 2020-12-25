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

	CImages* m_images;
	CBulletImage* m_image01;
	CBulletImage* m_image02;
	CBulletImage* m_image03;
	CBulletImage* m_image04;
	CBulletImage* m_image05;
	CBulletImage* m_image06;
};
