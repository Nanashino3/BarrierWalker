//****************************************************************************
// ファイル名：CUiController(UI管理者)
// 作　成　日：2022/08/06
#include "CUiController.h"

#include "CUiBarrier.h"
#include "CUiTrilemma.h"
#include "CUiTimeLimit.h"
#include "CUiCoinNumber.h"

namespace Ui
{
CUiController::CUiController()
{
	AUi* ui = nullptr;
	ui = new CUiBarrier();		// バリアUI
	m_uiList.push_back(ui);
	ui = new CUiTrilemma();		// 三すくみUI
	m_uiList.push_back(ui);
	ui = new CUiTimeLimit();	// 制限時間UI
	m_uiList.push_back(ui);
	ui = new CUiCoinNumber();	// コイン数UI
	m_uiList.push_back(ui);
}

CUiController::~CUiController()
{
	for(int i = 0; i < m_uiList.size(); i++){
		delete m_uiList[i];
	}
	m_uiList.clear();
	m_uiList.shrink_to_fit();
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各UIクラスに初期化を依頼
//****************************************************************************
void CUiController::Initialize(ShareInfo::CDocGameInfo& info)
{
	for(int i = 0; i < m_uiList.size(); i++){
		m_uiList[i]->Initialize(info);
	}
}

//****************************************************************************
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各UIクラスに更新を依頼
//****************************************************************************
void CUiController::Update(ShareInfo::CDocGameInfo& info)
{
	for(int i = 0; i < m_uiList.size(); i++){
		m_uiList[i]->Update(info);
	}
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各UIクラスに描画を依頼
//****************************************************************************
void CUiController::Draw(ShareInfo::CDocGameInfo& info)
{
	for(int i = 0; i < m_uiList.size(); i++){
		m_uiList[i]->Draw(info);
	}
}

} // namespace UserInterface