#pragma once

#include "Func.h"
#include "DxLib.h"

class CResourceInfo;
class CImage;
class CImages;
class CImageAnime;
class CBulletImage;
class CFont;
class CBgm;
class CSe;
class CResourceManager;

const int RESOURCENAMEMAX = 40; // ���\�[�X�̖��O

//���ׂẴ��\�[�X�����Ǘ����
//�d�������ꍇ�����������������Ă���
class CResourceInfo{
public:
	//���\�[�X���������Ă���idx
	int m_index;

	//�Ǘ���	�������w�肵�Ĉ��������Ă���
	char m_name[RESOURCENAMEMAX];
	//�Ǘ��ԍ�	�������w�肵�Ĉ��������Ă���
	int m_number;

	//�ǂݍ��ݏI������
	//false:���ǂݍ���
	bool m_loadFlg;
};

//---------------------------------------------------------------------------------
// ���\�[�X�̃x�[�X�ƂȂ�N���X
//---------------------------------------------------------------------------------
class CResource{
public:
	CResourceInfo info;
	void SetIndex(int Idx);
	void SetInfo(const char* name, int number);
	
	CResource();
	virtual ~CResource();
	void Init();

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
// �C���[�W�Q���łP��
//---------------------------------------------------------------------------------
class CImageSet : public CResource{
public:
	CImageSet(int Num, const char Filename[][RESOURCENAMEMAX]);
	virtual ~CImageSet();
	CImage* GetImage(int Idx);

	int m_num;
	CImage** m_image;
};

//---------------------------------------------------------------------------------
//	n�����o����C���[�W
//---------------------------------------------------------------------------------
class CImages : public CResource{
public:
	CImages(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY);
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
	CImageAnime(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY, int animeSpeed);
	virtual ~CImageAnime();

	int m_animeSpeed;
};
//---------------------------------------------------------------------------------
// �e�̃C���[�W
//---------------------------------------------------------------------------------
class CBulletImage : public CImageAnime{
public:
	CBulletImage(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY, int animeSpeed, double RotaSpeed);
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


//---------------------------------------------------------------------------------
//	���\�[�X�̃}�l�[�W��
//---------------------------------------------------------------------------------
class CResourceManager{
public:
	std::vector<CResource*> m_resource;
	int Add(CResource* Resource, const char* Name, int Number);

	CResource* GetResource(int number);
	CResource* GetResource(const char* Name);

	CResourceManager();
	~CResourceManager();
};


/*
//����Ȋ�����
//CResourceManager.Add( new CImage("C:/m_bullet.jpg"), "bullet001", 1);

CResource�N���X���p������
CResourceManager�ɂԂ�����


*/
