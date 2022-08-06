//****************************************************************************
// �t�@�C�����FCBarrier(�o���A)
// ��@���@���F2022/08/06
#include "CBarrier.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"

namespace Function
{
CBarrier::CBarrier(unsigned int function_type)
: AFunction(function_type)
, m_animType(BARRIER_CURRENT)
, m_animFrame(0)
, m_soundManager(nullptr)
{
	std::vector<std::string> fileList = {
		"resource/pipo-btleffect111g.png",	// ��
		"resource/pipo-btleffect111d.png",	// ��
		"resource/pipo-btleffect111e.png"	// ��
	};
	for(const auto& file : fileList){
		m_imageList.push_back(file.c_str());
	}
}

CBarrier::~CBarrier()
{
	for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
		for(int j = 0; j < MAX_BARRIER_ANIM_FRAME; j++){
			DeleteGraph(m_gfxHdl[i][j]);
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
	// �摜�̃��[�h
	for (int i = 0; i < m_imageList.size(); i++) {
		LoadDivGraph(m_imageList[i].c_str(),
					 MAX_BARRIER_IMAGE_NUM,
					 BARRIER_IMAGE_NUMX, BARRIER_IMAGE_NUMX,
					 BARRIER_IMAGE_SIZEX, BARRIER_IMAGE_SIZEY,
					 m_gfxHdl[i]);
	}
	m_soundManager = Sound::CSoundManager::GetInstance();

	m_barriers.clear();
	m_barriers.shrink_to_fit();
	for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
		// �o���A���ݒ�
		ShareInfo::S_BARRIER_INFO barrier = { i, MAX_ENERGY, false, false };
		m_barriers.push_back(barrier);
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
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_B) && !m_barriers[BARRIER_CURRENT].isEmpty){
		if(function_type & m_function_type){
			// �o���AOFF
			function_type &= ~m_function_type;
			m_barriers[BARRIER_CURRENT].isBarrier = false;
			m_soundManager->StopSE(SE_ID_BARRIER);
		}else{
			// �o���AON
			function_type |= m_function_type;
			m_barriers[BARRIER_CURRENT].isBarrier = true;
			m_soundManager->PlaySE(SE_ID_BARRIER);
		}
	}

	// �o���A��������
	for(int count = 0; count < m_barriers.size(); count++){
		// �o���A�ؑ֒��ł͂Ȃ�
		if(!(change_type & CHANGE_ON)){
			// ��������
			PriFluctuation(function_type, count, info);
		}
		// �䗦�v�Z��UI���ւ������̂��߂ɐؑւ̗L���ɂ�����炸�ݒ肷��
		info.SetBarrierEnergy(count, m_barriers[count].energy);
		info.SetBarrierColor(count, m_barriers[count].color);
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
	for(int i = 0; i < m_barriers.size(); i++){
		// �o���A�ؑ֒� or �o���A��W�J���Ă��Ȃ��ꍇ�͕\�����Ȃ�
		if((change_type & CHANGE_ON) || !m_barriers[i].isBarrier){ continue; }
		int view_pos_x = pos.x - camera->GetPosition().x + screen_half_w;
		int view_pos_y = pos.y - camera->GetPosition().y + screen_half_h;
		DrawRotaGraph(view_pos_x, view_pos_y, 1.0f, 0, m_gfxHdl[m_animType][m_animFrame], true);
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
	m_barriers[BARRIER_CURRENT].isBarrier = false;

	ShareInfo::S_BARRIER_INFO temp = m_barriers[BARRIER_CURRENT];
	m_barriers[BARRIER_CURRENT] = m_barriers[next];
	m_barriers[next] = m_barriers[prev];
	m_barriers[prev] = temp;

	// ���݂̐F�ɃA�j���[�V������ʂ����킹��
	m_animType = m_barriers[BARRIER_CURRENT].color;
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
	if(m_barriers[num].isBarrier){
		m_barriers[num].energy = (m_barriers[num].energy > 0.0f) ? m_barriers[num].energy - 0.35f : 0.0f;
		// �o���A�G�l���M�[����
		if(m_barriers[num].energy == 0.0f){
			m_barriers[num].isEmpty = true;
			m_barriers[num].isBarrier = false;
			// �o���A�@�\��OFF����
			if(m_animType == m_barriers[num].color){ function_type &= ~m_function_type; }
		}

		// �A�j���[�V��������
		static float anim_time_count = 0.0f;
		anim_time_count += info.GetDeltaTime();
		if(anim_time_count > 0.05f){
			anim_time_count = 0.0f;
			m_animFrame++;
			m_animFrame %= (MAX_BARRIER_ANIM_FRAME / 2);
		}
	}else{
		// �o���A���g�p or �o���A���g���؂����ꍇ
		m_barriers[num].energy = (m_barriers[num].energy < MAX_ENERGY) ? m_barriers[num].energy + 0.1f : MAX_ENERGY;
		if(m_barriers[num].energy == MAX_ENERGY){ m_barriers[num].isEmpty = false; }
	}
}

} // namespace Function