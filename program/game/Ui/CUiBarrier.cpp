//****************************************************************************
// ファイル名：CUiBarrier(バリアUI)
// 作　成　日：2022/08/06
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
	// 背景UI用設定値
	std::vector<ShareInfo::S_DRAW_INFO> bgUiList = {
		{192,  82, 0.75f, 0.0f, 0, true},	// top
		{192, 100, 1.0f , 0.0f, 0, true},	// current
		{192, 118, 0.75f, 0.0f, 0, true},	// bottom
	};
	for(const auto& bgUi : bgUiList){
		m_bgUiList.push_back(bgUi);
	}

	// バリアゲージ用設定値
	std::vector<ShareInfo::S_DRAW_INFO_3F> gaugeList = {
		{99,  82, 0, 6, 0.75f, 0.75f, 0.0f, 0, true},	// top
		{67, 100, 0, 6, 1.0f , 1.0f , 0.0f, 0, true},	// current
		{99, 118, 0, 6, 0.75f, 0.75f, 0.0f, 0, true},	// bottom
	};
	for(const auto& gauge : gaugeList){
		m_gaugeList.push_back(gauge);
	}

	// 画像読み込み
	m_bgGfxHdl = Utility::CImageManager::GetInstance()->LoadGraphEx("resource/ui/HPGauge02_bg_01.png");
	std::vector<std::string> fileList = {
		"resource/ui/HPGauge02_red.png",	// 赤ゲージ
		"resource/ui/HPGauge02_green.png",	// 緑ゲージ
		"resource/ui/HPGauge02_blue.png"	// 青ゲージ	
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
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：UIのアニメーション等に使用する変数の更新処理
//****************************************************************************
void CUiBarrier::Update(ShareInfo::CDocGameInfo& info)
{
	unsigned int changeType = info.GetChangeType();
	if(!(changeType & CHANGE_ON)){ return; }

	// UIアニメーション用オフセット計算
	m_offsetX = (m_offsetX < BARRIER_OFFSETX) ? m_offsetX += 1 : BARRIER_OFFSETX;
	m_offsetY = (m_offsetY < BARRIER_OFFSETY) ? m_offsetY += 1 : BARRIER_OFFSETY;
	m_ratio = (m_ratio < BARRIER_RATIO_OFFSETX) ? m_ratio += 0.0078f : BARRIER_RATIO_OFFSETX;
	if(m_ratio == BARRIER_RATIO_OFFSETX && m_offsetX == BARRIER_OFFSETX && m_offsetY == BARRIER_OFFSETY){
		changeType = 0;
		info.SetChangeType(changeType);
	}
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：バリアUIの描画処理
//****************************************************************************
void CUiBarrier::Draw(ShareInfo::CDocGameInfo& info)
{
	// バリア比率
	float ratios[] = {
		{info.GetBarrierEnergy(BARRIER_TOP) / MAX_ENERGY},
		{info.GetBarrierEnergy(BARRIER_CURRENT) / MAX_ENERGY},
		{info.GetBarrierEnergy(BARRIER_BOTTOM) / MAX_ENERGY}
	};
	// バリアの色
	int colors[] = {
		{info.GetBarrierColor(BARRIER_TOP)},
		{info.GetBarrierColor(BARRIER_CURRENT)},
		{info.GetBarrierColor(BARRIER_BOTTOM)}
	};

	//*************************************
	// ここでやりたいことは以下
	// ・入替え処理をしていない場合は固定描画
	// ・入替えONになった場合、固定描画を一旦非表示にしアニメーション用描画を行う
	// 　アニメーションが終了したら、固定描画に戻す
	//**************************************
	unsigned int changeType = info.GetChangeType();
	if(changeType & CHANGE_ON){
		// アニメーション
		PriDrawAnimation(changeType, ratios, colors);
	}else{
		m_ratio = 0.0f;
		m_offsetX = 0, m_offsetY = 0;
		for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
			// 背景表示
			DrawRotaGraph(m_bgUiList[i].x, m_bgUiList[i].y,
						  m_bgUiList[i].extRate,
						  m_bgUiList[i].angle,
						  m_bgGfxHdl,
						  m_bgUiList[i].transFlag);

			// バリアゲージ表示
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
// 関数名：PriDrawAnimation
// 概　要：アニメーション(Private)
// 引　数：第1引数　切替種(バリアのON/OFFや上下どちらに切替えるかの情報)
// 　　　　第2引数　比率配列
// 　　　　第3引数　色配列
// 戻り値：なし
// 詳　細：バリア切替アニメーション
//****************************************************************************
void CUiBarrier::PriDrawAnimation(unsigned int changeType, float ratios[], int colors[])
{
	unsigned int type = changeType & ~CHANGE_ON;
	switch(type)
	{
	// 上方向に切替アニメーション
	case CHANGE_UP:
		{
			// バリア切替前の情報を設定
			float prevRatios[] = {
				{ratios[BARRIER_BOTTOM]}, {ratios[BARRIER_TOP]}, {ratios[BARRIER_CURRENT]}
			};
			int prevColors[] = {
				{colors[BARRIER_BOTTOM]}, {colors[BARRIER_TOP]}, {colors[BARRIER_CURRENT]}
			};
			S_ANIMATION_INFO animInfo[] = {
				{ 0,  0,  2}, {-1,  1, -1}, { 1, -1, -1}
			};
			// 切替える方向によって描画順を切替える
			for(int cnt = 0; cnt < MAX_ATTRIBUTE_TYPE; cnt++){
				PriDrawAnimationImpl(cnt, animInfo[cnt].sign, animInfo[cnt].dx, animInfo[cnt].dy, prevRatios[cnt], prevColors[cnt]);
			}
		}
		break;
	// 下方向に切替アニメーション
	case CHANGE_DOWN:
		{
			// バリア切替前の情報を設定
			float prevRatios[] = {
				{ratios[BARRIER_CURRENT]}, {ratios[BARRIER_BOTTOM]}, {ratios[BARRIER_TOP]}
			};
			int prevColors[] = {
				{colors[BARRIER_CURRENT]}, {colors[BARRIER_BOTTOM]}, {colors[BARRIER_TOP]}
			};
			S_ANIMATION_INFO animInfo[] = {
				{ 1, -1,  1}, {-1,  1,  1},	{ 0,  0, -2}
			};
			
			// 切替える方向によって描画順を切替える
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
// 関数名：PriDrawAnimationImpl
// 概　要：アニメーション用描画(Private)
// 引　数：第1引数　配列にアクセスするための添字
// 　　　　第2引数　符号(拡大縮小の方向に関わる)
// 　　　　第3引数　X方向の移動値
// 　　　　第4引数　Y方向の移動値
// 　　　　第5引数　入替え前の比率
// 　　　　第6引数　入替え前の色
// 戻り値：なし
// 詳　細：アニメーション用描画処理
//****************************************************************************
void CUiBarrier::PriDrawAnimationImpl(int num, int sign, int dx, int dy, float prevRatio, int prevColor)
{
	// 背景UI描画
	int y = m_bgUiList[num].y + m_offsetY * dy;
	float exRate = (float)(m_bgUiList[num].extRate + m_ratio * sign);
	DrawRotaGraph(m_bgUiList[num].x, y, exRate, m_bgUiList[num].angle, m_bgGfxHdl, true);

	// バリアUI描画
	float xf = m_gaugeList[num].xf + m_offsetX * dx;
	float yf = m_gaugeList[num].yf + m_offsetY * dy;

	double extRateX = (double)(prevRatio * (m_gaugeList[num].extRateX + m_ratio * sign));
	double extRateY = (double)(m_gaugeList[num].extRateY + m_ratio * sign);
	DrawRotaGraph3F(xf, yf, m_gaugeList[num].cxf, m_gaugeList[num].cyf, extRateX, extRateY, m_gaugeList[num].angle, m_gfxHdList[prevColor], true);

	return;
}

} // namespace Ui