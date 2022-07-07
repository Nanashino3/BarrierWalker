#include "CHelpScene.h"
#include "CPauseScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../Utility/CMenu.h"
#include "../../ShareInfo/CDocGameInfo.h"

namespace {
// TODO�F���b�Z�[�W�n��CSV�ɂ�����
const std::string COMMAND_MOVE_R		= "D�L�|";
const std::string COMMAND_MOVE_L		= "A�L�|";
const std::string COMMAND_JUMP			= "SPACE";
const std::string COMMAND_DASH			= "L SHIFT";
const std::string COMMAND_BARRIER		= "B�L�|";
const std::string COMMAND_CHANGE_UP		= "W�L�|";
const std::string COMMAND_CHANGE_DOWN	= "S�L�|";
const std::string COMMAND_PAUSE			= "G�L�|";
const std::vector<std::string> operate_text = {
	COMMAND_MOVE_R, COMMAND_MOVE_L, COMMAND_JUMP, COMMAND_DASH,
	COMMAND_BARRIER, COMMAND_CHANGE_UP, COMMAND_CHANGE_DOWN, COMMAND_PAUSE
};

const std::string OPE_NAME_MOVE_R		= "�ړ��F�E����";
const std::string OPE_NAME_MOVE_L		= "�ړ��F������";
const std::string OPE_NAME_JUMP			= "�W�����v";
const std::string OPE_NAME_DASH			= "�_�b�V��";
const std::string OPE_NAME_BARRIER		= "�o���A�W�J";
const std::string OPE_NAME_CHANGE_UP	= "�o���A�ؑցF�����";
const std::string OPE_NAME_CHANGE_DOWN	= "�o���A�ؑցF������";
const std::string OPE_NAME_PAUSE		= "�|�[�Y/�|�[�Y����";
const std::vector<std::string> action_text = {
	OPE_NAME_MOVE_R, OPE_NAME_MOVE_L, OPE_NAME_JUMP, OPE_NAME_DASH,
	OPE_NAME_BARRIER, OPE_NAME_CHANGE_UP, OPE_NAME_CHANGE_DOWN, OPE_NAME_PAUSE
};

const int FIRST_LABEL_X = 400;	// ���x�����W�FX��
const int FIRST_LABEL_Y = 250;	// ���x�����W�FY��

const int TITLE_LABEL_X = 450;	// ���x�����W�FX��
const int TITLE_LABEL_Y = 450;	// ���x�����W�FY��

const int ALIGNMENT_X = 20;		// ���x���\���Ԋu�FX��
const int ALIGNMENT_Y = 20;		// ���x���\���Ԋu�FY��

const std::string RETURN_TEXT = "�߂�";
std::vector<std::string> message_text = { RETURN_TEXT };

std::tuple<bool, std::string> result;
}

namespace Sequence
{
namespace PlayGame
{
CHelpScene::CHelpScene()
{
	result = std::make_tuple(false, "");
	Utility::SelectReset();
}

CHelpScene::~CHelpScene()
{}

IScene* CHelpScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;

	// �Q�[����ʂŃw���v��ʂ�\��
	if(info.GetPrevScene() == PAUSE_SCENE){
		controller.GetGameInstance()->Draw();
	}

	// �R�}���h�e�L�X�g�`��
	for(int i = 0; i < operate_text.size(); i++){
		DrawStringEx(FIRST_LABEL_X, FIRST_LABEL_Y + i * ALIGNMENT_Y, GetColor(255, 255, 0), "%s", operate_text[i].c_str());
	}

	// ��؂蕶���`��
	for(int i = 0; i < operate_text.size(); i++){
		DrawStringEx(FIRST_LABEL_X + 75, FIRST_LABEL_Y + i * ALIGNMENT_Y, GetColor(255, 255, 0), "%s", "�F");
	}

	// ����e�L�X�g�`��
	for(int i = 0; i < action_text.size(); i++){
		DrawStringEx(FIRST_LABEL_X + 100, FIRST_LABEL_Y + i * ALIGNMENT_Y, GetColor(255, 255, 0), "%s", action_text[i].c_str());
	}

	// ���j���[�I��
	Utility::MenuDraw(message_text,
					  TITLE_LABEL_X, TITLE_LABEL_Y,
					  ALIGNMENT_X, ALIGNMENT_Y,
					  GetColor(255, 255, 0), GetColor(255, 255, 0));

	// �I���������j���[���̏���
	if(!std::get<0>(result)){
		// ���j���[�I��
		result = Utility::MenuSelect(message_text);
	}else{
		std::string message_text = std::get<1>(result);
		if(message_text == RETURN_TEXT){
			if(info.GetPrevScene() == PAUSE_SCENE){
				// �|�[�Y��ʂɖ߂�
				next_scene = new CPauseScene();
			}else{
				// �^�C�g����ʂɖ߂�
				controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
			}
		}else{
			tnl::DebugTrace("CHelpScene->Error No Text\n");
		}
		info.SetPrevScene(HELP_SCENE);
	}

	return next_scene;
}

} // namespace PlayGame

} // namespace Sequence