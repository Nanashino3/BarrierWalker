//****************************************************************************
// ファイル名：CUiTimeLimit(制限時間UI)
// 作　成　日：2022/08/06
#include "CUiTimeLimit.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace Ui
{
CUiTimeLimit::CUiTimeLimit()
: m_elapsed(0)
, m_timeLimit(0)
{
	std::vector<int> timeLimits = { 250, 250, 250 };
	for(int i = 0; i < timeLimits.size(); ++i){
		m_timeLimits.push_back(timeLimits[i]);
	}
}

CUiTimeLimit::~CUiTimeLimit()
{}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：初期設定
//****************************************************************************
void CUiTimeLimit::Initialize(ShareInfo::CDocGameInfo& info)
{
	m_timeLimit = m_timeLimits[info.GetStageIndex()];
}

//****************************************************************************
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：タイマーの減算
//****************************************************************************
void CUiTimeLimit::Update(ShareInfo::CDocGameInfo& info)
{
	if(m_timeLimit == 0){ info.SetIsGameOver(true); }
	m_elapsed += info.GetDeltaTime();
	if(m_elapsed > 0.4f){
		m_elapsed = 0.0f;
		if(m_timeLimit > 0){ --m_timeLimit; }
	}
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：タイマーの描画
//****************************************************************************
void CUiTimeLimit::Draw(ShareInfo::CDocGameInfo&)
{
	DrawStringEx(800, 100, -1, "TIME LIMIT：%d", m_timeLimit);
}

} // namespace Ui