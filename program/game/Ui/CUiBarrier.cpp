//****************************************************************************
// �t�@�C�����FCUiBarrier(�o���AUI)
// ��@���@���F2022/08/06
#include "CUiBarrier.h"
#include "CONST_UI_VALUE.h"
#include "S_ANIMATION_INFO.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace Ui
{
CUiBarrier::CUiBarrier()
: m_bgGfxHdl(0)
, m_offsetX(0), m_offsetY(0)
, m_ratio(0)
{
	// �w�iUI�p�ݒ�l
	std::vector<ShareInfo::S_DRAW_INFO> bgUiList = {
		{192,  82, 0.75f, 0.0f, 0, true},	// top
		{192, 100, 1.0f , 0.0f, 0, true},	// current
		{192, 118, 0.75f, 0.0f, 0, true},	// bottom
	};
	for(const auto& bgUi : bgUiList){
		m_bgUiList.push_back(bgUi);
	}

	// �o���A�Q�[�W�p�ݒ�l
	std::vector<ShareInfo::S_DRAW_INFO_3F> gaugeList = {
		{99,  82, 0, 6, 0.75f, 0.75f, 0.0f, 0, true},	// top
		{67, 100, 0, 6, 1.0f , 1.0f , 0.0f, 0, true},	// current
		{99, 118, 0, 6, 0.75f, 0.75f, 0.0f, 0, true},	// bottom
	};
	for(const auto& gauge : gaugeList){
		m_gaugeList.push_back(gauge);
	}

	// �摜�ǂݍ���
	m_bgGfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx("resource/ui/HPGauge02_bg_01.png");
	std::vector<std::string> fileList = {
		"resource/ui/HPGauge02_red.png",	// �ԃQ�[�W
		"resource/ui/HPGauge02_green.png",	// �΃Q�[�W
		"resource/ui/HPGauge02_blue.png"	// �Q�[�W	
	};	
	for(const auto& file : fileList){
		int gfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx(file.c_str());
		m_gfxHdList.push_back(gfxHdl);
	}
}

CUiBarrier::~CUiBarrier()
{
	Utility::CImageManager::GetInstance()->DeleteGraphEx(m_bgGfxHdl);
	for(const auto& gfxHdl : m_gfxHdList){
		Utility::CImageManager::GetInstance()->DeleteGraphEx(gfxHdl);
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
	unsigned int changeType = info.GetChangeType();
	if(!(changeType & CHANGE_ON)){ return; }

	// UI�A�j���[�V�����p�I�t�Z�b�g�v�Z
	m_offsetX = (m_offsetX < BARRIER_OFFSETX) ? m_offsetX += 1 : BARRIER_OFFSETX;
	m_offsetY = (m_offsetY < BARRIER_OFFSETY) ? m_offsetY += 1 : BARRIER_OFFSETY;
	m_ratio = (m_ratio < BARRIER_RATIO_OFFSETX) ? m_ratio += 0.0078f : BARRIER_RATIO_OFFSETX;
	if(m_ratio == BARRIER_RATIO_OFFSETX && m_offsetX == BARRIER_OFFSETX && m_offsetY == BARRIER_OFFSETY){
		changeType = 0;
		info.SetChangeType(changeType);
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
	unsigned int changeType = info.GetChangeType();
	if(changeType & CHANGE_ON){
		// �A�j���[�V����
		PriDrawAnimation(changeType, ratios, colors);
	}else{
		m_ratio = 0.0f;
		m_offsetX = 0, m_offsetY = 0;
		for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
			// �w�i�\��
			DrawRotaGraph(m_bgUiList[i].x, m_bgUiList[i].y,
						  m_bgUiList[i].extRate,
						  m_bgUiList[i].angle,
						  m_bgGfxHdl,
						  m_bgUiList[i].transFlag);

			// �o���A�Q�[�W�\��
			double extRateX = (double)(ratios[i] * m_gaugeList[i].extRateX);
			DrawRotaGraph3F(m_gaugeList[i].xf, m_gaugeList[i].yf,
							m_gaugeList[i].cxf, m_gaugeList[i].cyf,
							extRateX, m_gaugeList[i].extRateY,
							m_gaugeList[i].angle,
							m_gfxHdList[colors[i]],
							m_gaugeList[i].transFlag);
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
void CUiBarrier::PriDrawAnimation(unsigned int changeType, float ratios[], int colors[])
{
	unsigned int type = changeType & ~CHANGE_ON;
	switch(type)
	{
	// ������ɐؑփA�j���[�V����
	case CHANGE_UP:
		{
			// �o���A�֑ؑO�̏���ݒ�
			float prevRatios[] = {
				{ratios[BARRIER_BOTTOM]}, {ratios[BARRIER_TOP]}, {ratios[BARRIER_CURRENT]}
			};
			int prevColors[] = {
				{colors[BARRIER_BOTTOM]}, {colors[BARRIER_TOP]}, {colors[BARRIER_CURRENT]}
			};
			S_ANIMATION_INFO animInfo[] = {
				{ 0,  0,  2}, {-1,  1, -1}, { 1, -1, -1}
			};
			// �ؑւ�������ɂ���ĕ`�揇��ؑւ���
			for(int cnt = 0; cnt < MAX_ATTRIBUTE_TYPE; cnt++){
				PriDrawAnimationImpl(cnt, animInfo[cnt].sign, animInfo[cnt].dx, animInfo[cnt].dy, prevRatios[cnt], prevColors[cnt]);
			}
		}
		break;
	// �������ɐؑփA�j���[�V����
	case CHANGE_DOWN:
		{
			// �o���A�֑ؑO�̏���ݒ�
			float prevRatios[] = {
				{ratios[BARRIER_CURRENT]}, {ratios[BARRIER_BOTTOM]}, {ratios[BARRIER_TOP]}
			};
			int prevColors[] = {
				{colors[BARRIER_CURRENT]}, {colors[BARRIER_BOTTOM]}, {colors[BARRIER_TOP]}
			};
			S_ANIMATION_INFO animInfo[] = {
				{ 1, -1,  1}, {-1,  1,  1},	{ 0,  0, -2}
			};
			
			// �ؑւ�������ɂ���ĕ`�揇��ؑւ���
			for(int cnt = MAX_ATTRIBUTE_TYPE - 1; cnt >= 0; cnt--){
				PriDrawAnimationImpl(cnt, animInfo[cnt].sign, animInfo[cnt].dx, animInfo[cnt].dy, prevRatios[cnt], prevColors[cnt]);
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
void CUiBarrier::PriDrawAnimationImpl(int num, int sign, int dx, int dy, float prevRatio, int prevColor)
{
	// �w�iUI�`��
	int y = m_bgUiList[num].y + m_offsetY * dy;
	float exRate = (float)(m_bgUiList[num].extRate + m_ratio * sign);
	DrawRotaGraph(m_bgUiList[num].x, y, exRate, m_bgUiList[num].angle, m_bgGfxHdl, true);

	// �o���AUI�`��
	float xf = m_gaugeList[num].xf + m_offsetX * dx;
	float yf = m_gaugeList[num].yf + m_offsetY * dy;

	double extRateX = (double)(prevRatio * (m_gaugeList[num].extRateX + m_ratio * sign));
	double extRateY = (double)(m_gaugeList[num].extRateY + m_ratio * sign);
	DrawRotaGraph3F(xf, yf, m_gaugeList[num].cxf, m_gaugeList[num].cyf, extRateX, extRateY, m_gaugeList[num].angle, m_gfxHdList[prevColor], true);

	return;
}

} // namespace Ui