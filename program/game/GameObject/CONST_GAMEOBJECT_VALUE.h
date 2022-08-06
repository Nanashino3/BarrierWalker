#ifndef GAMEOBJECT_CONST_GAMEOBJECT_VALUE_H_
#define GAMEOBJECT_CONST_GAMEOBJECT_VALUE_H_

namespace GameObject
{
//****************************************************************************
// �ړ��I�u�W�F�N�g�֘A�萔
const unsigned int MOVE_OBJ_RECTW = 64;
const unsigned int MOVE_OBJ_RECTH = 24;

const int MOVE_UP     = 0;
const int MOVE_RIGHT  = 1;
const int MOVE_BOTTOM = 2;
const int MOVE_LEFT   = 3;
//****************************************************************************

//****************************************************************************
// �v���C���[�֘A�萔
const float PLAYER_SPEED = 2.0f;

// �W�����v�֘A
const float PLAYER_JUMP_SPEED = -6.5f;
const float PLAYER_JUMP_ACCEL = 0.2f;

// �_�b�V���֘A
const float PLAYER_DASH_SPEED	= 1.0f;
const float PLAYER_DASH_ACCEL	= 0.1f;

// �������W(�X�N���[�����W)
const float PLAYER_FIRST_POSX = 75.f;
const float PLAYER_FIRST_POSY = 551.f;
//****************************************************************************

//****************************************************************************
// �Œ�I�֘A�萔
// �e�F�e��
const int MAX_BULLET = 10;
// ��`�T�C�Y
const int FIXEDENEMY_P_RECT_W = 24;
//****************************************************************************
}

#endif // #ifndef GAMEOBJECT_CONST_GAMEOBJECT_VALUE_H_