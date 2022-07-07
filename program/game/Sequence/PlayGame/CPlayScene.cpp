// プレイ画面
#include "CPlayScene.h"
#include "CClearScene.h"
#include "CGameOverScene.h"
#include "CPauseScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../Utility/CUtility.h"
#include "../../ShareInfo/CDocGameInfo.h"
#include "../../Sound/CSoundManager.h"

namespace{
const unsigned int g_sounds[] = {
	BGM_ID_STAGE_1, BGM_ID_STAGE_2, BGM_ID_STAGE_3
};
}

namespace Sequence
{
namespace PlayGame
{
CPlayScene::CPlayScene(bool isFadeIn)
: m_isFadeIn(isFadeIn)
{
	m_soundManager = Sound::CSoundManager::GetInstance();
}

CPlayScene::~CPlayScene()
{}

IScene* CPlayScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;

	bool isGameOver = info.GetIsGameOver();
	bool isClear = info.GetIsClear();
	// ゲームオーバ処理
	if(isGameOver){
		next_scene = new CGameOverScene();
		m_soundManager->AllStopSound();
		m_soundManager->PlaySE(SE_ID_GAME_OVER);
	}
	// ステージクリア処理
	if(isClear){
		next_scene = new CClearScene();
		m_soundManager->AllStopSound();
		m_soundManager->PlaySE(SE_ID_GAME_CLEAR);
	}

	// 一時停止
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_G)){
		next_scene = new CPauseScene();
		m_soundManager->PlaySE(SE_ID_PAUSE_OPEN);
	}

	if(!isClear && !isGameOver){
		int index = info.GetStageIndex();
		if(!m_soundManager->IsPlayBGM(g_sounds[index])){
			m_soundManager->PlayBGM(g_sounds[index]);
		}
	}

	// ゲーム管理者取得
	CGameDirector* director = controller.GetGameInstance();
	if(m_isFadeIn){
		if(Utility::FadeIn()){ m_isFadeIn = false; }
	}else{
		// ゲームの更新
		director->Update();
	}

	// 描画
	director->Draw();

	return next_scene;
}

} // namespace PlayGame

} // namespace Sequence