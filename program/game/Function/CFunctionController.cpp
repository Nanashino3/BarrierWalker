//****************************************************************************
// �t�@�C�����FCFunctionController(�@�\�Ǘ���)
// ��@���@���F2022/08/06
#include "CFunctionController.h"

#include "CLifePoint.h"
#include "CStageControl.h"
#include "CBarrier.h"

namespace Function
{
CFunctionController::CFunctionController()
{
	AFunction* function = nullptr;

	//************************
	// ��Ɏg�p����@�\
	//************************
	function = new CLifePoint();		// HP
	m_functions.push_back(function);
	function = new CStageControl();	// �X�e�[�W�Ǘ�
	m_functions.push_back(function);

	//************************
	// �@�\�̕t�ւ��\
	//************************
	function = new CBarrier(FUNCTION_TYPE_BARRIER);	// �o���A
	m_functions.push_back(function);
}

CFunctionController::~CFunctionController()
{
	for(int i = 0; i < m_functions.size(); i++){
		delete m_functions[i];
	}
	m_functions.clear();
	m_functions.shrink_to_fit();
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
	for(int i = 0; i < m_functions.size(); i++){
		m_functions[i]->Initialize();
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
	for(int i = 0; i < m_functions.size(); i++){
		m_functions[i]->Update(info);
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
	for(int i = 0; i < m_functions.size(); i++){
		m_functions[i]->Draw(info);
	}
}

} // namespace Function