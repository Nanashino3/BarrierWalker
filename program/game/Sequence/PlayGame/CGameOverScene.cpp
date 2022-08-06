//****************************************************************************
// �t�@�C�����FCGameOverScene(�Q�[���I�[�o�[���)
// ��@���@���F2022/08/05
#include "CGameOverScene.h"

#include "CLoadingScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../Utility/CUtility.h"
#include "../../ShareInfo/CDocGameInfo.h"
#include "../../Sound/CSoundManager.h"

namespace Sequence
{
namespace PlayGame
{
CGameOverScene::CGameOverScene()
: AMenuScene("resource/sceneInfo/GameOverSceneInfo.csv")
, m_selectMenu(-1)
{
	m_soundManager = Sound::CSoundManager::GetInstance();
}

CGameOverScene::~CGameOverScene()
{}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F��ʍX�V
// ���@���F��1����	�ꎟ�Ǘ���
//		   ��2����	�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e���ʂŕK�v�ȏ������s��
//****************************************************************************
IScene* CGameOverScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;
	// �Q�[����ʕ`��
	controller.GetGameInstance()->Draw();
	DrawStringEx(m_sceneTextPosX, m_sceneTextPosY, GetColor(255, 255, 0), m_sceneTitle.c_str());

	// SE�I���܂Ń��j���[��ʂ͏o���Ȃ�
	if(m_soundManager->IsPlaySE(SE_ID_GAME_OVER)){ return nextScene; }

	// ���j���[�I��
	MenuSelect(m_selectMenu);
	if(m_selectMenu < 0){ return nextScene; }
	
	// �I���������j���[���̏���
	if(!Utility::FadeOut()){ return nextScene; }
	if(m_selectMenu == 0){
		nextScene = new CLoadingScene();
	}else if(m_selectMenu == 1){
		controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
	}else{
		tnl::DebugTrace("CGameOverScene->Error No Text\n");
	}

	return nextScene;
}

} // namespace PlayGame

} // namespace Sequence