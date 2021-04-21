#include "DxLib.h"
#include "DXFunc.h"
#include "BaseItem.h"
#include "BaseBullet.h"
#include "BaseBullet.h"
#include "Game.h"
#include <float.h>
#include "BattleScene.h"
CBulletImage* CBaseItem::m_shineImage = nullptr;

// �v���C���[���A�C�e������͈͂ɓ�����
const int noTakeCount = 45; // �t���[��
const int fastGetItemCount = 0; // �A�C�e��������悤�ɂȂ���0.5�b�ȓ��ɃA�C�e���擾�͈͂ɓ�����
const int nomalGetItemCount = 60; // �A�C�e��������悤�ɂȂ���0.5+1�b�ȓ��ɃA�C�e���擾�͈͂ɓ�����

//---------------------------------------------------------------------------------
//	CItemManager
//---------------------------------------------------------------------------------
CItemManager::CItemManager(int m_num/* = 512*/){
	m_bulletTotalNum = m_num;

	m_bullet = new CBaseBullet*[m_bulletTotalNum];
	for(int i=0;i<m_bulletTotalNum;i++){
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}

CItemManager::~CItemManager()
{
	// �e�̃f�X�g���N�^�ŊJ�������
}

void CItemManager::Draw() {
	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] == nullptr) {
			continue;
		}
		m_bullet[i]->Draw();
	}
}

//---------------------------------------------------------------------------------
//	CBaseItem
//---------------------------------------------------------------------------------

CBaseItem::CBaseItem(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
						CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, image),
	m_gravity(0.0)
{
	if (m_shineImage == nullptr) {
		m_shineImage = (CBulletImage*)CGame::GetResource(20721);
	}
	Set();
	SetImage(image);
}
//CBaseItem::CBaseItem(EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName) :
//						CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, ImageName),
//	m_gravity(0.0)
//{
//	Set();
//	SetImage(ImageName);
//}

CBaseItem::~CBaseItem(){

}
void CBaseItem::Set(){
	funcP = nullptr;
	m_removeNow = false;

	m_waitCount = 0;
	m_waitTime = 0;
	m_survivalTime = 0;
	m_itemRank = 0;
	m_preRecoveryFlag = false;
}

void CBaseItem::Action(){
	if (m_waitTime > 0) {
		if (m_waitCount <= m_waitTime) {
			m_waitCount++;
			return;
		}
	}

	if (m_plusSize > 0 && m_size < m_maxSize) {
		m_size += m_plusSize;
		if (m_size > m_maxSize) {
			m_size = m_maxSize;
		}
	}
	else if (m_plusSize < 0 && m_size > m_maxSize) {
		m_size += m_plusSize;
		if (m_size < m_maxSize) {
			m_size = m_maxSize;
		}
	}

	if (m_recoveryFlag) {
		m_speed = 12.0;
		m_angle = (CFunc::GetTwoPointAngle(m_pos, m_target) + (180.0 / CFunc::RAD));
		//���x
		m_vel.x = cos(m_angle) * m_speed;
		m_vel.y = sin(m_angle) * m_speed;
		m_pos += m_vel;
	} else {

		//�ړ�
		Move();

		m_pos.y += m_gravity;
		m_gravity += 0.1;
	}

	if(funcP != nullptr){
		funcP(this);
	}

	//�͈͊O����
	RectOut();

	m_count++;
}


void CBaseItem::Draw(){
	if (m_waitTime > 0) {
		if (m_waitCount <= m_waitTime) {
			m_waitCount++;
			return;
		}
	}

	//��]�t���O��ON�Ȃ�
	if(m_imageInfo.m_rotationFlg == true){
		m_imageInfo.m_rotationAngle += m_imageInfo.m_rotationSpeed;
	}

	//�`��
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	CPos subPos = CBattleScene::GetBattleScene()->GetBackGroundscrollSmall();
	int itemRank = GetSurvivalTimeRank();
	if (m_recoveryFlag) {
		itemRank = m_itemRank; // �����擾���[�h�ɂȂ��Ă�Ȃ�A���̎����_�̒l�ɂ���
	}
	int shineImage = m_shineImage->m_images[0];
	if (itemRank == 3 || itemRank == 2) {
		double blend = 255.0 - 255.0 * ((double)(m_count) / (noTakeCount + fastGetItemCount + nomalGetItemCount)); // �A�C�e�����擾�ł��鎞�� / �A�C�e�����P���Ă��鎞��
		
		SetDrawBlendMode(DX_BLENDMODE_ADD, blend);
		CDxFunc::MyDrawRotaGraph(m_pos+ subPos, m_size * 2.0, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, shineImage);

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		CDxFunc::MyDrawRotaGraph(m_pos + subPos, m_size, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, m_image[m_imageInfo.m_animePos]);
	}
	else {
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
		CDxFunc::MyDrawRotaGraph(m_pos + subPos, m_size, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD, m_image[m_imageInfo.m_animePos]);
	}
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 ) ;
}

void CBaseItem::RectOut(){
	//�̈�O�ɏo����폜
	if (//m_pos.x <	CGame::GetGameRect().leftUp.x - m_imageInfo.m_sizeX / 2 ||
		//m_pos.x >	CGame::GetGameRect().rightDown.x + m_imageInfo.m_sizeX / 2 ||
		//m_pos.y <	CGame::GetGameRect().leftUp.y - m_imageInfo.m_sizeY / 2 ||
		m_pos.y >	CGame::GetGameRect().rightDown.y + m_imageInfo.m_sizeY / 2) {
		m_removeFlg = true;
	}
}

void CBaseItem::Remove(){
	//�������Ȃ�
}

void CBaseItem::SetFuncP(void (*FuncP)(CBaseItem* eff)){
	funcP = FuncP;
}

// �҂���
void CBaseItem::SetWaitTime(int waitTime)
{
	m_waitTime = waitTime;
}

void CBaseItem::SetSize(double size, double plusSize, double maxSize) {
	m_size = size;
	m_plusSize = plusSize;
	m_maxSize = maxSize;
}


int CBaseItem::GetSurvivalTimeRank() {
	if (m_count <= noTakeCount + fastGetItemCount) {
		return 3; // ���������
	}
	else if (m_count <= noTakeCount + fastGetItemCount + nomalGetItemCount) {
		return 2; // ���ʂ̑��x�Ŏ����
	}
	else {
		return 1; // �擾�x����
	}
}

void CBaseItem::SetRecovery() {
	m_recoveryFlag = true;
	m_itemRank = GetSurvivalTimeRank();
}
void CBaseItem::SetPreRecovery()
{
	m_preRecoveryFlag = true;
	m_itemRank = GetSurvivalTimeRank();
}

// �A�C�e���ɂ��X�R�A���Z
void CBaseItem::GetItemAddScore() {
	CBattleScene::AddItem(m_itemRank);
}

void CBaseItem::SetRemove()
{
	m_removeFlg = true;
}

// �o�Ă���A�C�e������莞�Ԃ͎��Ȃ��悤�ɂ��邽�߂̔���
bool CBaseItem::IsTakeTime()
{
	if (m_count < noTakeCount) {
		return false;
	}
	if (m_preRecoveryFlag) {
		m_recoveryFlag = true;
	}
	return true;
}

// �ǂ������̃t���O�������Ă�����A��������͈͂̔�����s��Ȃ�
bool CBaseItem::GetRecoveryFlag() {
	return m_recoveryFlag || m_preRecoveryFlag;
}
