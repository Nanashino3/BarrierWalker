//*************************************************************
// バリアゲージUIクラス
//*************************************************************
#include "CUiBarrier.h"

#include "../Utility/CImageManager.h"
#include "../ShareInfo/S_DRAW_INFO.h"
#include "../ShareInfo/S_DRAW_INFO_3F.h"
#include "../ShareInfo/CDocGameInfo.h"

namespace{
// バリアUI画像
const std::vector<std::string> g_barrierGauge = {
	"resource/ui/HPGauge02_red.png",	// 赤ゲージ
	"resource/ui/HPGauge02_green.png",	// 緑ゲージ
	"resource/ui/HPGauge02_blue.png"	// 青ゲージ
};
// 背景UI用設定値
const std::vector<ShareInfo::S_DRAW_INFO> g_bgUi = {
	{192,  82, 0.75f, 0.0f, 0, true},	// top
	{192, 100, 1.0f , 0.0f, 0, true},	// current
	{192, 118, 0.75f, 0.0f, 0, true},	// bottom
};
// バリアゲージ用設定値
const std::vector<ShareInfo::S_DRAW_INFO_3F> g_ui = {
	{99,  82, 0, 6, 0.75f, 0.75f, 0.0f, 0, true},	// top
	{67, 100, 0, 6, 1.0f , 1.0f , 0.0f, 0, true},	// current
	{99, 118, 0, 6, 0.75f, 0.75f, 0.0f, 0, true},	// bottom
};
// オフセット用設定値
const int UI_OFFSET_X = 32;
const int UI_OFFSET_Y = 18;
const float UI_RATIO_OFFSET_X = 0.25f;
// アニメーション情報
struct ANIMATION_INFO
{
	int sign;	// 符号
	int dx;		// X方向(1：右、-1：左)
	int dy;		// Y方向(1：下、-1：上)
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
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：UIのアニメーション等に使用する変数の更新処理
//****************************************************************************
void CUiBarrier::Update(ShareInfo::CDocGameInfo& info)
{
	unsigned int change_type = info.GetChangeType();
	if(!(change_type & CHANGE_ON)){ return; }

	// UIアニメーション用オフセット計算
	m_offset_x = (m_offset_x < UI_OFFSET_X) ? m_offset_x += 1 : UI_OFFSET_X;
	m_offset_y = (m_offset_y < UI_OFFSET_Y) ? m_offset_y += 1 : UI_OFFSET_Y;
	m_ratio = (m_ratio < UI_RATIO_OFFSET_X) ? m_ratio += 0.0078f : UI_RATIO_OFFSET_X;
	if(m_ratio == UI_RATIO_OFFSET_X && m_offset_x == UI_OFFSET_X && m_offset_y == UI_OFFSET_Y){
		change_type = 0;
		info.SetChangeType(change_type);
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
	unsigned int change_type = info.GetChangeType();
	if(change_type & CHANGE_ON){
		// アニメーション
		PriDrawAnimation(change_type, ratios, colors);
	}else{
		m_ratio = 0.0f;
		m_offset_x = 0, m_offset_y = 0;
		for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
			// 背景表示
			DrawRotaGraph(g_bgUi[i].x, g_bgUi[i].y, g_bgUi[i].extRate, g_bgUi[i].angle, m_bg_gfx_hdl, g_bgUi[i].transFlag);
			// バリアゲージ表示
			double extRateX = (double)(ratios[i] * g_ui[i].extRateX);
			DrawRotaGraph3F(g_ui[i].xf, g_ui[i].yf, g_ui[i].cxf, g_ui[i].cyf, extRateX, g_ui[i].extRateY, g_ui[i].angle, g_gfxHdl[colors[i]], g_ui[i].transFlag);
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
void CUiBarrier::PriDrawAnimation(unsigned int change_type, float ratios[], int colors[])
{
	unsigned int type = change_type & ~CHANGE_ON;
	switch(type)
	{
	// 上方向に切替アニメーション
	case CHANGE_UP:
		{
			// バリア切替前の情報を設定
			float prev_ratios[] = {
				{ratios[BARRIER_BOTTOM]}, {ratios[BARRIER_TOP]}, {ratios[BARRIER_CURRENT]}
			};
			int prev_colors[] = {
				{colors[BARRIER_BOTTOM]}, {colors[BARRIER_TOP]}, {colors[BARRIER_CURRENT]}
			};
			ANIMATION_INFO anim_info[] = {
				{ 0,  0,  2}, {-1,  1, -1}, { 1, -1, -1}
			};
			// 切替える方向によって描画順を切替える
			for(int cnt = 0; cnt < MAX_ATTRIBUTE_TYPE; cnt++){
				PriDrawAnimationImpl(cnt, anim_info[cnt].sign, anim_info[cnt].dx, anim_info[cnt].dy, prev_ratios[cnt], prev_colors[cnt]);
			}
		}
		break;
	// 下方向に切替アニメーション
	case CHANGE_DOWN:
		{
			// バリア切替前の情報を設定
			float prev_ratios[] = {
				{ratios[BARRIER_CURRENT]}, {ratios[BARRIER_BOTTOM]}, {ratios[BARRIER_TOP]}
			};
			int prev_colors[] = {
				{colors[BARRIER_CURRENT]}, {colors[BARRIER_BOTTOM]}, {colors[BARRIER_TOP]}
			};
			ANIMATION_INFO anim_info[] = {
				{ 1, -1,  1}, {-1,  1,  1},	{ 0,  0, -2}
			};
			
			// 切替える方向によって描画順を切替える
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
void CUiBarrier::PriDrawAnimationImpl(int num, int sign, int dx, int dy, float prev_ratio, int prev_color)
{
	// 背景UI描画
	int y = g_bgUi[num].y + m_offset_y * dy;
	float exRate = (float)(g_bgUi[num].extRate + m_ratio * sign);
	DrawRotaGraph(g_bgUi[num].x, y, exRate, g_bgUi[num].angle, m_bg_gfx_hdl, true);

	// バリアUI描画
	float xf = g_ui[num].xf + m_offset_x * dx;
	float yf = g_ui[num].yf + m_offset_y * dy;

	double extRateX = (double)(prev_ratio * (g_ui[num].extRateX + m_ratio * sign));
	double extRateY = (double)(g_ui[num].extRateY + m_ratio * sign);
	DrawRotaGraph3F(xf, yf, g_ui[num].cxf, g_ui[num].cyf, extRateX, extRateY, g_ui[num].angle, g_gfxHdl[prev_color], true);

	return;
}

} // namespace Ui