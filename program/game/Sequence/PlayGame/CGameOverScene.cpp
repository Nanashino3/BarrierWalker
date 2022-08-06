//****************************************************************************
// ファイル名：CGameOverScene(ゲームオーバー画面)
// 作　成　日：2022/08/05
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
// 関数名：Update
// 概　要：画面更新
// 引　数：第1引数	一次管理者
//		   第2引数	ゲーム情報
// 戻り値：なし
// 詳　細：各種画面で必要な処理を行う
//****************************************************************************
IScene* CGameOverScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;
	// ゲーム画面描画
	controller.GetGameInstance()->Draw();
	DrawStringEx(m_sceneTextPosX, m_sceneTextPosY, GetColor(255, 255, 0), m_sceneTitle.c_str());

	// SE終了までメニュー画面は出さない
	if(m_soundManager->IsPlaySE(SE_ID_GAME_OVER)){ return nextScene; }

	// メニュー選択
	MenuSelect(m_selectMenu);
	if(m_selectMenu < 0){ return nextScene; }
	
	// 選択したメニュー毎の処理
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