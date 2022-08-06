//*************************************************************
// �Œ�X�e�[�W�N���X
//*************************************************************
#include "CFixedFloor.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Utility/CMath.h"

namespace GameObject
{
CFixedFloor::CFixedFloor()
: m_firstPosX(0), m_firstPosY(0)
{
	LoadDivGraph("resource/block_ds.png", MAX_MAP_CHIP, MAP_CHIP_X_NUM, MAP_CHIP_Y_NUM, MAP_CHIP_W, MAP_CHIP_H, m_gfxHdl);
}

CFixedFloor::~CFixedFloor()
{
	// �������ォ��폜����
	for(int i = 0; i < MAX_MAP_CHIP; i++){
		DeleteGraph(m_gfxHdl[i]);
	}
}

//****************************************************************************
// �֐����FInitialize
// �T�@�v�F����������
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�e�X�e�[�W���(CSV)�̓Ǎ��ƃf�[�^�ݒ�
//****************************************************************************
void CFixedFloor::Initialize(ShareInfo::CDocGameInfo& info)
{
	// �v�f�̏������ƃ������T�C�Y�����킹��
	m_stageDatas.clear();
	m_stageDatas.shrink_to_fit();

	// �X�e�[�W���擾
	int index = info.GetStageIndex();
	std::vector<std::string> fileList = {
		//"resource/stage/stage_layout_1.csv",
		//"resource/stage/stage_layout_2.csv",
		"resource/stage/stage_layout_3.csv"
	};
	m_stageDatas = tnl::LoadCsv(fileList[index].c_str());

	// �ő�X�e�[�W���ݒ�(0�I���W��)
	info.SetMaxStage(fileList.size() - 1);

	// �ő��ʐ�(�u���b�N���� / 1��ʂɕ\���ł���u���b�N��)
	int screen_max = m_stageDatas[0].size() / SIZE_32;
	info.SetScreenMax(screen_max);

	// �����u���b�N�z�u�ʒu
	m_firstPosX = SIZE_16 - ((m_stageDatas[0].size() / screen_max) * SIZE_32 * 0.5f);
 	m_firstPosY = SIZE_16 - (m_stageDatas.size() * SIZE_32 * 0.5f);
}

//****************************************************************************
// �֐����FCollision
// �T�@�v�F�Փ˔��菈��
// ���@���F��1�����@���݈ʒu
// �@�@�@�@��2�����@�O��ʒu
// �@�@�@�@��3�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�X�e�[�W�̏Փ˔��菈��
//****************************************************************************
void CFixedFloor::Collision(tnl::Vector3& current_pos, tnl::Vector3& prev_pos, ShareInfo::CDocGameInfo& info)
{
	// TODO�F1�}�X�_�b�V������
	//ShareInfo::S_VELOCITY_INFO velocity = info.GetVelocity();
	for(int i = 0; i < m_stageDatas.size(); i++){
		for(int j = 0; j < m_stageDatas[i].size(); j++){
			int stage_num = std::atoi(m_stageDatas[i][j].c_str());
			if(stage_num == 0){ continue; }

			// �e�`�b�v�̑��΍��W���v�Z
			int stage_pos_x = m_firstPosX + j * SIZE_32;
			int stage_pos_y = m_firstPosY + i * SIZE_32;
			tnl::Vector3 obj = tnl::Vector3(stage_pos_x, stage_pos_y, 0);

			// TODO�F1�}�X�_�b�V������
			//static int prev_corrent_tyep = 0;

			// �����蔻�菈��
			int correct_type = IsIntersectRectToCorrectPosition(current_pos, prev_pos, PLAYER_RECT_W, PLAYER_RECT_H, obj, (MAP_CHIP_W << 1), (MAP_CHIP_H << 1));
			if(correct_type != 0){
				info.SetIsCollision(true);
				info.SetCorrectType(ShareInfo::COLLISION_OBJECT_STAGE, correct_type);
				info.SetCurrentPos(current_pos);

				// TODO�F1�}�X�_�b�V������
				//if(prev_corrent_tyep == 3 && ((correct_type == 1) || (correct_type == 2))){
				//	tnl::DebugTrace("�Փ˒��I�I\n");
				//}
				//prev_corrent_tyep = correct_type;
			}
		}
	}
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�X�e�[�W�̕`�揈��
//****************************************************************************
void CFixedFloor::Draw(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;

	for(int i = 0; i < m_stageDatas.size(); i++){
		for(int j = 0; j < m_stageDatas[i].size(); j++){
			int imageID = std::atoi(m_stageDatas[i][j].c_str());
			if(imageID == 0){ continue; }

			// �e�`�b�v�̑��΍��W���v�Z
			int stage_pos_x = m_firstPosX + j * SIZE_32;
			int stage_pos_y = m_firstPosY + i * SIZE_32;

			// �`��p���W�ɕϊ�
			int view_stagePos_x = stage_pos_x - camera->GetPosition().x + screen_half_w;
			int view_stagePos_y = stage_pos_y - camera->GetPosition().y + screen_half_h;

			DrawRotaGraph(view_stagePos_x, view_stagePos_y, 2.0f, 0, m_gfxHdl[imageID - 1], true);
		}
	}
}

} // namespace GameObject