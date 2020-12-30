#pragma once

#include "Func.h"
#include "DxLib.h"

#include <map>
#include <string>

class CResourceInfo;
class CImage;
class CImages;
class CImageAnime;
class CBulletImage;
class CFont;
class CBgm;
class CSe;
class CResourceManager;

class CResource;

//---------------------------------------------------------------------------------
//	���\�[�X�̃}�l�[�W��
//---------------------------------------------------------------------------------
class CResourceManager {
	static const int MAXINDEX;
public:
	std::map<std::string, CResource*> m_resource;
	std::vector<CResource*> m_indexToMapResource;

	void Add(CResource* resource, const char* name, int index);
	CResource* GetResource(const char* name);
	CResource* GetResource(int index);

	CResourceManager();
	~CResourceManager();
};


//---------------------------------------------------------------------------------
// ���\�[�X�̃x�[�X�ƂȂ�N���X
//---------------------------------------------------------------------------------
class CResource{
public:
	//�Ǘ���	�������w�肵�Ĉ��������Ă���
	std::string m_name;
	int m_index;

	//�ǂݍ��ݏI������
	//false:���ǂݍ���
	bool m_loadFlg;

	CResource(const char* filename);
	virtual ~CResource();
};
//---------------------------------------------------------------------------------
// �C���[�W�P��
//---------------------------------------------------------------------------------
class CImage : public CResource {
public:
	CImage(const char* filename);
	virtual ~CImage();

	int m_iamge;
	int m_sizeX, m_sizeY;
};

//---------------------------------------------------------------------------------
//	n�����o����C���[�W
//---------------------------------------------------------------------------------
class CImages : public CResource{
public:
	CImages(const char* filename, int numAll, int numX, int numY, int m_sizeX, int m_sizeY);
	virtual ~CImages();

	int *m_images;
	int m_num;
	int m_numX, m_numY;
	int m_sizeX, m_sizeY;
};
//---------------------------------------------------------------------------------
// �A�j���[�V�����̃C���[�W
//---------------------------------------------------------------------------------
class CImageAnime : public CImages{
public:
	CImageAnime(const char* filename, int numAll, int numX, int numY, int m_sizeX, int m_sizeY, int m_animeSpeed);
	virtual ~CImageAnime();

	int m_animeSpeed;
};
//---------------------------------------------------------------------------------
// �e�̃C���[�W
//---------------------------------------------------------------------------------
class CBulletImage : public CImageAnime{
public:
	CBulletImage(const char* filename, int numAll, int numX, int numY, int m_sizeX, int m_sizeY, int m_animeSpeed, double RotaSpeed);
	virtual ~CBulletImage();

	double m_rotaSpeed;
};
//---------------------------------------------------------------------------------
//�@�t�H���g
//---------------------------------------------------------------------------------
class CFont : public CResource{
public:
	CFont(const char* fontPath, int m_size = -1, int m_thick = -1, int fontType = DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	
	virtual ~CFont();

	int m_font;
};
//---------------------------------------------------------------------------------
//	��
//---------------------------------------------------------------------------------
class CBgm : public CResource{
public:
	CBgm();
	virtual ~CBgm();

	int m_BGM;
};
//---------------------------------------------------------------------------------
//	���ʉ�
//---------------------------------------------------------------------------------
class CSe : public CResource{
public:
	CSe();
	virtual ~CSe();

	int m_SE;
};


/*
//����Ȋ�����
//CResourceManager.Add( new CImage("C:/m_bullet.jpg"), "bullet001", 1);

CResource�N���X���p������
CResourceManager�ɂԂ�����


*/
