//*************************************************************
// �o���A�@�\�N���X
//*************************************************************
#include "CBarrier.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../ShareInfo/S_BARRIER_INFO.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"
#include "../Sound/CSoundManager.h"

namespace{
// �A�j���[�V�����֘A
const int ANIM_FRAME_MAX = 10;
// �摜�Ǎ��p�萔
const int ALL_NUM = 10;
const int X_NUM	  = 5;
const int Y_NUM   = 2;
const int X_SIZE  = 600 / X_NUM;
const int Y_SIZE  = 240 / Y_NUM;
// �g�p�摜�ݒ�
const std::vector<std::string> g_images = {
	"resource/pipo-btleffect111g.png",	// ��
	"resource/pipo-btleffect111d.png",	// ��
	"resource/pipo-btleffect111e.png"	// ��
};

int g_gfxHdl[MAX_ATTRIBUTE_TYPE][ANIM_FRAME_MAX] = {0};
std::vector<ShareInfo::S_BARRIER_INFO> g_barriers;
}

namespace Function
{
CBarrier::CBarrier(unsigned int function_type)
: AFunction(function_type)
, m_animType(BARRIER_CURRENT)
, m_animFrame(0)
, m_soundManager(nullptr)
{}

CBarrier::~CBarrier()
{
	for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
		for(int j = 0; j < ANIM_FRAME_MAX; j++){
			DeleteGraph(g_gfxHdl[i][j]);
		}
	}
}

//****************************************************************************
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F�Ȃ�
// �߂�l�F�Ȃ�
// �ځ@�ׁF�摜�̃��[�h�Ə��̏����ݒ�
//****************************************************************************
void CBarrier::Initialize()
{
	for (int i = 0; i < g_images.size(); i++) {
		// �摜�̃��[�h
		LoadDivGraph(g_images[i].c_str(), ALL_NUM, X_NUM, Y_NUM, X_SIZE, Y_SIZE, g_gfxHdl[i]);
	}
	m_soundManager = Sound::CSoundManager::GetInstance();

	g_barriers.clear();
	g_barriers.shrink_to_fit();
	for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
		// �o���A���ݒ�
		ShareInfo::S_BARRIER_INFO barrier = { i, MAX_ENERGY, false, false };
		g_barriers.push_back(barrier);
	}
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�o���A�W�J��G�l���M�[�����A���ւ�������
//****************************************************************************
void CBarrier::Update(ShareInfo::CDocGameInfo& info)
{
	unsigned int function_type = info.GetFunctionType();
	unsigned int change_type = info.GetChangeType();

	// �o���A�I��
	bool isChangeBarrier = false;
	int next = -1, prev = -1;
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_W) && !(change_type & CHANGE_ON)){
		isChangeBarrier = true;
		change_type |= CHANGE_UP;
		prev = BARRIER_TOP;
		next = BARRIER_BOTTOM;
	}else if(tnl::Input::IsKeyDownTrigger(eKeys::KB_S) && !(change_type & CHANGE_ON)){
		isChangeBarrier = true;
		change_type |= CHANGE_DOWN;
		prev = BARRIER_BOTTOM;
		next = BARRIER_TOP;
	}

	// �o���A����ւ�
	if(isChangeBarrier){ PriSwap(change_type, function_type, next, prev); }

	// �o���A�W�J and �o���A����ł͂Ȃ�
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_B) && !g_barriers[BARRIER_CURRENT].isEmpty){
		if(function_type & m_function_type){
			// �o���AOFF
			function_type &= ~m_function_type;
			g_barriers[BARRIER_CURRENT].isBarrier = false;
			m_soundManager->StopSE(SE_ID_BARRIER);
		}else{
			// �o���AON
			function_type |= m_function_type;
			g_barriers[BARRIER_CURRENT].isBarrier = true;
			m_soundManager->PlaySE(SE_ID_BARRIER);
		}
	}

	// �o���A��������
	for(int count = 0; count < g_barriers.size(); count++){
		// �o���A�ؑ֒��ł͂Ȃ�
		if(!(change_type & CHANGE_ON)){
			// ��������
			PriFluctuation(function_type, count, info);
		}
		// �䗦�v�Z��UI���ւ������̂��߂ɐؑւ̗L���ɂ�����炸�ݒ肷��
		info.SetBarrierEnergy(count, g_barriers[count].energy);
		info.SetBarrierColor(count, g_barriers[count].color);
	}
	info.SetChangeType(change_type);
	info.SetFunctionType(function_type);
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�o���A�̕`��
//****************************************************************************
void CBarrier::Draw(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 pos = info.GetCurrentPos();
	Camera::CCamera2D* camera = info.GetCamera();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;

	unsigned int change_type = info.GetChangeType();
	for(int i = 0; i < g_barriers.size(); i++){
		// �o���A�ؑ֒� or �o���A��W�J���Ă��Ȃ��ꍇ�͕\�����Ȃ�
		if((change_type & CHANGE_ON) || !g_barriers[i].isBarrier){ continue; }
		int view_pos_x = pos.x - camera->GetPosition().x + screen_half_w;
		int view_pos_y = pos.y - camera->GetPosition().y + screen_half_h;
		DrawRotaGraph(view_pos_x, view_pos_y, 1.0f, 0, g_gfxHdl[m_animType][m_animFrame], true);
	}
}

//****************************************************************************
// �֐����FPriSwap
// �T�@�v�F���ւ�����(Private)
// ���@���F��1�����@�ؑ֎�(�o���A��ON/OFF��㉺�ǂ���ɐؑւ��邩�̏��)
// �@�@�@�@��2�����@�@�\��(����ON�ɂȂ��Ă���@�\)
// �@�@�@�@��3�����@���ɓ��ւ���o���A
// �@�@�@�@��4�����@�O�ɓ��ւ��Ă���͂��̃o���A
// �߂�l�F�Ȃ�
// �ځ@�ׁF�o���A���̓��ւ�����
//****************************************************************************
void CBarrier::PriSwap(unsigned int& change_type, unsigned int& function_type, int next, int prev)
{
	// ���ւ����Ƀo���A��OFF����
	function_type &= ~m_function_type;
	m_soundManager->StopSE(SE_ID_BARRIER);
	m_soundManager->PlaySE(SE_ID_BARRIER_SELECT);

	// �ؑփt���O�𗧂Ă�
	change_type |= CHANGE_ON;
	g_barriers[BARRIER_CURRENT].isBarrier = false;

	ShareInfo::S_BARRIER_INFO temp = g_barriers[BARRIER_CURRENT];
	g_barriers[BARRIER_CURRENT] = g_barriers[next];
	g_barriers[next] = g_barriers[prev];
	g_barriers[prev] = temp;

	// ���݂̐F�ɃA�j���[�V������ʂ����킹��
	m_animType = g_barriers[BARRIER_CURRENT].color;
}

//****************************************************************************
// �֐����FPriFluctuation
// �T�@�v�F��������(Private)
// ���@���F��1�����@�@�\��(����ON�ɂȂ��Ă���@�\)
// �@�@�@�@��2�����@�o���A�z��ɃA�N�Z�X���邽�߂̔ԍ�
// �@�@�@�@��3�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e�����o���A�̃G�l���M�|��������
//****************************************************************************
void CBarrier::PriFluctuation(unsigned int& function_type, int num, ShareInfo::CDocGameInfo& info)
{
	if(g_barriers[num].isBarrier){
		g_barriers[num].energy = (g_barriers[num].energy > 0.0f) ? g_barriers[num].energy - 0.35f : 0.0f;
		// �o���A�G�l���M�[����
		if(g_barriers[num].energy == 0.0f){
			g_barriers[num].isEmpty = true;
			g_barriers[num].isBarrier = false;
			// �o���A�@�\��OFF����
			if(m_animType == g_barriers[num].color){ function_type &= ~m_function_type; }
		}

		// �A�j���[�V��������
		static float anim_time_count = 0.0f;
		anim_time_count += info.GetDeltaTime();
		if(anim_time_count > 0.05f){
			anim_time_count = 0.0f;
			m_animFrame++;
			m_animFrame %= (ANIM_FRAME_MAX / 2);
		}
	}else{
		// �o���A���g�p or �o���A���g���؂����ꍇ
		g_barriers[num].energy = (g_barriers[num].energy < MAX_ENERGY) ? g_barriers[num].energy + 0.1f : MAX_ENERGY;
		if(g_barriers[num].energy == MAX_ENERGY){ g_barriers[num].isEmpty = false; }
	}
}

} // namespace Function