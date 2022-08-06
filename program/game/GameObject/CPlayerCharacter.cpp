//*************************************************************
// �v���C���[�L�����N�^�[�N���X
//*************************************************************
#include "CPlayerCharacter.h"
#include "CONST_GAMEOBJECT_VALUE.h"

#define NOMINMAX
#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"

namespace GameObject
{
CPlayerCharacter::CPlayerCharacter()
: m_posX(0), m_posY(0)
, m_actionType(0)
, m_velocityX(0), m_velocityY(0)
, m_animType(ANIM_TYPE_RIGHT)
, m_animFrame(0)
{
	LoadDivGraph("resource/c1_anim_left.png", SIZE_4, PLAYER_X_NUM, PLAYER_Y_NUM, PLAYER_RECT_W, PLAYER_RECT_H, m_gfxHdl[ANIM_TYPE_LEFT]);
	LoadDivGraph("resource/c1_anim_right.png", SIZE_4, PLAYER_X_NUM, PLAYER_Y_NUM, PLAYER_RECT_W, PLAYER_RECT_H, m_gfxHdl[ANIM_TYPE_RIGHT]);

	m_soundManager = Sound::CSoundManager::GetInstance();
}

CPlayerCharacter::~CPlayerCharacter()
{
	for(int i = 0; i < MAX_ANIM_TYPE; i++){
		for(int j = 0; j < MAX_ANIM_TYPE; j++){
			DeleteGraph(m_gfxHdl[i][j]);
		}
	}
}

//****************************************************************************
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e�����o�̏������⏉�����W�̌v�Z��
//****************************************************************************
void CPlayerCharacter::Initialize(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	// �������W�ݒ�
 	m_posX = PLAYER_FIRST_POSX - (info.GetScreenWidth() >> 1);
	m_posY = PLAYER_FIRST_POSY - (info.GetScreenHeight() >> 1);
	tnl::Vector3 pos(m_posX, m_posY, 0);
	info.SetCurrentPos(pos);

	m_actionType = 0;
	m_velocityY = 0.0f;
	m_velocityX = 0.0f;
	m_animType	 = ANIM_TYPE_RIGHT;
	m_animFrame = 0;
}

//****************************************************************************
// �֐����FMove
// �T�@�v�F�ړ�����
// ���@���F��1�����@���݈ʒu
// �@�@�@�@��2�����@�O��ʒu
// �@�@�@�@��3�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�v���C���[�̈ړ�����(���E�ړ��E�_�b�V���E�W�����v)
//****************************************************************************
void CPlayerCharacter::Move(tnl::Vector3& current_pos, tnl::Vector3& prev_pos, ShareInfo::CDocGameInfo& info)
{
	// �ړ��O�̍��W��ݒ�
	prev_pos = tnl::Vector3(m_posX, m_posY, 0);

	// �ړ�����
	float dx = 0.0f;
	if(tnl::Input::IsKeyDown(eKeys::KB_D)){
		dx =  1.0f;
		m_animType = ANIM_TYPE_RIGHT;
		m_actionType |= IS_MOVING;
	}else if(tnl::Input::IsKeyDown(eKeys::KB_A)){
		dx = -1.0f;
		m_animType = ANIM_TYPE_LEFT;
		m_actionType |= IS_MOVING;
	}else{
		m_actionType &= ~IS_MOVING;
	}
	
	// �_�b�V���@�\
	if(dx != 0.0f){ PriFunction_Dash(dx); }

	// �W�����v�@�\
	PriFunction_Jump();

	// ���x����ݒ�
	//info.SetVelocity(ShareInfo::S_VELOCITY_INFO(m_velocityX, m_velocityY));

	// �ړ��W�����v���̏ꍇ�A�ړ��O���W�����ݍ��W�ƍ��킹�Ă���
	// (�΂߉E��ړ������ꍇ�A�������ɓ�����������ɂȂ�)
	if(((m_actionType & IS_MOVING) && (m_actionType & IS_JUMPING))){
		prev_pos.y = m_posY;
	}

	// �ŏI���W��ݒ肷��
	current_pos = tnl::Vector3(m_posX, m_posY, 0);
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�ړ��͈͐����I�u�W�F�N�g�Ƃ̏Փ˔����̍X�V������
//****************************************************************************
void CPlayerCharacter::Update(ShareInfo::CDocGameInfo& info)
{
	// �A�j���[�V�����̃t���[����������
	static float anim_time_count;
	anim_time_count += info.GetDeltaTime();
	if(anim_time_count > 0.25f){
		anim_time_count = 0;
		m_animFrame++;
		m_animFrame %= MAX_ANIM_TYPE;
	}

	// �����蔻��̌��ʂ���K�v�ȏ������s��
	if(info.GetIsCollision()){
		// TODO�F�����Ă�����z��Ŏ�������
		int stageCorrect = info.GetCorrectType(ShareInfo::COLLISION_OBJECT_STAGE);
		int enemyCorrect = info.GetCorrectType(ShareInfo::COLLISION_OBJECT_ENMEY);
		int floorCorrect = info.GetCorrectType(ShareInfo::COLLISION_OBJECT_MOVE_FLOOR);

		bool canInitByStage = (stageCorrect == CORRECT_UP || stageCorrect == CORRECT_DOWN) ? true : false;
		bool canInitByEnemy = (enemyCorrect == CORRECT_UP || enemyCorrect == CORRECT_DOWN) ? true : false;
		bool canInitByFloor = (floorCorrect == CORRECT_UP) ? true : false;

		if(canInitByStage || canInitByEnemy || canInitByFloor){
			m_velocityY = 0.0f;
			m_actionType &= ~IS_JUMPING;
		}

		m_posX = info.GetCurrentPos().x;
		m_posY = info.GetCurrentPos().y;
	}
	info.SetActionType(m_actionType);
	PriMoveConstrain(info);
	PriGameInfoClear(info);
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�v���C���[�̕`�揈��
//****************************************************************************
void CPlayerCharacter::Draw(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;

	int view_playerPos_x = m_posX - camera->GetPosition().x + screen_half_w;
	int view_playerPos_y = m_posY - camera->GetPosition().y + screen_half_h;
	DrawRotaGraph(view_playerPos_x, view_playerPos_y, 1.0f, 0, m_gfxHdl[m_animType][m_animFrame], true);
}

//****************************************************************************
// �֐����FPriFunction_Dash
// �T�@�v�F�_�b�V���@�\(Private)
// ���@���F��1�����@�������������߂̕���
// �߂�l�F�Ȃ�
// �ځ@�ׁF�_�b�V������
//****************************************************************************
void CPlayerCharacter::PriFunction_Dash(float dx)
{
	if(tnl::Input::IsKeyDown(eKeys::KB_LSHIFT)){
		m_velocityX += PLAYER_DASH_ACCEL;
		m_actionType |= IS_DASHING;
	}else{
		m_velocityX -= PLAYER_DASH_ACCEL;
		m_actionType &= ~IS_DASHING;
	}
	if(m_velocityX < 0.0f){ m_velocityX = 0.0f; }
	if(m_velocityX > PLAYER_DASH_SPEED){ m_velocityX = PLAYER_DASH_SPEED; }

	m_posX += (PLAYER_SPEED + m_velocityX) * dx;
}

//****************************************************************************
// �֐����FPriFunction_Jump
// �T�@�v�F�W�����v����(Private)
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�W�����v����
//****************************************************************************
void CPlayerCharacter::PriFunction_Jump()
{
	// �W�����v����
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_SPACE) && !(m_actionType & IS_JUMPING) && !(m_actionType & IS_AIR)){
		m_velocityY = PLAYER_JUMP_SPEED;
		m_actionType |= IS_JUMPING;
		m_soundManager->PlaySE(SE_ID_JUMP);
	}

	// �d�͏���
	m_posY += m_velocityY;
	m_velocityY += PLAYER_JUMP_ACCEL;
	m_velocityY = std::min(30.0f, std::max(-30.0f, m_velocityY));

	// �󒆔���
	if((int)m_velocityY != 0){
		m_actionType |= IS_AIR;
	}else{
		if(!(m_actionType & IS_JUMPING)){ m_actionType &= ~IS_AIR; }
	}
}

//****************************************************************************
// �֐����FPriMoveConstrain
// �T�@�v�F�ړ�����(Private)
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�v���C���|�̈ړ��͈͂𐧌䂷��
//****************************************************************************
void CPlayerCharacter::PriMoveConstrain(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camera_pos = info.GetCamera()->GetPosition();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;
	int rect_half_w = PLAYER_RECT_W >> 1;
	int rect_half_h = PLAYER_RECT_H >> 1;

	// X������
	if(m_posX < camera_pos.x - screen_half_w + rect_half_w){
		m_posX = camera_pos.x - screen_half_w + rect_half_w;
	}
	else if(m_posX > info.GetScreenWidth() * info.GetScreenMax() - (screen_half_w + rect_half_w)){
		m_posX = info.GetScreenWidth() * info.GetScreenMax() - (screen_half_w + rect_half_w);
	}
}

//****************************************************************************
// �֐����FPriGameInfoClear
// �T�@�v�F�Q�[����������������(Private)
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�v���C���[�X�V��Փ˔���̃t���O�Ȃǂ����������邽�߂̊֐�
// �@�@�@�@�������A�v���C���[�N���X�̐Ӗ��ł͂Ȃ��̂łǂ����̃^�C�~���O�Ő؂藣��
//****************************************************************************
void CPlayerCharacter::PriGameInfoClear(ShareInfo::CDocGameInfo& info)
{
	info.SetIsCollision(false);
	for(int i = 0; i < ShareInfo::MAX_COLLISION_OBJECT_TYPE; i++){
		info.SetCorrectType(static_cast<ShareInfo::E_COLLISION_OBJECT_TYPE>(i), NO_HIT);
	}
	tnl::Vector3 init(m_posX, m_posY, 0);
	info.SetCurrentPos(init);
}

} // namespace GameObject