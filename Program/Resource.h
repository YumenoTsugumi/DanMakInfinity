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

const int RESOURCENAMEMAX = 40; // リソースの名前

//すべてのリソースが持つ管理情報
//重複した場合早い物を引っ張ってくる
class CResourceInfo{
public:
	//リソースが所属しているidx
	int m_index;

	//管理名	こいつを指定して引っ張ってくる
	char m_name[RESOURCENAMEMAX];
	//管理番号	こいつを指定して引っ張ってくる
	int m_number;

	//読み込み終えたか
	//false:未読み込み
	bool m_loadFlg;
};

//---------------------------------------------------------------------------------
// リソースのベースとなるクラス
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
// イメージ２枚で１つ
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
//	n分割出来るイメージ
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
// アニメーションのイメージ
//---------------------------------------------------------------------------------
class CImageAnime : public CImages{
public:
	CImageAnime(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY, int animeSpeed);
	virtual ~CImageAnime();

	int m_animeSpeed;
};
//---------------------------------------------------------------------------------
// 弾のイメージ
//---------------------------------------------------------------------------------
class CBulletImage : public CImageAnime{
public:
	CBulletImage(const char* filename, int numAll, int numX, int numY, int sizeX, int sizeY, int animeSpeed, double RotaSpeed);
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


//---------------------------------------------------------------------------------
//	リソースのマネージャ
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
//こんな感じに
//CResourceManager.Add( new CImage("C:/m_bullet.jpg"), "bullet001", 1);

CResourceクラスを継承して
CResourceManagerにぶっこむ


*/
