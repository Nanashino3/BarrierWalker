// 二次管理者
#include "CSecondaryController.h"

#include "IScene.h"
#include "CLoadingScene.h"
#include "CHelpScene.h"

#include "../CTitleScene.h"
#include "../../CGameDirector.h"

namespace Sequence
{
namespace PlayGame
{
CSecondaryController::CSecondaryController(unsigned int createScene)
: m_primarySceneId(PRIMARY_ID_NONE)
, m_currentScene(nullptr)
, m_gameDirector(nullptr)
{
	switch(createScene)
	{
	case SECONDARY_ID_LOADING:
		m_currentScene = new CLoadingScene();
		break;
	case SECONDARY_ID_HELP:
		m_currentScene = new CHelpScene();
		break;
	default:
		break;
	}
}

CSecondaryController::~CSecondaryController()
{
	delete m_gameDirector;
	delete m_currentScene;
}

Sequence::IScene* CSecondaryController::Update(const GrandController&, ShareInfo::CDocGameInfo& info)
{
	Sequence::PlayGame::IScene* nextScene = m_currentScene->Update(*this, info);
	if(nextScene != m_currentScene){
		delete m_currentScene;
		m_currentScene = nextScene;
	}

	// 状態管理(プライマリ画面)
	Sequence::IScene* primaryScene = this;
	switch(m_primarySceneId)
	{
	case PRIMARY_ID_TITLE:
		primaryScene = new CTitleScene();
		break;
	default:
		break;
	}
	m_primarySceneId = PRIMARY_ID_NONE;
	nextScene = nullptr;

	return primaryScene;
}

// ゲーム管理者作成
void CSecondaryController::StartLoading()
{
	if(m_gameDirector != nullptr){ delete m_gameDirector; }
	m_gameDirector = new CGameDirector();
	m_gameDirector->Initialize();
}

} // namespace PlayGame

} // namespace Sequence