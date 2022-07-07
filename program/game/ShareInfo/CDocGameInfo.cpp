#include "CDocGameInfo.h"
#include "../CGameDirector.h"
#include "../Camera/CCamera2D.h"

namespace ShareInfo
{
CDocGameInfo* CDocGameInfo::s_myInstance = nullptr;

CDocGameInfo::CDocGameInfo()
: m_deltaTime(0.0f)
, m_isCollision(false)
, m_currentPos(tnl::Vector3(0, 0, 0))
, m_screenMax(0)
, m_functionType(0)
, m_isPlayerHit(false)
, m_isGmaeOver(false)
, m_stageIndex(0)
, m_screenWidth(0)
, m_screenHeight(0)
, m_maxStage(0)
, m_isClear(false)
, m_isFinalStage(false)
, m_actionType(0)
, m_camera(nullptr)
, m_acquiredItem(0)
, m_changeType(0)
, m_prevScene(0)
{
	for(int i = 0; i < MAX_COLLISION_OBJECT_TYPE; i++){
		m_correctType[i] = COLLISION_OBJECT_EMPTY;
	}

	for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
		m_barrierEnergy[i] = MAX_ENERGY;
		m_barrierColor[i] = i;
	}

	m_velocity.x = 0.0f;
	m_velocity.y = 0.0f;

	for(int i = 0; i < MAX_ACCESS_KEY; i++){
		m_calcSource[i] = 0;
	}
}

CDocGameInfo::~CDocGameInfo()
{}

CDocGameInfo* CDocGameInfo::GetInstance()
{
	if(s_myInstance == nullptr){
		s_myInstance = new CDocGameInfo();
	}

	return s_myInstance;
}
void CDocGameInfo::DestroyInstance()
{
	delete s_myInstance;
}

void CDocGameInfo::Initialize()
{
	m_deltaTime		= 0.0f;
	m_isCollision	= false;
	m_currentPos	= tnl::Vector3(0, 0, 0);
	m_screenMax		= 0;
	m_functionType	= 0;
	m_isPlayerHit	= false;
	m_isGmaeOver	= false;
	m_screenWidth	= 0;
	m_screenHeight	= 0;
	m_isClear		= false;
	m_isFinalStage	= false;
	m_actionType	= 0;
	m_camera		= nullptr;
	m_changeType	= 0;

	for(int i = 0; i < MAX_COLLISION_OBJECT_TYPE; i++){
		m_correctType[i] = COLLISION_OBJECT_EMPTY;
	}

	for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
		m_barrierEnergy[i] = MAX_ENERGY;
		m_barrierColor[i] = i;
	}
}

// �o�ߎ���(1�t���[��)
void CDocGameInfo::SetDeltaTime(float value)
{
	m_deltaTime = value;
}
float CDocGameInfo::GetDeltaTime() const
{
	return m_deltaTime;
}

// �Փ˃t���O
void CDocGameInfo::SetIsCollision(bool isCollision)
{
	m_isCollision = isCollision;
}

bool CDocGameInfo::GetIsCollision() const
{
	return m_isCollision;
}

// �v���C���[�̍��W
void CDocGameInfo::SetCurrentPos(tnl::Vector3& pos)
{
	m_currentPos = pos;
}
tnl::Vector3 CDocGameInfo::GetCurrentPos() const
{
	return m_currentPos;
}

// �Փ˂�������
void CDocGameInfo::SetCorrectType(E_COLLISION_OBJECT_TYPE type, int value)
{
	m_correctType[type] = value;
}
int CDocGameInfo::GetCorrectType(E_COLLISION_OBJECT_TYPE type) const
{
	return m_correctType[type];
}

// �ő��ʐ�
void CDocGameInfo::SetScreenMax(int value)
{
	m_screenMax = value;
}
int CDocGameInfo::GetScreenMax() const
{
	return m_screenMax;
}

// �@�\���
void CDocGameInfo::SetFunctionType(unsigned int value)
{
	m_functionType = value;
}
unsigned int CDocGameInfo::GetFunctionType() const
{
	return m_functionType;
}

// �o���A�G�l���M�[
void CDocGameInfo::SetBarrierEnergy(int index, float value)
{
	m_barrierEnergy[index] = value;
}
float CDocGameInfo::GetBarrierEnergy(int index) const
{
	return m_barrierEnergy[index];
}

// �o���A�̐F
void CDocGameInfo::SetBarrierColor(int index, int value)
{
	m_barrierColor[index] = value;
}
int CDocGameInfo::GetBarrierColor(int index) const
{
	return m_barrierColor[index];
}

// Player�Ƀq�b�g������
void CDocGameInfo::SetIsPlayerHit(bool isPlayerHit)
{
	m_isPlayerHit = isPlayerHit;
}
bool CDocGameInfo::GetIsPlayerHit() const
{
	return m_isPlayerHit;
}

// �Q�[���I�[�o��
void CDocGameInfo::SetIsGameOver(bool isGameOver)
{
	m_isGmaeOver = isGameOver;
}
bool CDocGameInfo::GetIsGameOver() const
{
	return m_isGmaeOver;
}

// �X�e�[�W�ԍ�
void CDocGameInfo::SetStageIndex(int value)
{
	m_stageIndex = value;
}
int CDocGameInfo::GetStageIndex() const
{
	return m_stageIndex;
}

// ��ʕ�
void CDocGameInfo::SetScreenWidth(int value)
{
	m_screenWidth = value;
}
int CDocGameInfo::GetScreenWidth() const
{
	return m_screenWidth;
}

// ��ʍ���
void CDocGameInfo::SetScreenHeight(int value)
{
	m_screenHeight = value;
}
int CDocGameInfo::GetScreenHeight() const
{
	return m_screenHeight;
}

// �ő�X�e�[�W��
void CDocGameInfo::SetMaxStage(int value)
{
	m_maxStage = value;
}
int CDocGameInfo::GetMaxStage() const
{
	return m_maxStage;
}

// �N���A������
void CDocGameInfo::SetIsClear(bool isClear)
{
	m_isClear = isClear;
}
bool CDocGameInfo::GetIsClear() const
{
	return m_isClear;
}

// �ŏI�X�e�[�W��
void CDocGameInfo::SetIsFinalStage(bool isFinal)
{
	m_isFinalStage = isFinal;
}
bool CDocGameInfo::GetIsFinalStage() const
{
	return m_isFinalStage;
}

// �A�N�V������
void CDocGameInfo::SetActionType(unsigned int value)
{
	m_actionType = value;
}
unsigned int CDocGameInfo::GetActionType() const
{
	return m_actionType;
}

// �J�����C���X�^���X
void CDocGameInfo::SetCamera(Camera::CCamera2D* camera)
{
	m_camera = camera;
}
Camera::CCamera2D* CDocGameInfo::GetCamera() const
{
	return m_camera;
}

// �擾�����A�C�e��
void CDocGameInfo::SetAcquiredItem(unsigned int value)
{
	m_acquiredItem = value;
}
unsigned int CDocGameInfo::GetAcquiredItem() const
{
	return m_acquiredItem;
}

// �ؑփ^�C�v
void CDocGameInfo::SetChangeType(unsigned int value)
{
	m_changeType = value;
}
unsigned int CDocGameInfo::GetChangeType() const
{
	return m_changeType;
}

// ���x���
void CDocGameInfo::SetVelocity(const S_VELOCITY_INFO& value)
{
	m_velocity = value;
}
S_VELOCITY_INFO CDocGameInfo::GetVelocity() const
{
	return m_velocity;
}

// �O�̉��
void CDocGameInfo::SetPrevScene(unsigned int value)
{
	m_prevScene = value;
}
unsigned int CDocGameInfo::GetPrevScene() const
{
	return m_prevScene;
}

// �X�R�A�v�Z�����
void CDocGameInfo::SetScoreCalcSrcInfo(unsigned int index, unsigned int value)
{
	m_calcSource[index] = value;
}
unsigned int CDocGameInfo::GetScoreCalcSrcInfo(unsigned int index)
{
	return m_calcSource[index];
}

} // namespace ShareInfo