#include "DxLib.h"
#include "DXFunc.h"
#include "BaseEffect.h"
#include "BaseBullet.h"
#include "Game.h"
#include <float.h>
#include "BattleScene.h"

//---------------------------------------------------------------------------------
//	CEffectManager
//---------------------------------------------------------------------------------
CEffectManager::CEffectManager(int m_num/* = 512*/){
	m_bulletTotalNum = m_num;

	m_bullet = new CBaseBullet*[m_bulletTotalNum];
	for(int i=0;i<m_bulletTotalNum;i++){
		m_bullet[i] = nullptr;
	}

	m_order = 0;
}

CEffectManager::~CEffectManager()
{
	// �e�̃f�X�g���N�^�ŊJ�������
	//delete [] m_bullet;
}

void CEffectManager::Draw(int order) {
	for (int i = 0; i < m_bulletTotalNum; i++) {
		if (m_bullet[i] == nullptr) {
			continue;
		}
		CBaseEffect* effect = (CBaseEffect*)m_bullet[i];
		if (effect->m_order == order) {
			m_bullet[i]->Draw();
		}
	}
}

//---------------------------------------------------------------------------------
//	CBaseEffect
//---------------------------------------------------------------------------------

CBaseEffect::CBaseEffect(int order, EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, int image) :
						CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, image),
	m_order(order)
{
	Set();
	SetImage(image);
}
//CBaseEffect::CBaseEffect(int order, EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, const char* ImageName) :
//						CBaseBullet(type, P, speed, angle, corner, acce, maxSpeed, nearAngle, ImageName),
//	m_order(order)
//{
//	Set();
//	SetImage(ImageName);
//}

CBaseEffect::~CBaseEffect(){

}
void CBaseEffect::Set(){
	m_blendType = DX_BLENDMODE_ADD;
	m_blendDepth = 255.0;
	m_plusBlend = 0.0;
	m_size = 1.0;
	m_plusSize = 0.0;
	funcP = nullptr;
	m_animeEndDelFlg = true;

	m_removeNow = false;
	m_deleteOverSize = DBL_MAX;
	m_deleteTimingPlusAlpha = -10.0;
	m_deleteTimingPlusSize = 0.0;
	m_removeCount = MAXINT;

	m_waitCount = 0;
	m_waitTime = 0;
	m_scroll = true;

	m_drawAddAnimeRotate = 0.0;
	m_drawAnimeRotate = 0.0;
	m_addAcce2 = 0.0;
	m_acce2 = 0.0;
}

void CBaseEffect::Action(){
	if (m_waitTime > 0) {
		if (m_waitCount <= m_waitTime) {
			m_waitCount++;
			return;
		}
	}

	m_acce2 += m_addAcce2;
	m_accelerate += m_acce2;

	//�ړ�
	Move();

	//�G�t�F�N�g�֘A�̓���
	EffectAction();

	if(funcP != nullptr){
		funcP(this);
	}
	m_count++;
}

void CBaseEffect::EffectAction(){

	m_blendDepth += m_plusBlend;

	if (m_plusBlend == 0) {

	} else 
	if (m_plusBlend > 0) {
		if (m_blendDepth < m_maxBlend) {
			m_blendDepth += m_plusBlend;
		}
		else {
			m_blendDepth = m_maxBlend;
		}
	}
	else {
		if (m_blendDepth > m_maxBlend) {
			m_blendDepth -= m_plusBlend;
		}
		else {
			m_blendDepth = m_maxBlend;
		}
	}

	m_size += m_plusSize;


	if(m_blendDepth > 255.0){
		m_removeNow = true;
	}
	if(m_size > m_deleteOverSize){
		m_removeNow = true;
	}
	if(m_count > m_removeCount){
		m_removeNow = true;
	}
	
	//�T�C�Y��255�ȏ�̎��ɂ����Ȃ���������a���Ȃ̂ŁA�k�������t���OON
	if(m_removeNow == true){
		m_blendDepth += m_deleteTimingPlusAlpha;
		m_size += m_deleteTimingPlusSize;
	}

	if(m_blendDepth <= 0.0){
		m_blendDepth = 0;
		m_removeFlg = true;
	}
	if(m_size < 0.0){
		m_size = 0;
		m_removeFlg = true;
	}
}

void CBaseEffect::Draw(){
	if (m_waitTime > 0) {
		if (m_waitCount <= m_waitTime) {
			m_waitCount++;
			return;
		}
	}

	SetDrawBlendMode( m_blendType , (int)m_blendDepth ) ;

	//�A�j���t���O��ON�Ȃ�
	if(m_imageInfo.m_animeFlg == true){
		if(m_count % m_imageInfo.m_animeSpeed == 0){
			m_imageInfo.m_animePos++;
			if(m_imageInfo.m_animePos >= m_imageInfo.m_animeNum){
				//�A�j���[�V����������摜�ŃA�j�����I�������폜���邩�ǂ���
				//true:�폜����	false:���Ȃ�
				//bool m_animeEndDelFlg;
				if(m_animeEndDelFlg == true){
					m_removeFlg = true;
				} else {
					m_imageInfo.m_animePos = 0;
				}
			}
		}
	}
	//��]�t���O��ON�Ȃ�
	if(m_imageInfo.m_rotationFlg == true){
		m_imageInfo.m_rotationAngle += m_imageInfo.m_rotationSpeed;
	}
	//�`��
	if (m_scroll) {
		CPos subPos = CBattleScene::GetBattleScene()->GetBackGroundscrollSmall();
		CDxFunc::MyDrawRotaGraph(m_pos + subPos, m_size, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD + m_drawAnimeRotate, m_image[m_imageInfo.m_animePos]);
	}
	else {
		CDxFunc::MyDrawRotaGraph(m_pos, m_size, m_angle + m_imageInfo.m_rotationAngle + 90.0 / CFunc::RAD + m_drawAnimeRotate, m_image[m_imageInfo.m_animePos]);
	}
	m_drawAnimeRotate += m_drawAddAnimeRotate;

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 ) ;
}



//void CBaseEffect::RectOut(){
//	//�������Ȃ�
//}

void CBaseEffect::Remove(){
	//�������Ȃ�
}

void CBaseEffect::SetFuncP(void (*FuncP)(CBaseEffect* eff)){
	funcP = FuncP;
}

void CBaseEffect::SetBlend(int BlendDepth, double PlusBlend, double MaxBlend /*= 255*/){
	// PlusBlend�����̒l�̏ꍇ�AMaxBlend���w�����邱��
	if (PlusBlend < 0) {
		if (MaxBlend >= BlendDepth) {
			assert(0);
		}
	}
	m_blendDepth = (double)BlendDepth;
	m_maxBlend = MaxBlend;
	m_plusBlend = PlusBlend;
}

void CBaseEffect::SetSize(double Size, double PlusSize){
	m_size = Size;
	m_plusSize = PlusSize;
}

void CBaseEffect::SetAnimeEndDelFlg(bool AnimeEndDelFlg){
	m_animeEndDelFlg = AnimeEndDelFlg;
}

void CBaseEffect::SetDeleteTiming(double DeleteTimingPlusAlpha, double DeleteTimingPlusSize){
	m_deleteTimingPlusAlpha = DeleteTimingPlusAlpha;
	m_deleteTimingPlusSize = DeleteTimingPlusSize;
}

void CBaseEffect::SetRemoveCount(int DelCount){
	m_removeCount = DelCount;
}
// �҂���
void CBaseEffect::SetWaitTime(int waitTime)
{
	m_waitTime = waitTime;
}

//---------------------------------------------------------------------------------
//	CStringEffect
//---------------------------------------------------------------------------------

CStringEffect::CStringEffect(int order, EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, char* DRAWSTR, int FONT) :
					CBaseEffect(order, type, P, speed, angle, corner, acce, maxSpeed, nearAngle, -1){
	fontCr = 0xffffff;
	edgeCr = 0x000000;
	strcpy_s(drawString, STRINGEFFECTMAX, DRAWSTR);
	font = (CFont*)CGame::GetResource(FONT);
}
CStringEffect::CStringEffect(int order, EDirType type, CPos P, double speed, double angle, double corner, double acce, double maxSpeed, double nearAngle, char* DRAWSTR, const char* FONTNAME) :
					CBaseEffect(order, type, P, speed, angle, corner, acce, maxSpeed, nearAngle, -1){
	fontCr = 0xffffff;
	edgeCr = 0x000000;
	strcpy_s(drawString, STRINGEFFECTMAX, DRAWSTR);
	font = (CFont*)CGame::GetResource(FONTNAME);
}


//�f�X�g���N�^
CStringEffect::~CStringEffect(){

}

//���C���s��
void CStringEffect::Action(){
	//�ړ�
	Move();

	//�G�t�F�N�g�֘A�̓���
	EffectAction();

	if(funcP != nullptr){
		funcP(this);
	}
	m_count++;
}

//���C���`��
void CStringEffect::Draw(){
	SetDrawBlendMode( m_blendType , (int)m_blendDepth ) ;
	//CDxFunc::DrawString(pos , drawString, fontCr, font->font, edgeCr);

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 ) ;

}


