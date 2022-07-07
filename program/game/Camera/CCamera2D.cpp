//*************************************************************
// カメラ2Dクラス
//*************************************************************
#include "CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace Camera
{
CCamera2D::CCamera2D(int screen_w, int screen_h, const tnl::Vector3& pos)
: m_screen_w(screen_w), m_screen_h(screen_h)
, m_pos(pos)
{}
CCamera2D::~CCamera2D()
{}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：なし
// 戻り値：なし
// 詳　細：カメラの初期化処理
//****************************************************************************
void CCamera2D::Initialize()
{
	m_pos = tnl::Vector3(0, 0, 0);
}

//****************************************************************************
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：カメラの注視点を更新
//****************************************************************************
void CCamera2D::Update(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 pos = info.GetCurrentPos();
	int screen_max = info.GetScreenMax();
	
	// カメラの注視点まで移動したらカメラ座標を動かす(プレイヤー座標に合わせる)
	if(pos.x >= m_pos.x){
		m_pos.x = pos.x;
	}

	// 右画面端まで到達した場合はカメラ位置を固定する
	int screenHalf_x = m_screen_w >> 1;
	if(m_pos.x + screenHalf_x >= m_screen_w * screen_max - screenHalf_x){
		m_pos.x = m_screen_w * screen_max - m_screen_w;
	}
}

} // namespace Camera