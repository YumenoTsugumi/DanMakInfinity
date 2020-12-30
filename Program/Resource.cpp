#include "DxLib.h"
#include "resource.h"



//---------------------------------------------------------------------------------
//	�}�l�[�W���[
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
	while (!res || !res->m_loadFlg) {
		ProcessMessage();
		res = m_resource[name];
		Sleep(16);
	}
	return m_resource[name];
}
CResource* CResourceManager::GetResource(int index) {
	CResource* res = m_indexToMapResource[index];
	while (!res || !res->m_loadFlg) {
		ProcessMessage();
		res = m_indexToMapResource[index];
		Sleep(16);
	}
	return m_indexToMapResource[index];
}



//---------------------------------------------------------------------------------
//	���\�[�X�̃x�[�X�ƂȂ�N���X
//---------------------------------------------------------------------------------
CResource::CResource(const char* filename) :
	m_name(filename),
	m_loadFlg(false)
{
}
CResource::~CResource(){
}

//---------------------------------------------------------------------------------
//	�C���[�W�P��
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
//	n�����o����C���[�W
//---------------------------------------------------------------------------------
CImages::CImages(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY) :
	CResource(filename) ,
	m_images(nullptr),
	m_num(0),
	m_numX(0), m_numY(0),
	m_sizeX(0), m_sizeY(0)
{
	m_images = (int*)malloc(sizeof(int*)*(numAll));

	//�摜�ǂݍ���
	LoadDivGraph( filename , numAll , numX , numY , sizeX , sizeY , m_images ) ;

	m_num = numAll;
	m_numX = numX;
	m_numY = numY;
	m_sizeX = m_sizeX;
	m_sizeY = m_sizeY;

	m_loadFlg = true;
}

CImages::~CImages(){
	free(m_images);
}

//---------------------------------------------------------------------------------
//	�A�j���[�V�����̃C���[�W
//---------------------------------------------------------------------------------
CImageAnime::CImageAnime(const char* filename, int numAll, int numX, int numY, int m_sizeX, int m_sizeY, int animeSpeed) : 
	CImages(filename, numAll, numX, numY, m_sizeX, m_sizeY) , 
	m_animeSpeed(0)
{
	m_animeSpeed = animeSpeed;
}

CImageAnime::~CImageAnime(){
}
//---------------------------------------------------------------------------------
//	�e�̃C���[�W
//---------------------------------------------------------------------------------
CBulletImage::CBulletImage(const char* filename, int numAll, int numX, int numY, int m_sizeX, int m_sizeY, int animeSpeed, double RotaSpeed) : 
	CImageAnime(filename, numAll, numX, numY, m_sizeX, m_sizeY, animeSpeed),
	m_rotaSpeed(0)
{
	m_rotaSpeed = RotaSpeed;
}

CBulletImage::~CBulletImage()
{
}


//---------------------------------------------------------------------------------
//	�t�H���g
//---------------------------------------------------------------------------------
CFont::CFont(const char* fontPath, int m_size /*= -1*/, int m_thick /*= -1*/, int fontType /*= DX_FONTTYPE_ANTIALIASING_EDGE_4X4*/) : 
	CResource(fontPath)
{
	m_font = -1;

	LPCSTR font_path1 = fontPath; // �ǂݍ��ރt�H���g�t�@�C���̃p�X
	if (AddFontResourceEx(font_path1, FR_PRIVATE, nullptr) > 0) {
	} else {
		assert(0);
	}
	m_font = CreateFontToHandle( "IPA�S�V�b�N" , m_size , m_thick , DX_FONTTYPE_ANTIALIASING_EDGE, -1, 1 ) ;
	m_loadFlg = true;
}
CFont::~CFont(){

}



//---------------------------------------------------------------------------------
//	��
//---------------------------------------------------------------------------------



//---------------------------------------------------------------------------------
//	���ʉ�
//---------------------------------------------------------------------------------


