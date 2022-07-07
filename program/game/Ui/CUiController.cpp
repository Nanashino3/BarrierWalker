//*************************************************************
// UI�Ǘ��҃N���X
//*************************************************************
#include "CUiController.h"

#include "CUiBarrier.h"
#include "CUiTrilemma.h"
#include "CUiTimeLimit.h"
#include "CUiCoinNumber.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace{ std::vector<Ui::AUi*> g_ui; }
namespace Ui
{
CUiController::CUiController()
{
	AUi* ui = nullptr;
	ui = new CUiBarrier();		// �o���AUI
	g_ui.push_back(ui);
	ui = new CUiTrilemma();		// �O������UI
	g_ui.push_back(ui);
	ui = new CUiTimeLimit();	// ��������UI
	g_ui.push_back(ui);
	ui = new CUiCoinNumber();	// �R�C����UI
	g_ui.push_back(ui);
}

CUiController::~CUiController()
{
	for(int i = 0; i < g_ui.size(); i++){
		delete g_ui[i];
	}
	g_ui.clear();
	g_ui.shrink_to_fit();
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
	for(int i = 0; i < g_ui.size(); i++){
		g_ui[i]->Initialize(info);
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
	for(int i = 0; i < g_ui.size(); i++){
		g_ui[i]->Update(info);
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
	for(int i = 0; i < g_ui.size(); i++){
		g_ui[i]->Draw(info);
	}
}

} // namespace UserInterface