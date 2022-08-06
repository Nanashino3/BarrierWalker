//****************************************************************************
// �t�@�C�����FCLoadingScene(���[�h���)
// ��@���@���F2022/08/05
#include "CLoadingScene.h"
#include "CSecondaryController.h"
#include "CPlayScene.h"

#include "../../CGameDirector.h"
#include "../../ShareInfo/CDocGameInfo.h"

namespace{
const int LOADING_LABEL_X = 450;
const int LOADING_LABEL_Y = 300;
const float CHANGE_SCENE_TIME = 2.0f; // ��ʐ؂�ւ�����(TODO�F�S�̂ŋ��ʂɂ��邩�͍l�����B�B)

const std::vector<unsigned int> access_keys = {
	ACCESS_KEY_COIN, ACCESS_KEY_DEFEAT
};

}

namespace Sequence
{
namespace PlayGame
{
CLoadingScene::CLoadingScene()
: m_init(true)
{}

CLoadingScene::~CLoadingScene()
{}

IScene* CLoadingScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	if(m_init){
		m_init = false;
		
		// �Q�[������������
		info.Initialize();

		// �^�C�g����ʂ���̑J�ڂȂ�e��K�v�ȏ��������s��
		if(info.GetPrevScene() == TITLE_SCENE){
			info.SetStageIndex(0);
			for(int i = 0; i < access_keys.size(); i++){
				info.SetScoreCalcSrcInfo(access_keys[i], 0);
			}
		}

		// �Ǘ���
		controller.StartLoading();
	}

	IScene* nextScene = this;
	DrawStringEx(LOADING_LABEL_X, LOADING_LABEL_Y, -1, "���[�f�B���O��");

	// TODO�F����͌Œ�b�҂�
	static float deltaTime = 0;
	deltaTime += info.GetDeltaTime();
	if(deltaTime >= CHANGE_SCENE_TIME){
		deltaTime = 0.0f;
		nextScene = new CPlayScene();
		info.SetPrevScene(LOADING_SCENE);
	}


	return nextScene;
}

} // namespace PlayGame

} // namespace Sequence