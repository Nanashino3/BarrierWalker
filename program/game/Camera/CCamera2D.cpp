//*************************************************************
// �J����2D�N���X
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
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�J�����̏���������
//****************************************************************************
void CCamera2D::Initialize()
{
	m_pos = tnl::Vector3(0, 0, 0);
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�J�����̒����_���X�V
//****************************************************************************
void CCamera2D::Update(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 pos = info.GetCurrentPos();
	int screen_max = info.GetScreenMax();
	
	// �J�����̒����_�܂ňړ�������J�������W�𓮂���(�v���C���[���W�ɍ��킹��)
	if(pos.x >= m_pos.x){
		m_pos.x = pos.x;
	}

	// �E��ʒ[�܂œ��B�����ꍇ�̓J�����ʒu���Œ肷��
	int screenHalf_x = m_screen_w >> 1;
	if(m_pos.x + screenHalf_x >= m_screen_w * screen_max - screenHalf_x){
		m_pos.x = m_screen_w * screen_max - m_screen_w;
	}
}

} // namespace Camera