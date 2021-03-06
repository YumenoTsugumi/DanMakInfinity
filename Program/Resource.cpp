#include "DxLib.h"
#include "resource.h"



//---------------------------------------------------------------------------------
//	マネージャー
//---------------------------------------------------------------------------------
const int CResourceManager::MAXINDEX = 100000;
CResourceManager::CResourceManager() {
	m_indexToMapResource.resize(MAXINDEX);
}
CResourceManager::~CResourceManager() {

}

void CResourceManager::Add(CResource* resource, const char* name, int index) {
	if (resource == nullptr) {
		return;
	}
	if (index < 0 || index >= MAXINDEX) {
		assert(0);
		return;
	}

	m_resource[name] = resource;

	m_indexToMapResource[index] = resource;
}

CResource* CResourceManager::GetResource(const char* name) {
	CResource* res = m_resource[name];
	if (res == nullptr)return nullptr;

	while (!res->m_loadFlg) {
		ProcessMessage();
		res = m_resource[name];
		Sleep(16);
	}
	return m_resource[name];
}
CResource* CResourceManager::GetResource(int index) {
	CResource* res = m_indexToMapResource[index];
	if (res == nullptr)return nullptr;

	while (!res->m_loadFlg) {
		ProcessMessage();
		res = m_indexToMapResource[index];
		Sleep(16);
	}
	return m_indexToMapResource[index];
}



//---------------------------------------------------------------------------------
//	リソースのベースとなるクラス
//---------------------------------------------------------------------------------
CResource::CResource(const char* filename) :
	m_name(filename),
	m_loadFlg(false)
{
}
CResource::~CResource(){
}

//---------------------------------------------------------------------------------
//	イメージ１枚
//---------------------------------------------------------------------------------
CImage::CImage(const char* filename) : CResource(filename)
{
	m_iamge = LoadGraph(filename);
	GetGraphSize( m_iamge , &(m_sizeX) , &(m_sizeY) ) ;

	m_loadFlg = true;

	assert(m_iamge >= 0);
}
CImage::~CImage(){

}


//---------------------------------------------------------------------------------
//	n分割出来るイメージ
//---------------------------------------------------------------------------------
CImages::CImages(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY) :
	CResource(filename) ,
	m_images(nullptr),
	m_num(0),
	m_numX(0), m_numY(0),
	m_sizeX(0), m_sizeY(0)
{
	m_images = (int*)malloc(sizeof(int*)*(numAll));

	//画像読み込み
	LoadDivGraph( filename , numAll , numX , numY , sizeX , sizeY , m_images ) ;

	m_num = numAll;
	m_numX = numX;
	m_numY = numY;
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	m_loadFlg = true;
}
CImages::CImages(const std::vector<std::string>& filenames, int sizeX, int sizeY) :
	CResource("")
{

	m_images = (int*)malloc(sizeof(int*) * (filenames.size()));
	for (int ii = 0; ii < filenames.size(); ii++) {
		m_images[ii] = LoadGraph(filenames[ii].c_str());
	}

	m_num = filenames.size();
	m_numX = filenames.size();
	m_numY = 1;
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	m_loadFlg = true;
}


CImages::~CImages(){
	free(m_images);
}

//---------------------------------------------------------------------------------
//	アニメーションのイメージ
//---------------------------------------------------------------------------------
CImageAnime::CImageAnime(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY, int animeSpeed) : 
	CImages(filename, numAll, numX, numY, sizeX, sizeY) , 
	m_animeSpeed(animeSpeed)
{

}

CImageAnime::CImageAnime(const std::vector<std::string>& filenames, int sizeX, int sizeY, int animeSpeed) :
	CImages(filenames, sizeX, sizeY),
	m_animeSpeed(animeSpeed)
{

}

CImageAnime::~CImageAnime(){
}
//---------------------------------------------------------------------------------
//	弾のイメージ
//---------------------------------------------------------------------------------
CBulletImage::CBulletImage(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY, int animeSpeed, double rotaSpeed) :
	CImageAnime(filename, numAll, numX, numY, sizeX, sizeY, animeSpeed),
	m_rotaSpeed(rotaSpeed)
{
}
CBulletImage::CBulletImage(const std::vector<std::string>& filenames, int sizeX, int sizeY, int animeSpeed, double rotaSpeed) :
	CImageAnime(filenames, sizeX, sizeY, animeSpeed),
	m_rotaSpeed(rotaSpeed)
{
}

CBulletImage::~CBulletImage()
{
}


//---------------------------------------------------------------------------------
//	フォント
//---------------------------------------------------------------------------------
CFont::CFont(const char* fontPath, int m_size /*= -1*/, int m_thick /*= -1*/, int fontType /*= DX_FONTTYPE_ANTIALIASING_EDGE_4X4*/) : 
	CResource(fontPath)
{
	m_font = -1;

	LPCSTR font_path1 = fontPath; // 読み込むフォントファイルのパス
	if (AddFontResourceEx(font_path1, FR_PRIVATE, nullptr) > 0) {
	} else {
		assert(0);
	}
	m_font = CreateFontToHandle( "IPAゴシック" , m_size , m_thick , DX_FONTTYPE_ANTIALIASING_EDGE, -1, 1 ) ;
	m_loadFlg = true;
}
CFont::~CFont(){

}



//---------------------------------------------------------------------------------
//	曲
//---------------------------------------------------------------------------------



//---------------------------------------------------------------------------------
//	効果音
//---------------------------------------------------------------------------------


