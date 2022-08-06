//****************************************************************************
// ファイル名：CGameDirector(ゲームディレクター)
// 作　成　日：2022/08/06
#include "CGameDirector.h"

#include "Camera/CCamera2D.h"
#include "Function/CFunctionController.h"
#include "GameObject/CPlayerCharacter.h"
#include "GameObject/CBackGround.h"
#include "GameObject/CFixedEnemy.h"
#include "GameObject/CFixedFloor.h"
#include "GameObject/CItemManager.h"
#include "GameObject/CMoveObjectManager.h"
#include "ShareInfo/CDocGameInfo.h"
#include "Ui/CUiController.h"

CGameDirector::CGameDirector()
: m_camera(nullptr)
, m_functionController(nullptr)
, m_docGameInfo(nullptr)
{
	m_camera = new Camera::CCamera2D(SCREEN_WIDTH, SCREEN_HEIGHT);

	// ゲーム情報設定
	m_docGameInfo = ShareInfo::CDocGameInfo::GetInstance();
	m_docGameInfo->SetCamera(m_camera);
	m_docGameInfo->SetScreenWidth(SCREEN_WIDTH);
	m_docGameInfo->SetScreenHeight(SCREEN_HEIGHT);

	// インスタンス生成
	m_functionController = new Function::CFunctionController();
	m_uiController = new Ui::CUiController();
	m_playerObject = new GameObject::CPlayerCharacter;
	m_moveObjManager = new GameObject::CMoveObjectManager();
	GameObject::AFixedObject* fixedObject = nullptr;
	fixedObject = new GameObject::CBackGround;		// 背景
	m_fixedObjects.push_back(fixedObject);
	fixedObject = new GameObject::CFixedFloor;		// 固定ステージ
	m_fixedObjects.push_back(fixedObject);
	fixedObject = new GameObject::CFixedEnemy;		// 固定敵
	m_fixedObjects.push_back(fixedObject);
	fixedObject = new GameObject::CItemManager;		// アイテム
	m_fixedObjects.push_back(fixedObject);
}

CGameDirector::~CGameDirector()
{
	delete m_camera;
	delete m_functionController;;
	delete m_uiController;
	delete m_playerObject;
	delete m_moveObjManager;

	for(int i = 0; i < m_fixedObjects.size(); i++){
		delete m_fixedObjects[i];
	}
	m_fixedObjects.clear();
	m_fixedObjects.shrink_to_fit();
}

//****************************************************************************
// 関数名：Initialize
// 概　要：ゲーム初期化処理
// 引　数：なし
// 戻り値：なし
// 詳　細：プレイ画面に関わるオブジェクト・UI・機能の初期化を行う
//****************************************************************************
void CGameDirector::Initialize()
{
	tnl::DebugTrace("GameD->Initialize\n");
	m_camera->Initialize();
	m_playerObject->Initialize(*m_docGameInfo);
	m_moveObjManager->Initialize(*m_docGameInfo);
	for(int i = 0; i < m_fixedObjects.size(); i++){
		m_fixedObjects[i]->Initialize(*m_docGameInfo);
	}
	m_functionController->Initialize();
	m_uiController->Initialize(*m_docGameInfo);
}

//****************************************************************************
// 関数名：Update
// 概　要：ゲーム更新処理
// 引　数：なし
// 戻り値：なし
// 詳　細：プレイ画面に関わるオブジェクト・UI・機能の更新を行う
// 　　　　※ここで移動や当たり判定も行う
//****************************************************************************
void CGameDirector::Update()
{
	if(m_docGameInfo->GetIsClear() || m_docGameInfo->GetIsGameOver()){ return; }

	// 移動
	tnl::Vector3 prevPos(0, 0, 0), currentPos(0, 0, 0);
	m_moveObjManager->Move(*m_docGameInfo);
	m_playerObject->Move(currentPos, prevPos, *m_docGameInfo);

	// 衝突判定
	for(int s_cnt = 0; s_cnt < m_fixedObjects.size(); s_cnt++){
		m_fixedObjects[s_cnt]->Collision(currentPos, prevPos, *m_docGameInfo);
	}
	m_moveObjManager->Collision(currentPos, prevPos, *m_docGameInfo);

	// 更新
	m_playerObject->Update(*m_docGameInfo);
	m_functionController->Update(*m_docGameInfo);
	m_uiController->Update(*m_docGameInfo);
	for(int i = 0; i < m_fixedObjects.size(); i++){
		m_fixedObjects[i]->Update(*m_docGameInfo);
	}
	m_camera->Update(*m_docGameInfo);
}

//****************************************************************************
// 関数名：Draw
// 概　要：ゲーム描画処理
// 引　数：なし
// 戻り値：なし
// 詳　細：プレイ画面に関わるオブジェクト・UI・機能の描画を行う
// 　　　　※描画順も意識する必要がある
//****************************************************************************
void CGameDirector::Draw()
{
	for(int i = 0; i < m_fixedObjects.size(); i++){
		m_fixedObjects[i]->Draw(*m_docGameInfo);
	}
	m_moveObjManager->Draw(*m_docGameInfo);
	m_uiController->Draw(*m_docGameInfo);
	m_playerObject->Draw(*m_docGameInfo);
	m_functionController->Draw(*m_docGameInfo);
}