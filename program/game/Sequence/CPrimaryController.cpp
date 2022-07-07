// �ꎟ�Ǘ���
#include "CPrimaryController.h"
#include "CTitleScene.h"

#include "../ShareInfo/CDocGameInfo.h"

namespace Sequence
{
CPrimaryController::CPrimaryController()
: m_currentScene(nullptr)
{
	m_currentScene = new CTitleScene;
}

CPrimaryController::~CPrimaryController()
{
	delete m_currentScene;
}

// ��ʍX�V
bool CPrimaryController::Update(ShareInfo::CDocGameInfo& info)
{
	// �e��ʂ��玟�̑J�ڐ��Ԃ��Ă��炤
	IScene* nextScene = m_currentScene->Update(*this, info);
	if(nextScene == nullptr){ return false; }

	// ���݂Ǝ��̃V�[�����قȂ��Ă���΃C���X�^���X�����ւ���
	if(nextScene != m_currentScene){
		delete m_currentScene;
		m_currentScene = nextScene;
	}

	nextScene = nullptr;
	return true;
}

} // namespace Sequence