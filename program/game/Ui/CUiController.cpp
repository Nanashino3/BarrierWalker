//****************************************************************************
// �t�@�C�����FCUiController(UI�Ǘ���)
// ��@���@���F2022/08/06
#include "CUiController.h"

#include "CUiBarrier.h"
#include "CUiTrilemma.h"
#include "CUiTimeLimit.h"
#include "CUiCoinNumber.h"

namespace Ui
{
CUiController::CUiController()
{
	AUi* ui = nullptr;
	ui = new CUiBarrier();		// �o���AUI
	m_uiList.push_back(ui);
	ui = new CUiTrilemma();		// �O������UI
	m_uiList.push_back(ui);
	ui = new CUiTimeLimit();	// ��������UI
	m_uiList.push_back(ui);
	ui = new CUiCoinNumber();	// �R�C����UI
	m_uiList.push_back(ui);
}

CUiController::~CUiController()
{
	for (const auto& ui : m_uiList) {
		delete ui;
	}
	m_uiList.clear();
	m_uiList.shrink_to_fit();
}

//****************************************************************************
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�eUI�N���X�ɏ��������˗�
//****************************************************************************
void CUiController::Initialize(ShareInfo::CDocGameInfo& info)
{
	for(const auto& ui : m_uiList){
		ui->Initialize(info);
	}
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�eUI�N���X�ɍX�V���˗�
//****************************************************************************
void CUiController::Update(ShareInfo::CDocGameInfo& info)
{
	for (const auto& ui : m_uiList) {
		ui->Update(info);
	}
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�eUI�N���X�ɕ`����˗�
//****************************************************************************
void CUiController::Draw(ShareInfo::CDocGameInfo& info)
{
	for (const auto& ui : m_uiList) {
		ui->Draw(info);
	}
}

} // namespace UserInterface