//****************************************************************************
// ファイル名：CClearScene(クリア画面)
// 作　成　日：2022/08/05
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
// 関数名：Update
// 概　要：画面更新
// 引　数：第1引数	一次管理者
//		   第2引数	ゲーム情報
// 戻り値：なし
// 詳　細：各種画面で必要な処理を行う
//****************************************************************************
IScene* CClearScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;

	// ゲーム画面描画
	controller.GetGameInstance()->Draw();

	m_elapsed += info.GetDeltaTime();
	if(info.GetIsFinalStage()){
		DrawStringEx(GAME_CLEAR_X, GAME_CLEAR_Y, GetColor(255, 255, 0), "GAME CLEAR!!");
		// ゲームクリアなら初期ステージから開始
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