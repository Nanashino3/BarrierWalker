//****************************************************************************
// �t�@�C�����FCClearScene(�N���A���)
// ��@���@���F2022/08/05
#include "CClearScene.h"
#include "CLoadingScene.h"
#include "CScoreScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../ShareInfo/CDocGameInfo.h"
#include "../../Utility/CScreenEffect.h"

namespace {
const int GAME_CLEAR_X = 450;
const int GAME_CLEAR_Y = 300;
const float CHANGE_SCEME = 5.0f;
}

namespace Sequence
{
namespace PlayGame
{
CClearScene::CClearScene()
: m_elapsed(0)
{}

CClearScene::~CClearScene()
{}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F��ʍX�V
// ���@���F��1����	�ꎟ�Ǘ���
//		   ��2����	�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e���ʂŕK�v�ȏ������s��
//****************************************************************************
IScene* CClearScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;

	// �Q�[����ʕ`��
	controller.GetGameInstance()->Draw();

	m_elapsed += info.GetDeltaTime();
	if(info.GetIsFinalStage()){
		DrawStringEx(GAME_CLEAR_X, GAME_CLEAR_Y, GetColor(255, 255, 0), "GAME CLEAR!!");
		// �Q�[���N���A�Ȃ珉���X�e�[�W����J�n
		info.SetStageIndex(0);
		if(m_elapsed > CHANGE_SCEME){
			m_elapsed = 0.0f;
			nextScene = new CScoreScene();
		}
	}else{
		DrawStringEx(GAME_CLEAR_X, GAME_CLEAR_Y, GetColor(255, 255, 0), "STAGE CLEAR!!");
		if(m_elapsed > CHANGE_SCEME){
			if(!Utility::CScreenEffect::GetInstance()->FadeOut()){ return nextScene; }
			m_elapsed = 0.0f;
			nextScene = new CLoadingScene();
		}
	}

	return nextScene;
}

} // namespace PlayGame

} // namespace Sequence