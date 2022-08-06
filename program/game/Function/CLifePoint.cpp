//****************************************************************************
// �t�@�C�����FCLifePoint(�̗͊Ǘ�)
// ��@���@���F2022/08/06
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
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF���ɂȂ�
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
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁFHP��������
//****************************************************************************
void CLifePoint::Update(ShareInfo::CDocGameInfo& info)
{
	// TODO�F�����葁��Y���W���f����(�{���͕ʂ̏ꏊ�ł�肽��)
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
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁFHP�`��
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