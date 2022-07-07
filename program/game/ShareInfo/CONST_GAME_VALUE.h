#ifndef SHAREINFO_CONST_GAME_VALUE_H_
#define SHAREINFO_CONST_GAME_VALUE_H_

namespace{
// �e��T�C�Y
const int SIZE_4	= 0x0004;
const int SIZE_8	= (SIZE_4  << 1);
const int SIZE_16	= (SIZE_8  << 1);
const int SIZE_32	= (SIZE_16 << 1);

const int SCREEN_WIDTH = 1024;	// ��ʕ�
const int SCREEN_HEIGHT = 640;	// ��ʍ���

// ��ʔԍ�
const int TITLE_SCENE	= 1;	// �^�C�g�����
const int PAUSE_SCENE	= 2;	// �|�[�Y���
const int HELP_SCENE	= 3;	// �w���v���
const int LOADING_SCENE = 4;	// ���[�f�B���O���
const int RANKING_SCENE = 5;	// �����L���O���

// �v���C���[�֘A
const int PLAYER_WIDHT	= 128;							// �e�N�X�`���T�C�Y�F��
const int PLAYER_HEIGHT = 48;							// �e�N�X�`���T�C�Y�F����
const int PLAYER_X_NUM	= 4;							// �摜���FX����
const int PLAYER_Y_NUM	= 1;							// �摜���FY����
const int PLAYER_RECT_W = PLAYER_WIDHT / PLAYER_X_NUM;	// ��`��
const int PLAYER_RECT_H = PLAYER_HEIGHT / PLAYER_Y_NUM;	// ��`����

// �v���C���[����
const unsigned int IS_MOVING	= (1 << 0);	// �ړ�
const unsigned int IS_JUMPING	= (1 << 1);	// �W�����v
const unsigned int IS_DASHING	= (1 << 2);	// �_�b�V��
const unsigned int IS_AIR		= (1 << 3);	// ��

// �}�b�v�`�b�v(����)
const int MAX_MAP_CHIP		= 192;		// �ő�}�b�v�`�b�v��
const int MAP_CHIP_X_NUM	= SIZE_8;	// �摜���FX����
const int MAP_CHIP_Y_NUM	= 24;		// �摜���FY����
const int MAP_CHIP_W		= SIZE_16;	// �摜��
const int MAP_CHIP_H		= SIZE_16;	// �摜����

// �ő呮�����
const int ATTRIBUTE_TYPE_RED	= 0;	// �����F��
const int ATTRIBUTE_TYPE_GREEN	= 1;	// �����F��
const int ATTRIBUTE_TYPE_BLUE	= 2;	// �����F��
const int MAX_ATTRIBUTE_TYPE	= 3;	// �ő呮����

// �o���A�֘A�萔
const float MAX_ENERGY = 100.0f;			// �ő�G�l���M�[
const int BARRIER_TOP	  = 0;				// �o���A�F��i
const int BARRIER_CURRENT = 1;				// �o���A�F�g�p��
const int BARRIER_BOTTOM  = 2;				// �o���A�F���i
const unsigned int CHANGE_ON	= 0x0001;	// �ؑ�ON
const unsigned int CHANGE_UP	= 0x0002;	// �ؑցF�����
const unsigned int CHANGE_DOWN	= 0x0004;	// �ؑցF������

// ���s�֘A�萔
const int JUDGE_EVEN = 0;	// ����F����
const int JUDGE_WIN	 = 1;	// ����F����
const int JUDGE_LOSE = 2;	// ����F����

// HP�֘A�萔
const int MAX_HIT_POINT = 3;	// �ő�q�b�g�|�C���g

// �Փˊ֘A�萔
const int NO_HIT		= 0;	// �q�b�g�Ȃ�
const int CORRECT_LEFT	= 1;	// ������
const int CORRECT_RIGHT	= 2;	// �E����
const int CORRECT_UP	= 3;	// �����
const int CORRECT_DOWN	= 4;	// ������

// �@�\�t���O
const unsigned int FUNCTION_TYPE_BARRIER  = (1 << 0);	// �o���A�@�\

// �A�C�e���t���O
const unsigned int ITEM_COIN = (1 << 0);	// �A�C�e���F�R�C��

// �f�[�^�ɃA�N�Z�X���邽�߂̓Y��
const unsigned int ACCESS_KEY_COIN		= 0;	// �R�C��
const unsigned int ACCESS_KEY_DEFEAT	= 1;	// �G���j��
const unsigned int MAX_ACCESS_KEY		= 2;	// �ő�A�N�Z�X�L�|

// �T�E���h�֘A�萔
const unsigned int SE_ID_BARRIER		= 0;
const unsigned int SE_ID_GUARD			= 1;
const unsigned int SE_ID_GAME_CLEAR		= 2;
const unsigned int SE_ID_GAME_OVER		= 3;
const unsigned int SE_ID_GET_COIN		= 4;
const unsigned int SE_ID_JUMP			= 5;
const unsigned int SE_ID_SHOOT			= 7;
const unsigned int SE_ID_MENU_DECIDE	= 8;
const unsigned int SE_ID_MENU_SELECT	= 9;
const unsigned int SE_ID_PAUSE_CLOSE	= 10;
const unsigned int SE_ID_PAUSE_OPEN		= 11;
const unsigned int SE_ID_DAMAGE			= 12;
const unsigned int SE_ID_DEFEAT			= 13;
const unsigned int SE_ID_BARRIER_SELECT = 14;

const unsigned int BGM_ID_TITLE		= 0;
const unsigned int BGM_ID_STAGE_1	= 1;
const unsigned int BGM_ID_STAGE_2	= 2;
const unsigned int BGM_ID_STAGE_3	= 3;
}
#endif // #ifndef SHAREINFO_CONST_GAME_VALUE_H_