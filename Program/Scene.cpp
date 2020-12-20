
#include "Scene.h"

//---------------------------------------------------------------------------------
//	CSceneManager
//---------------------------------------------------------------------------------
CSceneManager::CSceneManager(){
	//�V�[���N���X�Ɏ��g�̃|�C���^��n���Ă���
	CScene::SetSceneManager(this);
}
CSceneManager::~CSceneManager(){
	
}

void CSceneManager::Main(CInputAllStatus *input){
	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 );

	if (m_scene.size() <= 0) return;

	//int idx = m_scene.getNum()-1;
	int idx = (int)m_scene.size() - 1;
	CScene* sceneP = (CScene*)(m_scene[idx]);

	sceneP->Main(input);

	//�t�F�[�h�C�����I���܂Ńt�F�[�h�C��
	if(sceneP->m_inStatus != ENDFEED){
		sceneP->FeedIn();
	}
	//���̃V�[�����ݒ肳��Ă���A�t�F�[�h�A�E�g���I����Ă��Ȃ����
	if(	sceneP->m_outFlg == true &&
		sceneP->m_outStatus != ENDFEED){
		sceneP->FeedOut();
	}
	//�V�[����ς��Ă悯��΃V�[����ς���
	sceneP->ChangeScene();

	//�폜
	if(sceneP->m_backScene == true){
		//�t�F�[�h���I���܂ł͍폜���Ȃ�
		if(	sceneP->m_outFlg == true && sceneP->m_outStatus == ENDFEED){
			//m_scene.removeData(idx);
			m_scene.erase(m_scene.begin() + idx);
		}
	}
}

void CSceneManager::AddScene(CScene* Scene){
	//m_scene.Add(Scene);
	m_scene.push_back(Scene);
}



//---------------------------------------------------------------------------------
//	CScene
//---------------------------------------------------------------------------------
CResourceManager* CScene::m_resManager = nullptr;
CSceneManager* CScene::m_sceneManager = nullptr;
void CScene::SetResource(CResourceManager *ResManager){
	m_resManager = ResManager;
}
void CScene::SetSceneManager(CSceneManager *SceneManager){
	m_sceneManager = SceneManager;
}

CScene::CScene(int InTime){
	Init();
}

CScene::~CScene(){
}

void CScene::Init(){
	m_inTime = m_outTime = 0;
	m_inBlend = m_outBlend = 0.0;
	m_inNowTime = m_outNowTime = 0;	
	m_inStatus = m_outStatus = PREFEED;
	m_nextScene = nullptr;
	m_backScene = false;
	m_outFlg = false;
	m_count = 0;
}
void CScene::Set(int InTime){

	m_inTime = InTime;
	if(m_inTime > 0){

	} else{
		m_inStatus = ENDFEED;
	}

	m_outTime = 0;
	m_outStatus = PREFEED;

	m_inNowTime = m_outNowTime = 0;	

}

void CScene::Main(CInputAllStatus *input){
	//����
}

void CScene::FeedIn(){
	//�t�F�[�h�C������
	if(m_inNowTime < m_inTime){
		m_inNowTime++;

		m_inStatus = NOWFEED;
		
		//�t�F�[�h�̎���MAX��BLEND�}�b�N�X
		m_inBlend = 1.0 - (double)m_inNowTime/(double)m_inTime;

		int alpha = (int)((double)255 * m_inBlend);
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , alpha );

		CDxFunc::DrawBox2(0,0,800,600, 0x000000);
	}
	if(m_inNowTime >= m_inTime){
		m_inStatus = ENDFEED;
	}
}

void CScene::FeedOut(){
	//�t�F�[�h�A�E�g����
	if(m_outNowTime < m_outTime){
		m_outNowTime++;

		m_outStatus = NOWFEED;

		//�t�F�[�h�̎���MAX��BLEND�}�b�N�X
		m_outBlend = (double)m_outNowTime/(double)m_outTime;

		int alpha = (int)((double)255 * m_outBlend);
		SetDrawBlendMode( DX_BLENDMODE_ALPHA , alpha );

		CDxFunc::DrawBox2(0,0,800,600, 0x000000);
	}
	if(m_outNowTime >= m_outTime){
		m_outStatus = ENDFEED;
	}
}
inline bool CScene::NowFeed() const {
	if(m_inStatus == NOWFEED){
		return true;
	}
	if(m_outStatus == NOWFEED){
		return true;
	}
	return false;
}

void CScene::SetFeedOut(int OutTime){
	m_inStatus = ENDFEED;
	m_inNowTime = m_inTime+1;

	m_outFlg = true;
	m_outTime = OutTime;
	m_outNowTime = 0;
	m_outStatus = PREFEED;
}

inline void CScene::SetNextScene(CScene* NextScene){
	//�ǂ�����ݒ肳��Ă��Ȃ��ꍇ�ɂ̂݁A�ݒ肷��
	if(m_nextScene == nullptr && m_backScene == false){
		m_nextScene = NextScene;
	}
}
inline void CScene::SetBackScene(){
	//�ǂ�����ݒ肳��Ă��Ȃ��ꍇ�ɂ̂݁A�ݒ肷��
	if(m_nextScene == nullptr && m_backScene == false){
		m_backScene = true;
	}
}
inline void CScene::ChangeScene(){
	if(m_nextScene != nullptr &&	m_outStatus == ENDFEED){
		m_sceneManager->AddScene(m_nextScene);

		//������
		m_inNowTime = m_outNowTime = 0;	
		m_inStatus = m_outStatus = PREFEED;
		m_nextScene = nullptr;
		m_backScene = false;
		m_outFlg = false;
		m_count = 0;
	}
}

//----------------------------------------------------------
//	��
//----------------------------------------------------------

CSceneExMain::CSceneExMain(int InTime) : CScene(InTime){
	Set(InTime);

	CFont* wrkFontP;
	wrkFontP = (CFont*)m_resManager->GetResource(1000);
	font1 = wrkFontP->m_font;
	wrkFontP = (CFont*)m_resManager->GetResource(1001);
	font2 = wrkFontP->m_font;

	m_count = 0;
	//effectManeger1 = new CEffectManager(256);

	selectPos = 0;
}

CSceneExMain::~CSceneExMain(){
	//delete(effectManeger1);
}

void CSceneExMain::Main(CInputAllStatus *input){
	

	SetDrawBlendMode( DX_BLENDMODE_ALPHA , 255 );

	//�t�F�[�h���łȂ����
	if(!NowFeed()){

		//���͎Q��
		//if(input->m_btnStatus[INPUT_PAD_UP] == INPUT_PUSH){
		//	selectPos--;
		//	if(selectPos < 0){
		//		selectPos = 2;
		//	}
		//}
		//if(input->m_btnStatus[INPUT_PAD_DOWN] == INPUT_PUSH){
		//	selectPos++;
		//	if(selectPos > 2){
		//		selectPos = 0;
		//	}
		//}

		//if(input->m_btnStatus[INPUT_PAD_BTN3] == INPUT_PUSH){
		//	if(selectPos == 0){
		//		SetFeedOut(60);

		//		CSceneExSub1 *scene2 = new CSceneExSub1(120);
		//		SetNextScene(scene2);
		//	}
		//}
	}



	//if(m_count % 20 == 0){
	//	CPos pp(CFunc::RandF(0,800), CFunc::RandF(0,600));
	//	double ang = CFunc::RandF(0,360);
	//	CBaseEffect* eff = new CBaseEffect(ABS, pp, 2.0, ang, 0,0,0, 200);
	//	eff->SetSize(1.0, +0.05);
	//	eff->SetBlend(30, +1.0);
	//	eff->SetAnimeEndDelFlg(false);
	//	eff->SetRemoveCount(60);
	//	effectManeger1->Add(eff);
	//}
	//effectManeger1->Action();
	//effectManeger1->Draw();


	//�ǂ�ȏ�Ԃł��A�N�V�������鏈��
	char menuStr[3][80] = {
		"�����̃��j���[��(�t�F�[�h�Ȃ��@�e������)",
		"�����̃��j���[��(�t�F�[�h����@�Ó])",
		"�����̃��j���[��(�t�F�[�h����@�����Ó])"
	};
	for(int i=0;i<3;i++){
		int useFont = font1;
		if(i == selectPos){
			useFont = font2;
		}
		CDxFunc::DrawString(CPos(20,100 + i*60), menuStr[i], 0xffff00, useFont, 0x0000ff);
	}
	CDxFunc::DrawString(CPos(20,400), "�㉺�őI��ły�Ō���I", 0xff00ff, font1, 0xff0000);

	char feedStr[3][80] = {
		"�t�F�[�h�O","�t�F�[�h��","�t�F�[�h�I��"
	};
	CDxFunc::DrawString(CPos(20,500), feedStr[m_inStatus], 0xff00ff, font1, 0xff0000);
	CDxFunc::DrawString(CPos(20,540), feedStr[m_outStatus], 0xff00ff, font1, 0xff0000);

	m_count++;

	SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 255 );
}

//----------------------------------------------------------
//	��2
//----------------------------------------------------------

CSceneExSub1::CSceneExSub1(int InTime) : 
	CScene(InTime)
{
	//�V�[��,	�t�F�[�h�C������60, �t�F�[�h�A�E�g����60, 
	//			�t�F�[�h�C���Z�W0.0, �t�F�[�h�A�E�g�Z�W0.0
	Set(InTime);

	//bulletManeger1 = new CBulletManager(256);

	CFont* wrkFontP;
	wrkFontP = (CFont*)m_resManager->GetResource(1000);
	m_font = wrkFontP->m_font;
}

CSceneExSub1::~CSceneExSub1(){
	//delete(bulletManeger1);
}

void CSceneExSub1::Main(CInputAllStatus *input){

	//�t�F�[�h���łȂ����
	if(!NowFeed()){

		//���͎Q��
		if(input->m_btnStatus[INPUT_PAD_BTN2] == INPUT_PUSH){
			SetFeedOut(120);
			SetBackScene();
		}
	}



	//CPos ppp(CFunc::RandF(0,360),CFunc::RandF(0,360));
	//CCustomBullet *bu = new CCustomBullet(ABS, ppp, 4, m_count, 0,0,0, 1);
	//bulletManeger1->Add(bu);
	////�ǂ�ȏ�Ԃł��A�N�V�������鏈��
	//bulletManeger1->Action();
	//bulletManeger1->Draw();

	//SetDrawBlendMode( DX_BLENDMODE_ALPHA , 128 ) ;
	//CPos center = CPos(400,400);
	//CPntLine triPos;
	//int dist = 100;
	//int kaku = 6;
	//for(int i=0;i<kaku;i++){
	//	//���ڂ�������肽��
	//	//400,400�𒆐S��5�p�����ߒ��S�ƑO��̓_������

	//	double x = cos(360.0/kaku * i / CFunc::RAD) * dist*i + center.x;
	//	double y = sin(360.0/kaku * i / CFunc::RAD) * dist + center.y;
	//	triPos.Add(CPos(x,y));
	//}
	//CDxFunc::DrawCloseLoop(center, triPos, 0xff00ff);

	//SetDrawBlendMode( DX_BLENDMODE_NOBLEND , 128 ) ;


	//CDxFunc::DrawString(CPos(20,400), "X�Ŗ߂�I", 0xff00ff, m_font, 0xff0000);

	m_count++;
}
