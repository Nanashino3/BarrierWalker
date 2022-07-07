//*************************************************************
// �@�\�Ǘ���
//*************************************************************
#include "CFunctionController.h"

#include "CHitPoint.h"
#include "CStageControl.h"
#include "CBarrier.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "..//ShareInfo/CONST_GAME_VALUE.h"

namespace{
std::vector<Function::AFunction*> g_functions;
}

namespace Function
{
//****************************************************************************
// �֐����FCFunctionController
// �T�@�v�F�R���X�g���N�^
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�@�\�̐����Ɛݒ�
//****************************************************************************
CFunctionController::CFunctionController()
{
	AFunction* function = nullptr;

	//************************
	// ��Ɏg�p����@�\
	//************************
	function = new CHitPoint();		// HP
	g_functions.push_back(function);
	function = new CStageControl();	// �X�e�[�W�Ǘ�
	g_functions.push_back(function);

	//************************
	// �@�\�̕t�ւ��\
	//************************
	function = new CBarrier(FUNCTION_TYPE_BARRIER);	// �o���A
	g_functions.push_back(function);
}

//****************************************************************************
// �֐����F~CFunctionController
// �T�@�v�F�f�X�g���N�^
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�㏈�����s��
//****************************************************************************
CFunctionController::~CFunctionController()
{
	for(int i = 0; i < g_functions.size(); i++){
		delete g_functions[i];
	}
	g_functions.clear();
	g_functions.shrink_to_fit();
}

//****************************************************************************
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e�@�\�ɑ΂��ď��������˗�
//****************************************************************************
void CFunctionController::Initialize()
{
	for(int i = 0; i < g_functions.size(); i++){
		g_functions[i]->Initialize();
	}
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e�@�\�ɑ΂��čX�V���˗�
//****************************************************************************
void CFunctionController::Update(ShareInfo::CDocGameInfo& info)
{
	for(int i = 0; i < g_functions.size(); i++){
		g_functions[i]->Update(info);
	}
}

//****************************************************************************
// �֐����FCollision
// �T�@�v�F�Փ˔��菈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e�@�\�ɑ΂��ďՓ˔�����˗�
//****************************************************************************
void CFunctionController::Collision(ShareInfo::CDocGameInfo& info)
{}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e�@�\�ɑ΂��ĕ`����˗�
//****************************************************************************
void CFunctionController::Draw(ShareInfo::CDocGameInfo& info)
{
	for(int i = 0; i < g_functions.size(); i++){
		g_functions[i]->Draw(info);
	}
}

} // namespace Function