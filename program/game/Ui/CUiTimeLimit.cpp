//****************************************************************************
// �t�@�C�����FCUiTimeLimit(��������UI)
// ��@���@���F2022/08/06
#include "CUiTimeLimit.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace Ui
{
CUiTimeLimit::CUiTimeLimit()
: m_elapsed(0)
, m_timeLimit(0)
{
	std::vector<int> timeLimits = { 250, 250, 250 };
	for(int i = 0; i < timeLimits.size(); ++i){
		m_timeLimits.push_back(timeLimits[i]);
	}
}

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
	m_timeLimit = m_timeLimits[info.GetStageIndex()];
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