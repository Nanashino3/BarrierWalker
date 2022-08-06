//****************************************************************************
// �t�@�C�����FCGameDirector(�Q�[���f�B���N�^�[)
// ��@���@���F2022/08/06
#include "CGameDirector.h"

#include "Camera/CCamera2D.h"
#include "Function/CFunctionController.h"
#include "GameObject/CPlayerCharacter.h"
#include "GameObject/CBackGround.h"
#include "GameObject/CFixedEnemy.h"
#include "GameObject/CFixedFloor.h"
#include "GameObject/CItemManager.h"
#include "GameObject/CMoveObjectManager.h"
#include "ShareInfo/CDocGameInfo.h"
#include "Ui/CUiController.h"

CGameDirector::CGameDirector()
: m_camera(nullptr)
, m_functionController(nullptr)
, m_docGameInfo(nullptr)
{
	m_camera = new Camera::CCamera2D(SCREEN_WIDTH, SCREEN_HEIGHT);

	// �Q�[�����ݒ�
	m_docGameInfo = ShareInfo::CDocGameInfo::GetInstance();
	m_docGameInfo->SetCamera(m_camera);
	m_docGameInfo->SetScreenWidth(SCREEN_WIDTH);
	m_docGameInfo->SetScreenHeight(SCREEN_HEIGHT);

	// �C���X�^���X����
	m_functionController = new Function::CFunctionController();
	m_uiController = new Ui::CUiController();
	m_playerObject = new GameObject::CPlayerCharacter;
	m_moveObjManager = new GameObject::CMoveObjectManager();
	GameObject::AFixedObject* fixedObject = nullptr;
	fixedObject = new GameObject::CBackGround;		// �w�i
	m_fixedObjects.push_back(fixedObject);
	fixedObject = new GameObject::CFixedFloor;		// �Œ�X�e�[�W
	m_fixedObjects.push_back(fixedObject);
	fixedObject = new GameObject::CFixedEnemy;		// �Œ�G
	m_fixedObjects.push_back(fixedObject);
	fixedObject = new GameObject::CItemManager;		// �A�C�e��
	m_fixedObjects.push_back(fixedObject);
}

CGameDirector::~CGameDirector()
{
	delete m_camera;
	delete m_functionController;;
	delete m_uiController;
	delete m_playerObject;
	delete m_moveObjManager;

	for(int i = 0; i < m_fixedObjects.size(); i++){
		delete m_fixedObjects[i];
	}
	m_fixedObjects.clear();
	m_fixedObjects.shrink_to_fit();
}

//****************************************************************************
// �֐����FInitialize
// �T�@�v�F�Q�[������������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�v���C��ʂɊւ��I�u�W�F�N�g�EUI�E�@�\�̏��������s��
//****************************************************************************
void CGameDirector::Initialize()
{
	tnl::DebugTrace("GameD->Initialize\n");
	m_camera->Initialize();
	m_playerObject->Initialize(*m_docGameInfo);
	m_moveObjManager->Initialize(*m_docGameInfo);
	for(int i = 0; i < m_fixedObjects.size(); i++){
		m_fixedObjects[i]->Initialize(*m_docGameInfo);
	}
	m_functionController->Initialize();
	m_uiController->Initialize(*m_docGameInfo);
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�Q�[���X�V����
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�v���C��ʂɊւ��I�u�W�F�N�g�EUI�E�@�\�̍X�V���s��
// �@�@�@�@�������ňړ��ⓖ���蔻����s��
//****************************************************************************
void CGameDirector::Update()
{
	if(m_docGameInfo->GetIsClear() || m_docGameInfo->GetIsGameOver()){ return; }

	// �ړ�
	tnl::Vector3 prevPos(0, 0, 0), currentPos(0, 0, 0);
	m_moveObjManager->Move(*m_docGameInfo);
	m_playerObject->Move(currentPos, prevPos, *m_docGameInfo);

	// �Փ˔���
	for(int s_cnt = 0; s_cnt < m_fixedObjects.size(); s_cnt++){
		m_fixedObjects[s_cnt]->Collision(currentPos, prevPos, *m_docGameInfo);
	}
	m_moveObjManager->Collision(currentPos, prevPos, *m_docGameInfo);

	// �X�V
	m_playerObject->Update(*m_docGameInfo);
	m_functionController->Update(*m_docGameInfo);
	m_uiController->Update(*m_docGameInfo);
	for(int i = 0; i < m_fixedObjects.size(); i++){
		m_fixedObjects[i]->Update(*m_docGameInfo);
	}
	m_camera->Update(*m_docGameInfo);
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�Q�[���`�揈��
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�v���C��ʂɊւ��I�u�W�F�N�g�EUI�E�@�\�̕`����s��
// �@�@�@�@���`�揇���ӎ�����K�v������
//****************************************************************************
void CGameDirector::Draw()
{
	for(int i = 0; i < m_fixedObjects.size(); i++){
		m_fixedObjects[i]->Draw(*m_docGameInfo);
	}
	m_moveObjManager->Draw(*m_docGameInfo);
	m_uiController->Draw(*m_docGameInfo);
	m_playerObject->Draw(*m_docGameInfo);
	m_functionController->Draw(*m_docGameInfo);
}