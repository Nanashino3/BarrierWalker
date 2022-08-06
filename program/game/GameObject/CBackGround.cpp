#include "CBackGround.h"

#include "../Camera/CCamera2D.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace{
const float FIRST_POS_X = SCREEN_WIDTH >> 1;
const float FIRST_POS_Y = SCREEN_HEIGHT >> 1;

const std::vector<std::string> g_images = {
	//"resource/background/stage1_bg.png",
	//"resource/background/stage2_bg.png",
	"resource/background/stage3_bg.png"
};
}

namespace GameObject
{
CBackGround::CBackGround()
: m_gfxHdl(0)
, m_posX(0.0f), m_posY(0.0f)
{}

CBackGround::~CBackGround()
{
	Utility::DeleteGraphEx(m_gfxHdl);
}

void CBackGround::Initialize(ShareInfo::CDocGameInfo& info)
{
	int index = info.GetStageIndex();
	m_gfxHdl = Utility::LoadGraphEx(g_images[index].c_str());

	m_posX = FIRST_POS_X - (info.GetScreenWidth() >> 1);
	m_posY = FIRST_POS_Y - (info.GetScreenHeight() >> 1);
}

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