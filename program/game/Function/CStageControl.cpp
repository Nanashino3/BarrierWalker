#include "CStageControl.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace Function
{
CStageControl::CStageControl(unsigned int type)
: AFunction(type)
{}

CStageControl::~CStageControl()
{}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�X�e�[�W�X�V�ƍŏI�X�e�[�W����
//****************************************************************************
void CStageControl::Update(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camPos = info.GetCamera()->GetPosition();
	tnl::Vector3 pos = info.GetCurrentPos();
	
	// �X�N���[����(����)
	int screenHalfW = info.GetScreenWidth() >> 1;
	// �v���C���[�̋�`��(����)
	int rectHalfW = PLAYER_RECT_W >> 1;
	// ���݂̃X�e�[�W�ԍ��擾
	int stage_index = info.GetStageIndex();
	// �`����W�ɕϊ�
	int playerPosX = pos.x - camPos.x + screenHalfW;

	// �ȉ��̃A�N�V�������쒆���m�F
	bool isJump = (info.GetActionType() & IS_JUMPING) ? true : false;
	bool isAir = (info.GetActionType() & IS_AIR) ? true : false;

	if(!info.GetIsClear() && !isJump && !isAir){
		if((playerPosX == info.GetScreenWidth() - rectHalfW)){
			// �Ƃ肠�����X�e�[�W�̓N���A
			info.SetIsClear(true);
			if (stage_index == info.GetMaxStage()) {
				info.SetIsFinalStage(true);
			}
			else {
				stage_index++;
			}
		}	
	}
	info.SetStageIndex(stage_index);
}

} // namespace Function