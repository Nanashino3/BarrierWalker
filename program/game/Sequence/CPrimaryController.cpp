//****************************************************************************
// ファイル名：CPrimaryController(一次管理者)
// 作　成　日：2022/08/05

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

//****************************************************************************
// 関数名：Update
// 概　要：画面更新
// 引　数：第1引数	ゲーム情報
// 戻り値：なし
// 詳　細：インスタンスに応じて画面を更新する
//****************************************************************************
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