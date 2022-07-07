//*************************************************************
// �^�C�}�[UI�N���X
//*************************************************************
#include "CUiTimeLimit.h"

#include "../ShareInfo/CDocGameInfo.h"

namespace {
const int TIME_LIMIT[] = { 250, 250, 250};
}

namespace Ui
{
CUiTimeLimit::CUiTimeLimit()
: m_elapsed(0)
, m_timeLimit(0)
{}

CUiTimeLimit::~CUiTimeLimit()
{}

//****************************************************************************
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�����ݒ�
//****************************************************************************
void CUiTimeLimit::Initialize(ShareInfo::CDocGameInfo& info)
{
	m_timeLimit = TIME_LIMIT[info.GetStageIndex()];
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�^�C�}�[�̌��Z
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
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�^�C�}�[�̕`��
//****************************************************************************
void CUiTimeLimit::Draw(ShareInfo::CDocGameInfo&)
{
	DrawStringEx(800, 100, -1, "TIME LIMIT�F%d", m_timeLimit);
}

} // namespace Ui