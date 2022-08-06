//*************************************************************
// �Œ�X�e�[�W�N���X
//*************************************************************
#include "CFixedFloor.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Utility/CMath.h"

namespace {
// ��`�T�C�Y
const int P_RECT_W = 20;
const int P_RECT_H = PLAYER_RECT_H;
const int RECT_W = MAP_CHIP_W << 1;
const int RECT_H = MAP_CHIP_H << 1;

// �n�`CSV�Ǎ��p
const std::vector<std::string> g_stageCsv = {
	//"resource/stage/stage_layout_1.csv",
	//"resource/stage/stage_layout_2.csv",
	"resource/stage/stage_layout_3.csv"
};

// �n�`����p
typedef std::vector<std::vector<std::string>> CSV_DATA;
CSV_DATA g_stageData;

// �n�`�摜�n���h��
int g_gfxHdl[MAX_MAP_CHIP] = {0};
}

namespace GameObject
{
CFixedFloor::CFixedFloor()
: m_firstPosX(0), m_firstPosY(0)
{
	LoadDivGraph("resource/block_ds.png", MAX_MAP_CHIP, MAP_CHIP_X_NUM, MAP_CHIP_Y_NUM, MAP_CHIP_W, MAP_CHIP_H, g_gfxHdl);
}

CFixedFloor::~CFixedFloor()
{
	// �������ォ��폜����
	for(int i = 0; i < MAX_MAP_CHIP; i++){
		DeleteGraph(g_gfxHdl[i]);
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
	g_stageData.clear();
	g_stageData.shrink_to_fit();

	// �X�e�[�W���擾
	int index = info.GetStageIndex();
	g_stageData = tnl::LoadCsv(g_stageCsv[index].c_str());

	// �ő�X�e�[�W���ݒ�(0�I���W��)
	info.SetMaxStage(g_stageCsv.size() - 1);

	// �ő��ʐ�(�u���b�N���� / 1��ʂɕ\���ł���u���b�N��)
	int screen_max = g_stageData[0].size() / SIZE_32;
	info.SetScreenMax(screen_max);

	// �����u���b�N�z�u�ʒu
	m_firstPosX = SIZE_16 - ((g_stageData[0].size() / screen_max) * SIZE_32 * 0.5f);
 	m_firstPosY = SIZE_16 - (g_stageData.size() * SIZE_32 * 0.5f);
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
	for(int i = 0; i < g_stageData.size(); i++){
		for(int j = 0; j < g_stageData[i].size(); j++){
			int stage_num = std::atoi(g_stageData[i][j].c_str());
			if(stage_num == 0){ continue; }

			// �e�`�b�v�̑��΍��W���v�Z
			int stage_pos_x = m_firstPosX + j * SIZE_32;
			int stage_pos_y = m_firstPosY + i * SIZE_32;
			tnl::Vector3 obj = tnl::Vector3(stage_pos_x, stage_pos_y, 0);

			// TODO�F1�}�X�_�b�V������
			//static int prev_corrent_tyep = 0;

			// �����蔻�菈��
			int correct_type = IsIntersectRectToCorrectPosition(current_pos, prev_pos, P_RECT_W, P_RECT_H, obj, RECT_W, RECT_H);
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

	for(int i = 0; i < g_stageData.size(); i++){
		for(int j = 0; j < g_stageData[i].size(); j++){
			int imageID = std::atoi(g_stageData[i][j].c_str());
			if(imageID == 0){ continue; }

			// �e�`�b�v�̑��΍��W���v�Z
			int stage_pos_x = m_firstPosX + j * SIZE_32;
			int stage_pos_y = m_firstPosY + i * SIZE_32;

			// �`��p���W�ɕϊ�
			int view_stagePos_x = stage_pos_x - camera->GetPosition().x + screen_half_w;
			int view_stagePos_y = stage_pos_y - camera->GetPosition().y + screen_half_h;

			DrawRotaGraph(view_stagePos_x, view_stagePos_y, 2.0f, 0, g_gfxHdl[imageID - 1], true);
		}
	}
}

} // namespace GameObject