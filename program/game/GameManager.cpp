//****************************************************************************
// �t�@�C�����FGameManager(�Q�[���Ǘ���)
// ��@���@���F2022/08/05
#include "GameManager.h"

#include "Sequence/CPrimaryController.h"
#include "ShareInfo/CDocGameInfo.h"
#include "Sound/CSoundManager.h"

GameManager* GameManager::m_instance = nullptr;
GameManager::GameManager()
{
	Sound::CSoundManager::CreateInstance();
	m_docGameInfo = ShareInfo::CDocGameInfo::GetInstance();
	m_sceneControl = new Sequence::CPrimaryController();
}

//****************************************************************************
// �֐����FGetInstance
// �T�@�v�F�C���X�^���X�擾
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�Q�[���Ǘ��҂̃C���X�^���X���擾����
//****************************************************************************
GameManager* GameManager::GetInstance()
{
	if(m_instance == nullptr){
		m_instance = new GameManager();
	}

	return m_instance;
}

//****************************************************************************
// �֐����FDestroyInstance
// �T�@�v�F�C���X�^���X�̔j��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�Q�[���Ǘ��҂̃C���X�^���X��j������
//****************************************************************************
void GameManager::DestroyInstance()
{
	delete m_instance;
	m_instance = nullptr;
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�Q�[���X�V����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�Q�[���̍X�V�������s��
//****************************************************************************
bool GameManager::Update(float deltaTime)
{
	m_docGameInfo->SetDeltaTime(deltaTime);
	// �V�[���J�ڂ��R���g���[��
	if(!m_sceneControl->Update(*m_docGameInfo)){
		delete m_sceneControl;
		ShareInfo::CDocGameInfo::DestroyInstance();
		Sound::CSoundManager::DestroyInstance();

		return false;
	}

	return true;
}