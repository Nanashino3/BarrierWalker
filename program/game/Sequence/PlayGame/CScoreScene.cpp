#include "CScoreScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../Utility/CMenu.h"
#include "../../Utility/CUtility.h"
#include "../../ShareInfo/CDocGameInfo.h"

namespace {
const std::string GAME_TITLE = "�^�C�g���֖߂�";
const std::vector<std::string> message_text = { GAME_TITLE };

const int LABE_X = 450;
const int LABE_Y = 200;
const int SCORE_TEXT_X = 450;
const int SCORE_TEXT_Y = 300;
const int MESSAGE_TEXT_X = 450;	// ���b�Z�[�W�e�L�X�g�FX��
const int MESSAGE_TEXT_Y = 384;	// ���b�Z�[�W�e�L�X�g�FY��
const int ALIGNMENT_X = 20;		// ���x���\���Ԋu�FX��
const int ALIGNMENT_Y = 20;		// ���x���\���Ԋu�FY��

const std::string GET_COIN_NUM = "�R�C���l�����@";
const std::string DEFEAT_TOTAL_NUM = "�G���j�����@�@";
const std::vector<std::string> label = {
	GET_COIN_NUM, DEFEAT_TOTAL_NUM
};
const float CHANGE_SCEME = 1.5f;

std::tuple<bool, std::string> result;
}

namespace Sequence
{
namespace PlayGame
{
CScoreScene::CScoreScene()
: m_elapsed(0.0f)
, m_key_count(0)
{
	result = std::make_tuple(false, "");
	Utility::SelectReset();
}
	
CScoreScene::~CScoreScene()
{}

IScene* CScoreScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;

	// �Q�[����ʕ`��
	controller.GetGameInstance()->Draw();

	DrawStringEx(LABE_X, LABE_Y, GetColor(255, 255, 0), "���Ȃ��̃X�R�A");

	// �X�R�A�\��
	m_elapsed += info.GetDeltaTime();
	if(m_elapsed > 1.0f){
		m_elapsed = 0.0f;
		if(m_key_count < MAX_ACCESS_KEY){ m_key_count++; }
	}
	for(int count = 0; count < m_key_count; count++){
		DrawStringEx(SCORE_TEXT_X, SCORE_TEXT_Y + count * 20, GetColor(255, 255, 0), "%s : %d", label[count].c_str(), info.GetScoreCalcSrcInfo(count));
	}

	// �S�ẴX�R�A��\�������ꍇ�I�����j���[���o��
	if(m_key_count == MAX_ACCESS_KEY){
		// ���j���[�`��
		Utility::MenuDraw(message_text,
						  MESSAGE_TEXT_X, MESSAGE_TEXT_Y,
						  ALIGNMENT_X, ALIGNMENT_Y,
						  GetColor(255, 255, 0), GetColor(255, 255, 0));

		// �I���������j���[���̏���
		if(!std::get<0>(result)){
			// ���j���[�I��
			result = Utility::MenuSelect(message_text);
		}else{
			std::string message_text = std::get<1>(result);
			if(message_text == GAME_TITLE){
				if(!Utility::FadeOut()){ return next_scene; }
				controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
			}else{
				tnl::DebugTrace("CScoreScene->Error No Text\n");
			}
		}
	}

	return next_scene;
}

} // namespace PlayGame

} // namespace Sequence