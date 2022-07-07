//**************************************
// �{�X�I�u�W�F�N�g
// TODO�F�����~�X�����̂Œe����������
//**************************************
#include "CBossObject.h"

#include "../Camera/CCamera2D.h"
#include "../Utility/CMath.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../../dxlib_ext/dxlib_ext.h"

namespace{
struct LOAD_SPRITE_INFO{
	std::string file_path;
	int all_num;
	int x_num;
	int y_num;
	int x_size;
	int y_size;
};

const std::vector<LOAD_SPRITE_INFO> sprites = {
	{"resource/enemy/dragon_idle.png", 4, 4, 1, 230, 150},
	{"resource/enemy/dragon_move.png", 6, 6, 1, 230, 150},
	{"resource/enemy/dragon_attack.png", 5, 5, 1, 230, 150}
};

const int BOSS_ACTION_IDLE = 0;
const int BOSS_ACTION_MOVE = 1;
const int BOSS_ACTION_ATTACK = 2;
const int MAX_BOSS_ACTION = 3;

const int anim_frames[MAX_BOSS_ACTION] = {
	sprites[BOSS_ACTION_IDLE].x_num,
	sprites[BOSS_ACTION_MOVE].x_num,
	sprites[BOSS_ACTION_ATTACK].x_num
};

const int BOSS_ANIM_FRAME_MAX = 6;
int prev_state = BOSS_ACTION_IDLE, state = BOSS_ACTION_IDLE;

int anim_gfx_hdl[MAX_BOSS_ACTION][BOSS_ANIM_FRAME_MAX] = {0};

float anim_time_count = 0;
int anim_type = 0, anim_frame = 0;
int reverse_flag = 0;
float change_elapsed = 0.0f;
tnl::Vector3 boss_pos;
}

namespace GameObject
{
CBossObject::CBossObject()
{}
	
CBossObject::~CBossObject()
{}

void CBossObject::Initialize(ShareInfo::CDocGameInfo& info)
{
	//boss_pos.x = 4976.0f - (info.GetScreenWidth() >> 1);
	//boss_pos.y = 1416.0f - (info.GetScreenHeight() >> 1);
	boss_pos.x = 50.0 - (info.GetScreenWidth() >> 1);
	boss_pos.y = 1416.0f - (info.GetScreenHeight() >> 1);
	for(int i = 0; i < sprites.size(); i++){
		LoadDivGraph(sprites[i].file_path.c_str(), sprites[i].all_num, sprites[i].x_num, sprites[i].y_num, sprites[i].x_size, sprites[i].y_size, anim_gfx_hdl[i]);
	}
}
	
void CBossObject::Collision(tnl::Vector3&, tnl::Vector3&, ShareInfo::CDocGameInfo&)
{}
	
void CBossObject::Update(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 pos = info.GetCurrentPos();
	float delta_time = info.GetDeltaTime();

	// �v���C���[�̕������擾
	int direction = tnl::GetRegionPointAndRect(pos, boss_pos, 250, 150);

	// �v���C���[�̕����ɔ��]
	switch(direction)
	{
	case 3:
		reverse_flag = 0;
		break;
	case 1:
		reverse_flag = 1;
		break;
	}

	// �e��Ԃ̓�����s��
	switch(state)
	{
	case BOSS_ACTION_IDLE:
		state = PriBossAction_Idle(pos, direction, delta_time);
		break;
	case BOSS_ACTION_MOVE:
		state = PriBossAction_Move(pos, direction, delta_time);
		break;
	case BOSS_ACTION_ATTACK:
		state = PriBossAction_Attack();
		break;
	default:
		break;
	}
	// ��Ԃ��A�j���^�C�v�ɓ����
	anim_type = state;

	// �O��̏�ԂƈقȂ��Ă���΃t���[������߂�
	if(prev_state != state){ anim_frame = 0; }
	prev_state = state;

	// �A�j���[�V����
	anim_time_count += delta_time;
	if(anim_time_count > 0.15f){
		anim_time_count = 0.0f;
		anim_frame++;
		anim_frame %= anim_frames[anim_type];
	}
}

void CBossObject::Draw(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;

	int view_pos_x = boss_pos.x - camera->GetPosition().x + screen_half_w;
	int view_pos_y = boss_pos.y - camera->GetPosition().y + screen_half_h;
	DrawRotaGraph(view_pos_x, view_pos_y, 0.75f, 0, anim_gfx_hdl[anim_type][anim_frame], true, reverse_flag);
}

// �{�X����(�ҋ@)
int CBossObject::PriBossAction_Idle(tnl::Vector3& pos, int direction, float delta_time)
{
	int next_state = BOSS_ACTION_IDLE;
	float distance = Utility::GetDistance2D(boss_pos, pos);
	DrawStringEx(100, 100, -1, "Distance : %f", distance);

	change_elapsed += delta_time;
	if(change_elapsed > 2.0f){
		change_elapsed = 0.0f;
		if(distance >= 300){
			// ��苗������Ă���ꍇ�F�ړ�
			next_state = BOSS_ACTION_MOVE;
		}
		else{
			// ��苗���߂Â��Ă���ꍇ�F�U��
			next_state = BOSS_ACTION_ATTACK;
		}
	}

	return next_state;
}

// �{�X����(�ړ�)
int CBossObject::PriBossAction_Move(tnl::Vector3& pos, int direction, float)
{
	int next_state = BOSS_ACTION_MOVE;

	// �v���C���[�̕������擾
	switch(direction)
	{
		// ������
	case 3:
		boss_pos.x += -1;
		break;
		// �E����
	case 1:
		boss_pos.x += 1;
		break;
	}
	float distance = Utility::GetDistance2D(boss_pos, pos);
	if(distance < 250){ next_state = BOSS_ACTION_IDLE; }
	// ��苗���߂Â��Ă���ꍇ�F�U��


	return next_state;
}

// �{�X����(�U��)
int CBossObject::PriBossAction_Attack()
{
	int next_state = BOSS_ACTION_ATTACK;

	// 1���[�v������ҋ@���[�h�ɖ߂�
	if(anim_frame == (anim_frames[anim_type] - 1)){
		next_state = BOSS_ACTION_IDLE;
	}

	return next_state;
}

} // namespace GameObject