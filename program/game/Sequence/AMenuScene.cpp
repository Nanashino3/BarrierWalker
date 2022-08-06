//****************************************************************************
// �t�@�C�����FAMenuScene(���j���[���(����)
// ��@���@���F2022/08/05
#include "AMenuScene.h"

#include "../../dxlib_ext/dxlib_ext.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"

namespace Sequence
{
AMenuScene::AMenuScene(const std::string& filePath)
: m_sceneTextPosX(0), m_sceneTextPosY(0)
, m_menuTextPosX(0), m_menuTextPosY(0)
, m_alignmentX(0), m_alignmentY(0)
, m_cursorPos(0)
{
	m_sceneDatas = tnl::LoadCsv(filePath);

	// �V�[���e�L�X�g
	m_sceneTitle = m_sceneDatas[0][1].c_str();
	m_sceneTextPosX = atoi(m_sceneDatas[1][1].c_str());
	m_sceneTextPosY = atoi(m_sceneDatas[1][2].c_str());
	// ���j���[�e�L�X�g
	for (int i = 1; i < m_sceneDatas[2].size(); ++i) {
		std::string msg = m_sceneDatas[2][i].c_str();
		m_msgList.push_back(msg);
	}
	m_menuTextPosX = atoi(m_sceneDatas[3][1].c_str());
	m_menuTextPosY = atoi(m_sceneDatas[3][2].c_str());
	m_alignmentX = atoi(m_sceneDatas[4][2].c_str());
	m_alignmentY = atoi(m_sceneDatas[4][2].c_str());
}

AMenuScene::~AMenuScene()
{}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F��ʍX�V
// ���@���F��1����	�ꎟ�Ǘ���
//		   ��2����	�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF��ʂŕK�v�ȍX�V���s��
//****************************************************************************
void AMenuScene::MenuSelect(int& cursorPos)
{
	// ���j���[�I�𕶎��`��
	for (int i = 0; i < m_msgList.size(); i++) {
		DrawStringEx(m_menuTextPosX, m_menuTextPosY + i * m_alignmentY, GetColor(255, 255, 0), m_msgList[i].c_str());
	}
	DrawStringEx(m_menuTextPosX - m_alignmentX, m_menuTextPosY + m_cursorPos * m_alignmentY, GetColor(255, 255, 0), "��");

	// ���j���[�I������
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_W)) {
		m_cursorPos = (m_cursorPos + m_msgList.size() - 1) % m_msgList.size();
		Sound::CSoundManager::GetInstance()->PlaySE(SE_ID_MENU_SELECT);
	}
	else if (tnl::Input::IsKeyDownTrigger(eKeys::KB_S)) {
		m_cursorPos = (m_cursorPos + 1) % m_msgList.size();
		Sound::CSoundManager::GetInstance()->PlaySE(SE_ID_MENU_SELECT);
	}

	// ���j���[���菈��
	if (tnl::Input::IsKeyDownTrigger(eKeys::KB_RETURN)) {
		Sound::CSoundManager::GetInstance()->PlaySE(SE_ID_MENU_DECIDE);
		cursorPos = m_cursorPos;
	}
	return ;
}

} // namespace Sequence