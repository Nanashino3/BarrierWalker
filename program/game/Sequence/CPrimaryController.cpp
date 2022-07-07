// 一次管理者
#include "CPrimaryController.h"
#include "CTitleScene.h"

#include "../ShareInfo/CDocGameInfo.h"

namespace Sequence
{
CPrimaryController::CPrimaryController()
: m_currentScene(nullptr)
{
	m_currentScene = new CTitleScene;
}

CPrimaryController::~CPrimaryController()
{
	delete m_currentScene;
}

// 画面更新
bool CPrimaryController::Update(ShareInfo::CDocGameInfo& info)
{
	// 各画面から次の遷移先を返してもらう
	IScene* nextScene = m_currentScene->Update(*this, info);
	if(nextScene == nullptr){ return false; }

	// 現在と次のシーンが異なっていればインスタンスを入れ替える
	if(nextScene != m_currentScene){
		delete m_currentScene;
		m_currentScene = nextScene;
	}

	nextScene = nullptr;
	return true;
}

} // namespace Sequence