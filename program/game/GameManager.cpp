//****************************************************************************
// ファイル名：GameManager(ゲーム管理者)
// 作　成　日：2022/08/05
#include "GameManager.h"

#include "Sequence/CPrimaryController.h"
#include "ShareInfo/CDocGameInfo.h"
#include "Sound/CSoundManager.h"

GameManager* GameManager::m_instance = nullptr;
GameManager::GameManager()
{
	Sound::CSoundManager::CreateInstance();
	m_docGameInfo = ShareInfo::CDocGameInfo::GetInstance();
	m_sceneControl = new Sequence::CPrimaryController();
}

//****************************************************************************
// 関数名：GetInstance
// 概　要：インスタンス取得
// 引　数：なし
// 戻り値：なし
// 詳　細：ゲーム管理者のインスタンスを取得する
//****************************************************************************
GameManager* GameManager::GetInstance()
{
	if(m_instance == nullptr){
		m_instance = new GameManager();
	}

	return m_instance;
}

//****************************************************************************
// 関数名：DestroyInstance
// 概　要：インスタンスの破棄
// 引　数：なし
// 戻り値：なし
// 詳　細：ゲーム管理者のインスタンスを破棄する
//****************************************************************************
void GameManager::DestroyInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

//****************************************************************************
// 関数名：Update
// 概　要：ゲーム更新処理
// 引　数：なし
// 戻り値：なし
// 詳　細：ゲームの更新処理を行う
//****************************************************************************
bool GameManager::Update(float deltaTime)
{
	m_docGameInfo->SetDeltaTime(deltaTime);
	// シーン遷移をコントロール
	if(!m_sceneControl->Update(*m_docGameInfo)){
		delete m_sceneControl;
		ShareInfo::CDocGameInfo::DestroyInstance();
		Sound::CSoundManager::DestroyInstance();

		return false;
	}

	return true;
}