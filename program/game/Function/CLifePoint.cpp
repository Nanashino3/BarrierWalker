//****************************************************************************
// ファイル名：CLifePoint(体力管理)
// 作　成　日：2022/08/06
#include "CLifePoint.h"

#include "../Camera/CCamera2D.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"

namespace Function
{
CLifePoint::CLifePoint(unsigned int type)
: AFunction(type)
, m_lifePoint(0)
, m_hitting(false)
, m_soundManager(nullptr)
{
	for (int i = 0; i < MAX_LIFE_POINT; ++i) {
		m_gfxHdl[i] = 0;
	}
}

CLifePoint::~CLifePoint()
{
	for(int i = 0; i < MAX_LIFE_POINT; ++i){
		Utility::CImageManager::GetInstance()->DeleteGraphEx(m_gfxHdl[i]);
	}
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：なし
// 戻り値：なし
// 詳　細：特になし
//****************************************************************************
void CLifePoint::Initialize()
{
	m_lifePoint = MAX_LIFE_POINT;
	for (int i = 0; i < MAX_LIFE_POINT; ++i) {
		m_gfxHdl[i] = Utility::CImageManager::GetInstance()->LoadGraphEx("resource/ui/sticon3g-3.png");
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
void CLifePoint::Update(ShareInfo::CDocGameInfo& info)
{
	// TODO：手っ取り早くY座標判断する(本来は別の場所でやりたい)
	if(m_lifePoint == 0 || info.GetCurrentPos().y > 768.0f){
		info.SetIsGameOver(true);
		return;
	}

	if(!m_hitting && info.GetIsPlayerHit()){
		if(m_lifePoint > 0){
			m_lifePoint--;
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
void CLifePoint::Draw(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 pos = info.GetCurrentPos();
	Camera::CCamera2D* camera = info.GetCamera();
	int screenHalfW = info.GetScreenWidth() >> 1;
	int screenHalfH = info.GetScreenHeight() >> 1;

	for(int i = 0; i < m_lifePoint; ++i){
		int viewPosX = (pos.x - LIFE_POINT_OFFSETX) + i * LIFE_POINT_OFFSETX - camera->GetPosition().x + screenHalfW;
		int viewPosY = (pos.y - LIFE_POINT_OFFSETY) - camera->GetPosition().y + screenHalfH;
		DrawRotaGraph(viewPosX, viewPosY, 0.75f, 0, m_gfxHdl[i], true);
	}
}

} // namespace Function