//*************************************************************
// コイン数UIクラス
//*************************************************************
#include "CUiCoinNumber.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace Ui
{
CUiCoinNumber::CUiCoinNumber()
: m_total_num(0)
, m_coin_num(0)
{}

CUiCoinNumber::~CUiCoinNumber()
{}

void CUiCoinNumber::Initialize(ShareInfo::CDocGameInfo& info)
{
	m_total_num = info.GetScoreCalcSrcInfo(ACCESS_KEY_COIN);
}

//****************************************************************************
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：コイン数の加算
//****************************************************************************
void CUiCoinNumber::Update(ShareInfo::CDocGameInfo& info)
{
	if(info.GetAcquiredItem() & ITEM_COIN){
		m_coin_num++;
		info.SetAcquiredItem(info.GetAcquiredItem() & ~ITEM_COIN);
	}

	// ステージクリアした場合は取得したコインを設定する
	if(info.GetIsClear()){ info.SetScoreCalcSrcInfo(ACCESS_KEY_COIN, m_total_num + m_coin_num); }
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：取得したコインの描画
//****************************************************************************
void CUiCoinNumber::Draw(ShareInfo::CDocGameInfo& info)
{
	DrawStringEx(800, 120, -1, "GET COIN：%d", m_total_num + m_coin_num);
}

} // namespace Ui