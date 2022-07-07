//*************************************************************
// UI管理者クラス
//*************************************************************
#include "CUiController.h"

#include "CUiBarrier.h"
#include "CUiTrilemma.h"
#include "CUiTimeLimit.h"
#include "CUiCoinNumber.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace{ std::vector<Ui::AUi*> g_ui; }
namespace Ui
{
CUiController::CUiController()
{
	AUi* ui = nullptr;
	ui = new CUiBarrier();		// バリアUI
	g_ui.push_back(ui);
	ui = new CUiTrilemma();		// 三すくみUI
	g_ui.push_back(ui);
	ui = new CUiTimeLimit();	// 制限時間UI
	g_ui.push_back(ui);
	ui = new CUiCoinNumber();	// コイン数UI
	g_ui.push_back(ui);
}

CUiController::~CUiController()
{
	for(int i = 0; i < g_ui.size(); i++){
		delete g_ui[i];
	}
	g_ui.clear();
	g_ui.shrink_to_fit();
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
	for(int i = 0; i < g_ui.size(); i++){
		g_ui[i]->Initialize(info);
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
	for(int i = 0; i < g_ui.size(); i++){
		g_ui[i]->Update(info);
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
	for(int i = 0; i < g_ui.size(); i++){
		g_ui[i]->Draw(info);
	}
}

} // namespace UserInterface