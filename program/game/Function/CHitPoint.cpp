//*************************************************************
// HP機能クラス
//*************************************************************
#include "CHitPoint.h"

#include "../Camera/CCamera2D.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"

namespace{
const int LAYOUT_OFFSET_X = 20;
const int LAYOUT_OFFSET_Y = 30;

int g_gfxHdl[MAX_HIT_POINT] = {0};
}

namespace Function
{
CHitPoint::CHitPoint(unsigned int type)
: AFunction(type)
, m_hitPoint(0)
, m_hitting(false)
, m_soundManager(nullptr)
{}

CHitPoint::~CHitPoint()
{
	for(int i = 0; i < MAX_HIT_POINT; i++){
		Utility::DeleteGraphEx(g_gfxHdl[i]);
	}
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：なし
// 戻り値：なし
// 詳　細：特になし
//****************************************************************************
void CHitPoint::Initialize()
{
	m_hitPoint = MAX_HIT_POINT;
	for (int i = 0; i < MAX_HIT_POINT; i++) {
		g_gfxHdl[i] = Utility::LoadGraphEx("resource/ui/sticon3g-3.png");
	}

	m_soundManager = Sound::CSoundManager::GetInstance();
}

//****************************************************************************
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：HP減少処理
//****************************************************************************
void CHitPoint::Update(ShareInfo::CDocGameInfo& info)
{
	// TODO：手っ取り早くY座標判断する(本来は別の場所でやりたい)
	if(m_hitPoint == 0 || info.GetCurrentPos().y > 768.0f){
		info.SetIsGameOver(true);
		return;
	}

	if(!m_hitting && info.GetIsPlayerHit()){
		if(m_hitPoint > 0){
			m_hitPoint--;
			m_soundManager->PlaySE(SE_ID_DAMAGE);
		}
	}
	m_hitting = info.GetIsPlayerHit();
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：HP描画
//****************************************************************************
void CHitPoint::Draw(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 pos = info.GetCurrentPos();
	Camera::CCamera2D* camera = info.GetCamera();
	int screenHalfW = info.GetScreenWidth() >> 1;
	int screenHalfH = info.GetScreenHeight() >> 1;

	for(int i = 0; i < m_hitPoint; i++){
		int viewPosX = (pos.x - LAYOUT_OFFSET_X) + i * LAYOUT_OFFSET_X - camera->GetPosition().x + screenHalfW;
		int viewPosY = (pos.y - LAYOUT_OFFSET_Y) - camera->GetPosition().y + screenHalfH;
		DrawRotaGraph(viewPosX, viewPosY, 0.75f, 0, g_gfxHdl[i], true);
	}
}

} // namespace Function