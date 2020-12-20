#include "DxLib.h"
#include "DXFunc.h"
#include "BaseEffect.h"
#include "BaseBullet.h"

#include <float.h>

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

CEffectManager::~CEffectManager(){

	delete [] m_bullet;
}

//---------------------------------------------------------------------------------
//	CBaseEffect
//---------------------------------------------------------------------------------

CBaseEffect::CBaseEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, int IMG) :
						CBaseBullet(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, IMG){
	Set();
	//SetImage(IMG);
}
CBaseEffect::CBaseEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, const char* IMAGENAME) :
						CBaseBullet(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, IMAGENAME){
	Set();
	//SetImage(IMAGENAME);
}

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
}

void CBaseEffect::Action(){
	//移動
	Move();

	//エフェクト関連の動作
	EffectAction();

	if(funcP != nullptr){
		funcP(this);
	}
	m_count++;
}

void CBaseEffect::EffectAction(){

	m_blendDepth += m_plusBlend;
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
	
	//サイズが255以上の時にいきなり消したら違和感なので、縮小薄くフラグON
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
	SetDrawBlendMode( m_blendType , (int)m_blendDepth ) ;

	//アニメフラグがONなら
	if(m_imageInfo.animeFlg == true){
		if(m_count % m_imageInfo.animeSpeed == 0){
			m_imageInfo.animePos++;
			if(m_imageInfo.animePos >= m_imageInfo.animeNum){
				//アニメーションがある画像でアニメが終わったら削除するかどうか
				//true:削除する	false:しない
				//bool m_animeEndDelFlg;
				if(m_animeEndDelFlg == true){
					m_removeFlg = true;
				} else {
					m_imageInfo.animePos = 0;
				}
			}
		}
	}
	//回転フラグがONなら
	if(m_imageInfo.rotationFlg == true){
		m_imageInfo.rotationAngle += m_imageInfo.rotationSpeed;
	}
	//描画
	CDxFunc::DrawRotaGraph(m_pos, m_size, m_angle + m_imageInfo.rotationAngle + 90.0/CFunc::RAD, m_image[m_imageInfo.animePos]);

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 ) ;
}



//void CBaseEffect::RectOut(){
//	//何もしない
//}

void CBaseEffect::Remove(){
	//何もしない
}

void CBaseEffect::SetFuncP(void (*FuncP)(CBaseEffect* eff)){
	funcP = FuncP;
}



void CBaseEffect::SetBlend(int BlendDepth, double PlusBlend){
	m_blendDepth = (double)BlendDepth;
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


//---------------------------------------------------------------------------------
//	CStringEffect
//---------------------------------------------------------------------------------

CStringEffect::CStringEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, char* DRAWSTR, int FONT) : 
					CBaseEffect(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, -1){
	fontCr = 0xffffff;
	edgeCr = 0x000000;
	strcpy_s(drawString, STRINGEFFECTMAX, DRAWSTR);
	font = (CFont*)m_imageManager->GetResource(FONT);
}
CStringEffect::CStringEffect(bool TYPE, CPos P, double SPEED, double ANGLE, double CORNER, double ACCE, double MAXSP, char* DRAWSTR, const char* FONTNAME) : 
					CBaseEffect(TYPE, P, SPEED, ANGLE, CORNER, ACCE, MAXSP, -1){
	fontCr = 0xffffff;
	edgeCr = 0x000000;
	strcpy_s(drawString, STRINGEFFECTMAX, DRAWSTR);
	font = (CFont*)m_imageManager->GetResource(FONTNAME);
}


//デストラクタ
CStringEffect::~CStringEffect(){

}

//メイン行動
void CStringEffect::Action(){
	//移動
	Move();

	//エフェクト関連の動作
	EffectAction();

	if(funcP != nullptr){
		funcP(this);
	}
	m_count++;
}

//メイン描画
void CStringEffect::Draw(){
	SetDrawBlendMode( m_blendType , (int)m_blendDepth ) ;
	//CDxFunc::DrawString(pos , drawString, fontCr, font->font, edgeCr);

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 ) ;

}


