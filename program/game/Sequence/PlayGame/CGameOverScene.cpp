#include "CGameOverScene.h"

#include "CLoadingScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../Utility/CMenu.h"
#include "../../Utility/CUtility.h"
#include "../../ShareInfo/CDocGameInfo.h"
#include "../../Sound/CSoundManager.h"

namespace{
const std::string GAME_RETRY = "���g���C";
const std::string GAME_TITLE = "�^�C�g���֖߂�";
const std::vector<std::string> message_text = {
	GAME_RETRY, GAME_TITLE
};

const int GAME_OVER_X = 450;
const int GAME_OVER_Y = 300;

const int FIRST_LABEL_X = 450;	// �������x�����W�FX��
const int FIRST_LABEL_Y = 384;	// �������x�����W�FY��

const int ALIGNMENT_X = 20;		// ���x���\���Ԋu�FX��
const int ALIGNMENT_Y = 20;		// ���x���\���Ԋu�FY��

std::tuple<bool, std::string> result;
}

namespace Sequence
{
namespace PlayGame
{
CGameOverScene::CGameOverScene()
{
	m_soundManager = Sound::CSoundManager::GetInstance();
	result = std::make_tuple(false, "");
	Utility::SelectReset();
}

CGameOverScene::~CGameOverScene()
{}

IScene* CGameOverScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;
	// �Q�[����ʕ`��
	controller.GetGameInstance()->Draw();
	DrawStringEx(GAME_OVER_X, GAME_OVER_Y, GetColor(255, 255, 0), "GAME OVER");

	// SE�I���܂Ń��j���[��ʂ͏o���Ȃ�
	if(m_soundManager->IsPlaySE(SE_ID_GAME_OVER)){ return next_scene; }

	// ���j���[�`��
	Utility::MenuDraw(message_text,
					  FIRST_LABEL_X, FIRST_LABEL_Y,
					  ALIGNMENT_X, ALIGNMENT_Y,
					  GetColor(255, 255, 0), GetColor(255, 255, 0));

	// �I���������j���[���̏���
	if(!std::get<0>(result)){
		// ���j���[�I��
		result = Utility::MenuSelect(message_text);
	}else{
		if(!Utility::FadeOut()){ return next_scene; }
		
		std::string text = std::get<1>(result);
		if(text == GAME_RETRY){
			next_scene = new CLoadingScene();
		}else if(text == GAME_TITLE){
			controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
		}else{
			tnl::DebugTrace("CGameOverScene->Error No Text\n");
		}
	}

	return next_scene;
}

} // namespace PlayGame

} // namespace Sequence