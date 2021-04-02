#pragma once

#include "resource.h"
#include "StageManager.h"

		
class SpawnerSmallTop_Stop : public SpawnerBase {		
public:											
	SpawnerSmallTop_Stop();
	virtual ~SpawnerSmallTop_Stop();					
	virtual void Spawne() override;				
	double m_returnAngle;
	int m_returnPettern; // 帰りのパターン
	double m_appearancePosition; // 出現位置誤差
};		

class SpawnerSmallLeftRight_Stop : public SpawnerBase {
public:
	SpawnerSmallLeftRight_Stop();
	virtual ~SpawnerSmallLeftRight_Stop();
	virtual void Spawne() override;

	int m_spawnerPettern; // 出現パターン
	double m_spawnerTargetPosX; // 出現後の移動先の位置
	double m_appearancePosition; // 出現位置誤差

	int m_returnPettern; // 帰りのパターン
	int m_returnAngle;

};
	
class SpawnerSmallTop_Repeat_Stop : public SpawnerBase {
public:
	SpawnerSmallTop_Repeat_Stop();
	virtual ~SpawnerSmallTop_Repeat_Stop();
	virtual void Spawne() override;

	int spawnerPettern; // 出現パターン
	double spawnerTargetPosX; // 出現後の移動先の位置
	double m_appearancePosition; // 出現位置誤差

	int returnPettern; // 帰りのパターン
};


class SpawnerMedium : public SpawnerBase {	
public:										
	SpawnerMedium();						
	virtual ~SpawnerMedium();				
	virtual void Spawne() override;		

	int spawnerPettern; // 出現パターン
	double m_appearancePosition; // 出現位置誤差

	virtual CBaseEnemy* GetEnemy(const CPos& pos);
};											
