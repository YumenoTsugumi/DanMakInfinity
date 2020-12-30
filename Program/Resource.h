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
//	リソースのマネージャ
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
// リソースのベースとなるクラス
//---------------------------------------------------------------------------------
class CResource{
public:
	//管理名	こいつを指定して引っ張ってくる
	std::string m_name;
	int m_index;

	//読み込み終えたか
	//false:未読み込み
	bool m_loadFlg;

	CResource(const char* filename);
	virtual ~CResource();
};
//---------------------------------------------------------------------------------
// イメージ１枚
//---------------------------------------------------------------------------------
class CImage : public CResource {
public:
	CImage(const char* filename);
	virtual ~CImage();

	int m_iamge;
	int m_sizeX, m_sizeY;
};

//---------------------------------------------------------------------------------
//	n分割出来るイメージ
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
// アニメーションのイメージ
//---------------------------------------------------------------------------------
class CImageAnime : public CImages{
public:
	CImageAnime(const char* filename, int numAll, int numX, int numY, int m_sizeX, int m_sizeY, int m_animeSpeed);
	virtual ~CImageAnime();

	int m_animeSpeed;
};
//---------------------------------------------------------------------------------
// 弾のイメージ
//---------------------------------------------------------------------------------
class CBulletImage : public CImageAnime{
public:
	CBulletImage(const char* filename, int numAll, int numX, int numY, int m_sizeX, int m_sizeY, int m_animeSpeed, double RotaSpeed);
	virtual ~CBulletImage();

	double m_rotaSpeed;
};
//---------------------------------------------------------------------------------
//　フォント
//---------------------------------------------------------------------------------
class CFont : public CResource{
public:
	CFont(const char* fontPath, int m_size = -1, int m_thick = -1, int fontType = DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	
	virtual ~CFont();

	int m_font;
};
//---------------------------------------------------------------------------------
//	曲
//---------------------------------------------------------------------------------
class CBgm : public CResource{
public:
	CBgm();
	virtual ~CBgm();

	int m_BGM;
};
//---------------------------------------------------------------------------------
//	効果音
//---------------------------------------------------------------------------------
class CSe : public CResource{
public:
	CSe();
	virtual ~CSe();

	int m_SE;
};


/*
//こんな感じに
//CResourceManager.Add( new CImage("C:/m_bullet.jpg"), "bullet001", 1);

CResourceクラスを継承して
CResourceManagerにぶっこむ


*/
