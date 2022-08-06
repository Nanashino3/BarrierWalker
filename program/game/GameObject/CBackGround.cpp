//****************************************************************************
// ファイル名：CBackGround(背景)
// 作　成　日：2022/08/06
#include "CBackGround.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Utility/CImageManager.h"

namespace GameObject
{
CBackGround::CBackGround()
: m_gfxHdl(0)
, m_posX(0.0f), m_posY(0.0f)
{}

CBackGround::~CBackGround()
{
	Utility::CImageManager::GetInstance()->DeleteGraphEx(m_gfxHdl);
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数	ゲーム情報
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CBackGround::Initialize(ShareInfo::CDocGameInfo& info)
{
	int index = info.GetStageIndex();
	std::string fileList[] = {
		//"resource/background/stage1_bg.png",
		//"resource/background/stage2_bg.png",
		"resource/background/stage3_bg.png"
	};
	m_gfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx(fileList[index].c_str());
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数	ゲーム情報
// 戻り値：なし
// 詳　細：
//****************************************************************************
void CBackGround::Draw(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screenHalf_w = info.GetScreenWidth() >> 1;
	int screenHalf_h = info.GetScreenHeight() >> 1;

	int viewPosX = m_posX - camera->GetPosition().x + screenHalf_w;
	int viewPosY = m_posY - camera->GetPosition().y + screenHalf_h;
	for(int i = 0; i < info.GetScreenMax(); i++){
		DrawRotaGraph(viewPosX + i * info.GetScreenWidth(), viewPosY, 1.0f, 0, m_gfxHdl, true);
	}
}

} // namespace GameObject