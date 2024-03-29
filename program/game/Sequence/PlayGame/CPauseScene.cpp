#include "CPauseScene.h"
#include "CPlayScene.h"
#include "CHelpScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../ShareInfo/CDocGameInfo.h"
#include "../../Sound/CSoundManager.h"
#include "../../Utility/CScreenEffect.h"

namespace Sequence
{
namespace PlayGame
{
CPauseScene::CPauseScene()
: AMenuScene("resource/sceneInfo/PauseSceneInfo.csv")
, m_selectMenu(-1)
{
	m_soundManager = Sound::CSoundManager::GetInstance();
}

CPauseScene::~CPauseScene()
{}

IScene* CPauseScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* nextScene = this;
	// ゲーム画面描画
	controller.GetGameInstance()->Draw();

	DrawStringEx(m_sceneTextPosX, m_sceneTextPosY, GetColor(255, 255, 0), m_sceneTitle.c_str());

	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_G)){
		m_soundManager->PlaySE(SE_ID_PAUSE_CLOSE);
		return new CPlayScene(false);
	}

	MenuSelect(m_selectMenu);
	if(m_selectMenu < 0){ return nextScene; }

	// 選択したメニュー毎の処理
	if(m_selectMenu == 0){
		nextScene = new CPlayScene(false);
	}else if(m_selectMenu == 1){
		nextScene = new CHelpScene();
	}else if(m_selectMenu == 2){
		if(!Utility::CScreenEffect::GetInstance()->FadeOut()){ return nextScene; }
		controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
		m_soundManager->AllStopSound();
	}else{
		tnl::DebugTrace("CPauseScene->Error No Text\n");
	}
	info.SetPrevScene(PAUSE_SCENE);

	return nextScene;
}

} // namespace PlayGame

} // namespace Sequence