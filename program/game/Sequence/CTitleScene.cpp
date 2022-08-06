//****************************************************************************
// �t�@�C�����FCTitleScene(�^�C�g�����)
// ��@���@���F2022/05/25
#include "CTitleScene.h"
#include "PlayGame/CSecondaryController.h"

#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"
#include "../Utility/CImageManager.h"
#include "../Utility/CScreenEffect.h"


namespace Sequence
{
CTitleScene::CTitleScene()
: AMenuScene("resource/sceneInfo/TitleSceneInfo.csv")
, m_bgGfxHdl(0)
, m_titleGfxHdl(0)
, m_alphaValue(0)
, m_elapsed(0.0f)
, m_selectMenu(-1)
{
	// �w�i�摜�̓ǂݍ���
	m_bgGfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx("resource/pipo-bg003a.jpg");
	m_soundManager = Sound::CSoundManager::GetInstance();
	m_titleGfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx("resource/string/title1.png");
}

CTitleScene::~CTitleScene()
{
	Utility::CImageManager::GetInstance()->DeleteGraphEx(m_bgGfxHdl);
	Utility::CImageManager::GetInstance()->DeleteGraphEx(m_titleGfxHdl);
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F��ʍX�V
// ���@���F��1����	�ꎟ�Ǘ���
//		   ��2����	�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e���ʂŕK�v�ȏ������s��
//****************************************************************************
IScene* CTitleScene::Update(const CPrimaryController&, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;

	// �w�i�摜�\��
	DrawRotaGraph(512, 384, 1.6f, 0, m_bgGfxHdl, true);

	// BGM���Ȃ��Ă��Ȃ��Ƃ������Đ�
	if (!m_soundManager->IsPlayBGM(BGM_ID_TITLE)) { m_soundManager->PlayBGM(BGM_ID_TITLE); }
	// �^�C�g����ʂ̃A�j���[�V����
	if(PriTitleAnimation(info)){ return nextScene; }
	
	// ���j���[�I������
	MenuSelect(m_selectMenu);
	if(m_selectMenu < 0){ return nextScene; }

	// �I���������j���[���̏���
	if(m_selectMenu == 0){
		if(!Utility::CScreenEffect::GetInstance()->FadeOut()){ return nextScene; }
		nextScene = new PlayGame::CSecondaryController(PlayGame::CSecondaryController::SECONDARY_ID_LOADING);
		m_soundManager->StopBGM(BGM_ID_TITLE);
	}else if(m_selectMenu == 1){
		nextScene = new PlayGame::CSecondaryController(PlayGame::CSecondaryController::SECONDARY_ID_HELP);
	}else if(m_selectMenu == 2){
		return nullptr;
	}else{
		tnl::DebugTrace("CTitleScene->Error No Text\n");
	}
	info.SetPrevScene(TITLE_SCENE);

	return nextScene;
}

//****************************************************************************
// �֐����FPriTitleAnimation
// �T�@�v�F�^�C�g����ʂ̃A�j���[�V����(Private)
// ���@���F��1����	�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�^�C�g����ʂ̃A�j���[�V��������
//****************************************************************************
bool CTitleScene::PriTitleAnimation(ShareInfo::CDocGameInfo& info)
{
	m_elapsed += info.GetDeltaTime();
	if(m_elapsed > 0.05f){
		m_elapsed = 0.0f;
		if(m_alphaValue != 255){
			m_alphaValue += 5;
		}else{
			m_alphaValue = 255;
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_alphaValue);
	DrawRotaGraph(512, 300, 1.0f, 0, m_titleGfxHdl, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	return (m_alphaValue != 255) ? true: false;
}

} // namespace Sequence