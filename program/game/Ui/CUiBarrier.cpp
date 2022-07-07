//*************************************************************
// �o���A�Q�[�WUI�N���X
//*************************************************************
#include "CUiBarrier.h"

#include "../Utility/CImageManager.h"
#include "../ShareInfo/S_DRAW_INFO.h"
#include "../ShareInfo/S_DRAW_INFO_3F.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace{
// �o���AUI�摜
const std::vector<std::string> g_barrierGauge = {
	"resource/ui/HPGauge02_red.png",	// �ԃQ�[�W
	"resource/ui/HPGauge02_green.png",	// �΃Q�[�W
	"resource/ui/HPGauge02_blue.png"	// �Q�[�W
};
// �w�iUI�p�ݒ�l
const std::vector<ShareInfo::S_DRAW_INFO> g_bgUi = {
	{192,  82, 0.75f, 0.0f, 0, true},	// top
	{192, 100, 1.0f , 0.0f, 0, true},	// current
	{192, 118, 0.75f, 0.0f, 0, true},	// bottom
};
// �o���A�Q�[�W�p�ݒ�l
const std::vector<ShareInfo::S_DRAW_INFO_3F> g_ui = {
	{99,  82, 0, 6, 0.75f, 0.75f, 0.0f, 0, true},	// top
	{67, 100, 0, 6, 1.0f , 1.0f , 0.0f, 0, true},	// current
	{99, 118, 0, 6, 0.75f, 0.75f, 0.0f, 0, true},	// bottom
};
// �I�t�Z�b�g�p�ݒ�l
const int UI_OFFSET_X = 32;
const int UI_OFFSET_Y = 18;
const float UI_RATIO_OFFSET_X = 0.25f;
// �A�j���[�V�������
struct ANIMATION_INFO
{
	int sign;	// ����
	int dx;		// X����(1�F�E�A-1�F��)
	int dy;		// Y����(1�F���A-1�F��)
};
int g_gfxHdl[MAX_ATTRIBUTE_TYPE] = {0};
}

namespace Ui
{
CUiBarrier::CUiBarrier()
: m_bg_gfx_hdl(0)
, m_offset_x(0), m_offset_y(0)
, m_ratio(0)
{
	m_bg_gfx_hdl = Utility::LoadGraphEx("resource/ui/HPGauge02_bg_01.png");
	for(unsigned int i = 0; i < g_barrierGauge.size(); i++){
		g_gfxHdl[i] = Utility::LoadGraphEx(g_barrierGauge[i].c_str());
	}
}

CUiBarrier::~CUiBarrier()
{
	Utility::DeleteGraphEx(m_bg_gfx_hdl);
	for(unsigned int i = 0; i < g_barrierGauge.size(); i++){
		Utility::DeleteGraphEx(g_gfxHdl[i]);
	}
}

//****************************************************************************
// �֐����FUpdate
// �T�@�v�F�X�V����
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁFUI�̃A�j���[�V�������Ɏg�p����ϐ��̍X�V����
//****************************************************************************
void CUiBarrier::Update(ShareInfo::CDocGameInfo& info)
{
	unsigned int change_type = info.GetChangeType();
	if(!(change_type & CHANGE_ON)){ return; }

	// UI�A�j���[�V�����p�I�t�Z�b�g�v�Z
	m_offset_x = (m_offset_x < UI_OFFSET_X) ? m_offset_x += 1 : UI_OFFSET_X;
	m_offset_y = (m_offset_y < UI_OFFSET_Y) ? m_offset_y += 1 : UI_OFFSET_Y;
	m_ratio = (m_ratio < UI_RATIO_OFFSET_X) ? m_ratio += 0.0078f : UI_RATIO_OFFSET_X;
	if(m_ratio == UI_RATIO_OFFSET_X && m_offset_x == UI_OFFSET_X && m_offset_y == UI_OFFSET_Y){
		change_type = 0;
		info.SetChangeType(change_type);
	}
}

//****************************************************************************
// �֐����FDraw
// �T�@�v�F�`�揈��
// ���@���F��1�����@�Q�[�����
// �߂�l�F�Ȃ�
// �ځ@�ׁF�o���AUI�̕`�揈��
//****************************************************************************
void CUiBarrier::Draw(ShareInfo::CDocGameInfo& info)
{
	// �o���A�䗦
	float ratios[] = {
		{info.GetBarrierEnergy(BARRIER_TOP) / MAX_ENERGY},
		{info.GetBarrierEnergy(BARRIER_CURRENT) / MAX_ENERGY},
		{info.GetBarrierEnergy(BARRIER_BOTTOM) / MAX_ENERGY}
	};
	// �o���A�̐F
	int colors[] = {
		{info.GetBarrierColor(BARRIER_TOP)},
		{info.GetBarrierColor(BARRIER_CURRENT)},
		{info.GetBarrierColor(BARRIER_BOTTOM)}
	};

	//*************************************
	// �����ł�肽�����Ƃ͈ȉ�
	// �E���ւ����������Ă��Ȃ��ꍇ�͌Œ�`��
	// �E���ւ�ON�ɂȂ����ꍇ�A�Œ�`�����U��\���ɂ��A�j���[�V�����p�`����s��
	// �@�A�j���[�V�������I��������A�Œ�`��ɖ߂�
	//**************************************
	unsigned int change_type = info.GetChangeType();
	if(change_type & CHANGE_ON){
		// �A�j���[�V����
		PriDrawAnimation(change_type, ratios, colors);
	}else{
		m_ratio = 0.0f;
		m_offset_x = 0, m_offset_y = 0;
		for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
			// �w�i�\��
			DrawRotaGraph(g_bgUi[i].x, g_bgUi[i].y, g_bgUi[i].extRate, g_bgUi[i].angle, m_bg_gfx_hdl, g_bgUi[i].transFlag);
			// �o���A�Q�[�W�\��
			double extRateX = (double)(ratios[i] * g_ui[i].extRateX);
			DrawRotaGraph3F(g_ui[i].xf, g_ui[i].yf, g_ui[i].cxf, g_ui[i].cyf, extRateX, g_ui[i].extRateY, g_ui[i].angle, g_gfxHdl[colors[i]], g_ui[i].transFlag);
		}
	}
}

//****************************************************************************
// �֐����FPriDrawAnimation
// �T�@�v�F�A�j���[�V����(Private)
// ���@���F��1�����@�ؑ֎�(�o���A��ON/OFF��㉺�ǂ���ɐؑւ��邩�̏��)
// �@�@�@�@��2�����@�䗦�z��
// �@�@�@�@��3�����@�F�z��
// �߂�l�F�Ȃ�
// �ځ@�ׁF�o���A�ؑփA�j���[�V����
//****************************************************************************
void CUiBarrier::PriDrawAnimation(unsigned int change_type, float ratios[], int colors[])
{
	unsigned int type = change_type & ~CHANGE_ON;
	switch(type)
	{
	// ������ɐؑփA�j���[�V����
	case CHANGE_UP:
		{
			// �o���A�֑ؑO�̏���ݒ�
			float prev_ratios[] = {
				{ratios[BARRIER_BOTTOM]}, {ratios[BARRIER_TOP]}, {ratios[BARRIER_CURRENT]}
			};
			int prev_colors[] = {
				{colors[BARRIER_BOTTOM]}, {colors[BARRIER_TOP]}, {colors[BARRIER_CURRENT]}
			};
			ANIMATION_INFO anim_info[] = {
				{ 0,  0,  2}, {-1,  1, -1}, { 1, -1, -1}
			};
			// �ؑւ�������ɂ���ĕ`�揇��ؑւ���
			for(int cnt = 0; cnt < MAX_ATTRIBUTE_TYPE; cnt++){
				PriDrawAnimationImpl(cnt, anim_info[cnt].sign, anim_info[cnt].dx, anim_info[cnt].dy, prev_ratios[cnt], prev_colors[cnt]);
			}
		}
		break;
	// �������ɐؑփA�j���[�V����
	case CHANGE_DOWN:
		{
			// �o���A�֑ؑO�̏���ݒ�
			float prev_ratios[] = {
				{ratios[BARRIER_CURRENT]}, {ratios[BARRIER_BOTTOM]}, {ratios[BARRIER_TOP]}
			};
			int prev_colors[] = {
				{colors[BARRIER_CURRENT]}, {colors[BARRIER_BOTTOM]}, {colors[BARRIER_TOP]}
			};
			ANIMATION_INFO anim_info[] = {
				{ 1, -1,  1}, {-1,  1,  1},	{ 0,  0, -2}
			};
			
			// �ؑւ�������ɂ���ĕ`�揇��ؑւ���
			for(int cnt = MAX_ATTRIBUTE_TYPE - 1; cnt >= 0; cnt--){
				PriDrawAnimationImpl(cnt, anim_info[cnt].sign, anim_info[cnt].dx, anim_info[cnt].dy, prev_ratios[cnt], prev_colors[cnt]);
			}
		}
		break;
	default:
		break;
	}
	return;
}

//****************************************************************************
// �֐����FPriDrawAnimationImpl
// �T�@�v�F�A�j���[�V�����p�`��(Private)
// ���@���F��1�����@�z��ɃA�N�Z�X���邽�߂̓Y��
// �@�@�@�@��2�����@����(�g��k���̕����Ɋւ��)
// �@�@�@�@��3�����@X�����̈ړ��l
// �@�@�@�@��4�����@Y�����̈ړ��l
// �@�@�@�@��5�����@���ւ��O�̔䗦
// �@�@�@�@��6�����@���ւ��O�̐F
// �߂�l�F�Ȃ�
// �ځ@�ׁF�A�j���[�V�����p�`�揈��
//****************************************************************************
void CUiBarrier::PriDrawAnimationImpl(int num, int sign, int dx, int dy, float prev_ratio, int prev_color)
{
	// �w�iUI�`��
	int y = g_bgUi[num].y + m_offset_y * dy;
	float exRate = (float)(g_bgUi[num].extRate + m_ratio * sign);
	DrawRotaGraph(g_bgUi[num].x, y, exRate, g_bgUi[num].angle, m_bg_gfx_hdl, true);

	// �o���AUI�`��
	float xf = g_ui[num].xf + m_offset_x * dx;
	float yf = g_ui[num].yf + m_offset_y * dy;

	double extRateX = (double)(prev_ratio * (g_ui[num].extRateX + m_ratio * sign));
	double extRateY = (double)(g_ui[num].extRateY + m_ratio * sign);
	DrawRotaGraph3F(xf, yf, g_ui[num].cxf, g_ui[num].cyf, extRateX, extRateY, g_ui[num].angle, g_gfxHdl[prev_color], true);

	return;
}

} // namespace Ui