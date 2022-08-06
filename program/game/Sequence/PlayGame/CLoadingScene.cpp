//****************************************************************************
// ファイル名：CLoadingScene(ロード画面)
// 作　成　日：2022/08/05
#include "CLoadingScene.h"
#include "CSecondaryController.h"
#include "CPlayScene.h"

#include "../../CGameDirector.h"
#include "../../ShareInfo/CDocGameInfo.h"

namespace{
const int LOADING_LABEL_X = 450;
const int LOADING_LABEL_Y = 300;
const float CHANGE_SCENE_TIME = 2.0f; // 画面切り替え時間(TODO：全体で共通にするかは考え中。。)

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
		
		// ゲーム情報を初期化
		info.Initialize();

		// タイトル画面からの遷移なら各種必要な初期化を行う
		if(info.GetPrevScene() == TITLE_SCENE){
			info.SetStageIndex(0);
			for(int i = 0; i < access_keys.size(); i++){
				info.SetScoreCalcSrcInfo(access_keys[i], 0);
			}
		}

		// 管理者
		controller.StartLoading();
	}

	IScene* nextScene = this;
	DrawStringEx(LOADING_LABEL_X, LOADING_LABEL_Y, -1, "ローディング中");

	// TODO：今回は固定秒待つ
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