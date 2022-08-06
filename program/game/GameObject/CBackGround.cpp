//****************************************************************************
// �t�@�C�����FCBackGround(�w�i)
// ��@���@���F2022/08/06
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
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F��1����	�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF
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
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1����	�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF
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