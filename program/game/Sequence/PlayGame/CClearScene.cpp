#include "CClearScene.h"
#include "CLoadingScene.h"
#include "CScoreScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../Utility/CUtility.h"
#include "../../ShareInfo/CDocGameInfo.h"

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

IScene* CClearScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;

	// ゲーム画面描画
	controller.GetGameInstance()->Draw();

	m_elapsed += info.GetDeltaTime();
	if(info.GetIsFinalStage()){
		DrawStringEx(GAME_CLEAR_X, GAME_CLEAR_Y, GetColor(255, 255, 0), "GAME CLEAR!!");
		// ゲームクリアなら初期ステージから開始
		info.SetStageIndex(0);
		if(m_elapsed > CHANGE_SCEME){
			m_elapsed = 0.0f;
			next_scene = new CScoreScene();
		}
	}else{
		DrawStringEx(GAME_CLEAR_X, GAME_CLEAR_Y, GetColor(255, 255, 0), "STAGE CLEAR!!");
		if(m_elapsed > CHANGE_SCEME){
			if(!Utility::FadeOut()){ return next_scene; }
			m_elapsed = 0.0f;
			next_scene = new CLoadingScene();
		}
	}

	return next_scene;
}

} // namespace PlayGame

} // namespace Sequence