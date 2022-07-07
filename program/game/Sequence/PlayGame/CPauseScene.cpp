#include "CPauseScene.h"
#include "CPlayScene.h"
#include "CHelpScene.h"
#include "CSecondaryController.h"

#include "../../Utility/CMenu.h"
#include "../../Utility/CUtility.h"
#include "../../CGameDirector.h"
#include "../../ShareInfo/CDocGameInfo.h"
#include "../../Sound/CSoundManager.h"

namespace {
const std::string RETURN_GAME_TEXT = "�Q�[���֖߂�";
const std::string HELP_TEXT = "�w���v";
const std::string RETURN_TITLE_TEXT = "�^�C�g���֖߂�";
const std::vector<std::string> message_text = {
	RETURN_GAME_TEXT, HELP_TEXT, RETURN_TITLE_TEXT
};

const int PAUSE_LABEL_X = 450;
const int PAUSE_LABEL_Y = 300;

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
CPauseScene::CPauseScene()
{
	m_soundManager = Sound::CSoundManager::GetInstance();
	// ���j���[�I���֘A�̏�����
	result = std::make_tuple(false, "");
	Utility::SelectReset();
}

CPauseScene::~CPauseScene()
{}

IScene* CPauseScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;
	// �Q�[����ʕ`��
	controller.GetGameInstance()->Draw();

	DrawStringEx(PAUSE_LABEL_X, PAUSE_LABEL_Y, GetColor(255, 255, 0), "PAUSE");

	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_G)){
		m_soundManager->PlaySE(SE_ID_PAUSE_CLOSE);
		return new CPlayScene(false);
	}

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
		std::string message_text = std::get<1>(result);
		if(message_text == RETURN_GAME_TEXT){
			next_scene = new CPlayScene(false);
		}else if(message_text == HELP_TEXT){
			next_scene = new CHelpScene();
		}else if(message_text == RETURN_TITLE_TEXT){
			if(!Utility::FadeOut()){ return next_scene; }
			controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
			m_soundManager->AllStopSound();
		}else{
			tnl::DebugTrace("CPauseScene->Error No Text\n");
		}
		info.SetPrevScene(PAUSE_SCENE);
	}

	return next_scene;
}

} // namespace PlayGame

} // namespace Sequence