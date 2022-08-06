//****************************************************************************
// ファイル名：CFunctionController(機能管理者)
// 作　成　日：2022/08/06
#include "CFunctionController.h"

#include "CLifePoint.h"
#include "CStageControl.h"
#include "CBarrier.h"

namespace Function
{
CFunctionController::CFunctionController()
{
	AFunction* function = nullptr;

	//************************
	// 常に使用する機能
	//************************
	function = new CLifePoint();		// HP
	m_functions.push_back(function);
	function = new CStageControl();	// ステージ管理
	m_functions.push_back(function);

	//************************
	// 機能の付替え可能
	//************************
	function = new CBarrier(FUNCTION_TYPE_BARRIER);	// バリア
	m_functions.push_back(function);
}

CFunctionController::~CFunctionController()
{
	for(int i = 0; i < m_functions.size(); i++){
		delete m_functions[i];
	}
	m_functions.clear();
	m_functions.shrink_to_fit();
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：なし
// 戻り値：なし
// 詳　細：各機能に対して初期化を依頼
//****************************************************************************
void CFunctionController::Initialize()
{
	for(int i = 0; i < m_functions.size(); i++){
		m_functions[i]->Initialize();
	}
}

//****************************************************************************
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各機能に対して更新を依頼
//****************************************************************************
void CFunctionController::Update(ShareInfo::CDocGameInfo& info)
{
	for(int i = 0; i < m_functions.size(); i++){
		m_functions[i]->Update(info);
	}
}

//****************************************************************************
// 関数名：Collision
// 概　要：衝突判定処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各機能に対して衝突判定を依頼
//****************************************************************************
void CFunctionController::Collision(ShareInfo::CDocGameInfo& info)
{}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各機能に対して描画を依頼
//****************************************************************************
void CFunctionController::Draw(ShareInfo::CDocGameInfo& info)
{
	for(int i = 0; i < m_functions.size(); i++){
		m_functions[i]->Draw(info);
	}
}

} // namespace Function