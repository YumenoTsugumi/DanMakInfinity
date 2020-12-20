#include "DxLib.h"
#include "resource.h"

//---------------------------------------------------------------------------------
//	リソースのベースとなるクラス
//---------------------------------------------------------------------------------
void CResource::SetInfo(const char* name, int number){
	int len = (int)strlen(name);
	if(len >= RESOURCENAMEMAX-1){
		return ;
	}
	strcpy_s(info.m_name, RESOURCENAMEMAX, name);
	info.m_number = number;
}

CResource::CResource(){
	Init();
}
CResource::~CResource(){
}

void CResource::Init(){
	info.m_index = -1;
	strcpy_s(info.m_name, RESOURCENAMEMAX, "noname");
	info.m_number = 0;
	info.m_loadFlg = false;
}

void CResource::SetIndex(int Idx){
	info.m_index = Idx;
}

//---------------------------------------------------------------------------------
//	イメージ１枚
//---------------------------------------------------------------------------------
CImage::CImage(const char* filename) : CResource(){
	m_iamge = LoadGraph(filename);
	GetGraphSize( m_iamge , &(m_sizeX) , &(m_sizeY) ) ;

	info.m_loadFlg = true;

	assert(m_iamge >= 0);
}

CImage::~CImage(){

}
//---------------------------------------------------------------------------------
//	イメージ２枚で１つ
//---------------------------------------------------------------------------------
CImageSet::CImageSet(int Num, const char Filename[][RESOURCENAMEMAX]){
	m_num = Num;
	m_image = (CImage**)malloc(sizeof(CImage*)*(m_num));

	for(int i=0;i<m_num;i++){
		m_image[i] = new CImage(Filename[i]);
	}
}
CImageSet::~CImageSet(){
	for(int i=0;i<m_num;i++){
		delete m_image[i];
	}
	free(m_image);
}
CImage* CImageSet::GetImage(int Idx){
	return m_image[Idx];
}

//---------------------------------------------------------------------------------
//	n分割出来るイメージ
//---------------------------------------------------------------------------------
CImages::CImages(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY){
	m_images = (int*)malloc(sizeof(int*)*(numAll));

	//画像読み込み
	LoadDivGraph( filename , numAll , numX , numY , sizeX , sizeY , m_images ) ;

	m_num = numAll;
	m_numX = numX;
	m_numY = numY;
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	info.m_loadFlg = true;
}

CImages::~CImages(){
	free(m_images);
}
//---------------------------------------------------------------------------------
//	アニメーションのイメージ
//---------------------------------------------------------------------------------

CImageAnime::CImageAnime(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY, int animeSpeed) : 
				CImages(filename, numAll, numX, numY, sizeX, sizeY){
	m_animeSpeed = animeSpeed;
	info.m_loadFlg = true;
}

CImageAnime::~CImageAnime(){
	free(m_images);
}
//---------------------------------------------------------------------------------
//	弾のイメージ
//---------------------------------------------------------------------------------
CBulletImage::CBulletImage(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY, int animeSpeed, double RotaSpeed) : 
				CImageAnime(filename, numAll, numX, numY, sizeX, sizeY, animeSpeed){
	m_rotaSpeed = RotaSpeed;
	info.m_loadFlg = true;
}

CBulletImage::~CBulletImage(){
	free(m_images);
}


//---------------------------------------------------------------------------------
//	フォント
//---------------------------------------------------------------------------------
CFont::CFont(const char* fontPath, int m_size /*= -1*/, int m_thick /*= -1*/, int fontType /*= DX_FONTTYPE_ANTIALIASING_EDGE_4X4*/){
	m_font = -1;

	LPCSTR font_path1 = fontPath; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path1, FR_PRIVATE, nullptr) > 0) {
	} else {
		assert(0);
	}
	m_font = CreateFontToHandle( "IPAゴシック" , m_size , m_thick , DX_FONTTYPE_ANTIALIASING_EDGE, -1, 1 ) ;
	info.m_loadFlg = true;
}
CFont::~CFont(){

}



//---------------------------------------------------------------------------------
//	曲
//---------------------------------------------------------------------------------



//---------------------------------------------------------------------------------
//	効果音
//---------------------------------------------------------------------------------


//---------------------------------------------------------------------------------
//	マネージャー
//---------------------------------------------------------------------------------


CResourceManager::CResourceManager(){

}
CResourceManager::~CResourceManager(){
	m_resource.clear();
}

int CResourceManager::Add(CResource* resource, const char* Name, int Number){
	if(resource == nullptr){
		return -1;
	}
	resource->SetInfo(Name, Number);
	m_resource.push_back(resource);
	int rtnIdx = (int)m_resource.size();
	resource->SetIndex(rtnIdx);

	return rtnIdx;
}

CResource* CResourceManager::GetResource(int number){
	int size = (int)m_resource.size();
	for(int i=0; i< size; i++){
		CResource* p = (CResource*)(m_resource[i]);
		if(p->info.m_number == number){
			return p;
		}
	}
	assert(0);
	return nullptr;
}

CResource* CResourceManager::GetResource(const char* Name){
	int size = (int)m_resource.size();
	for(int i=0; i< size; i++){
		CResource* p = (CResource*)(m_resource[i]);
		int rtn = strcmp(p->info.m_name, Name);
		if(rtn == 0){
			return p;
		}
	}
	assert(0);
	return nullptr;
}


