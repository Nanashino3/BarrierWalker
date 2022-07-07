//*************************************************************
// HP�@�\�N���X
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
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF���ɂȂ�
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
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁFHP��������
//****************************************************************************
void CHitPoint::Update(ShareInfo::CDocGameInfo& info)
{
	// TODO�F�����葁��Y���W���f����(�{���͕ʂ̏ꏊ�ł�肽��)
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
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁFHP�`��
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