//*************************************************************
// �R�C����UI�N���X
//*************************************************************
#include "CUiCoinNumber.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace Ui
{
CUiCoinNumber::CUiCoinNumber()
: m_total_num(0)
, m_coin_num(0)
{}

CUiCoinNumber::~CUiCoinNumber()
{}

void CUiCoinNumber::Initialize(ShareInfo::CDocGameInfo& info)
{
	m_total_num = info.GetScoreCalcSrcInfo(ACCESS_KEY_COIN);
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�R�C�����̉��Z
//****************************************************************************
void CUiCoinNumber::Update(ShareInfo::CDocGameInfo& info)
{
	if(info.GetAcquiredItem() & ITEM_COIN){
		m_coin_num++;
		info.SetAcquiredItem(info.GetAcquiredItem() & ~ITEM_COIN);
	}

	// �X�e�[�W�N���A�����ꍇ�͎擾�����R�C����ݒ肷��
	if(info.GetIsClear()){ info.SetScoreCalcSrcInfo(ACCESS_KEY_COIN, m_total_num + m_coin_num); }
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�擾�����R�C���̕`��
//****************************************************************************
void CUiCoinNumber::Draw(ShareInfo::CDocGameInfo& info)
{
	DrawStringEx(800, 120, -1, "GET COIN�F%d", m_total_num + m_coin_num);
}

} // namespace Ui