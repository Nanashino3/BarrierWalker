//*************************************************************
// バリア機能クラス
//*************************************************************
#include "CBarrier.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../ShareInfo/S_BARRIER_INFO.h"
#include "../ShareInfo/CONST_GAME_VALUE.h"
#include "../Sound/CSoundManager.h"

namespace{
// アニメーション関連
const int ANIM_FRAME_MAX = 10;
// 画像読込用定数
const int ALL_NUM = 10;
const int X_NUM	  = 5;
const int Y_NUM   = 2;
const int X_SIZE  = 600 / X_NUM;
const int Y_SIZE  = 240 / Y_NUM;
// 使用画像設定
const std::vector<std::string> g_images = {
	"resource/pipo-btleffect111g.png",	// 赤
	"resource/pipo-btleffect111d.png",	// 緑
	"resource/pipo-btleffect111e.png"	// 青
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
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：なし
// 戻り値：なし
// 詳　細：画像のロードと情報の初期設定
//****************************************************************************
void CBarrier::Initialize()
{
	for (int i = 0; i < g_images.size(); i++) {
		// 画像のロード
		LoadDivGraph(g_images[i].c_str(), ALL_NUM, X_NUM, Y_NUM, X_SIZE, Y_SIZE, g_gfxHdl[i]);
	}
	m_soundManager = Sound::CSoundManager::GetInstance();

	g_barriers.clear();
	g_barriers.shrink_to_fit();
	for(int i = 0; i < MAX_ATTRIBUTE_TYPE; i++){
		// バリア情報設定
		ShareInfo::S_BARRIER_INFO barrier = { i, MAX_ENERGY, false, false };
		g_barriers.push_back(barrier);
	}
}

//****************************************************************************
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：バリア展開やエネルギー増減、入替え処理等
//****************************************************************************
void CBarrier::Update(ShareInfo::CDocGameInfo& info)
{
	unsigned int function_type = info.GetFunctionType();
	unsigned int change_type = info.GetChangeType();

	// バリア選択
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

	// バリア入れ替え
	if(isChangeBarrier){ PriSwap(change_type, function_type, next, prev); }

	// バリア展開 and バリアが空ではない
	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_B) && !g_barriers[BARRIER_CURRENT].isEmpty){
		if(function_type & m_function_type){
			// バリアOFF
			function_type &= ~m_function_type;
			g_barriers[BARRIER_CURRENT].isBarrier = false;
			m_soundManager->StopSE(SE_ID_BARRIER);
		}else{
			// バリアON
			function_type |= m_function_type;
			g_barriers[BARRIER_CURRENT].isBarrier = true;
			m_soundManager->PlaySE(SE_ID_BARRIER);
		}
	}

	// バリア増減処理
	for(int count = 0; count < g_barriers.size(); count++){
		// バリア切替中ではない
		if(!(change_type & CHANGE_ON)){
			// 増減処理
			PriFluctuation(function_type, count, info);
		}
		// 比率計算とUI入替え処理のために切替の有無にかかわらず設定する
		info.SetBarrierEnergy(count, g_barriers[count].energy);
		info.SetBarrierColor(count, g_barriers[count].color);
	}
	info.SetChangeType(change_type);
	info.SetFunctionType(function_type);
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：バリアの描画
//****************************************************************************
void CBarrier::Draw(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 pos = info.GetCurrentPos();
	Camera::CCamera2D* camera = info.GetCamera();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;

	unsigned int change_type = info.GetChangeType();
	for(int i = 0; i < g_barriers.size(); i++){
		// バリア切替中 or バリアを展開していない場合は表示しない
		if((change_type & CHANGE_ON) || !g_barriers[i].isBarrier){ continue; }
		int view_pos_x = pos.x - camera->GetPosition().x + screen_half_w;
		int view_pos_y = pos.y - camera->GetPosition().y + screen_half_h;
		DrawRotaGraph(view_pos_x, view_pos_y, 1.0f, 0, g_gfxHdl[m_animType][m_animFrame], true);
	}
}

//****************************************************************************
// 関数名：PriSwap
// 概　要：入替え処理(Private)
// 引　数：第1引数　切替種(バリアのON/OFFや上下どちらに切替えるかの情報)
// 　　　　第2引数　機能種(現在ONになっている機能)
// 　　　　第3引数　次に入替えるバリア
// 　　　　第4引数　前に入替えてあるはずのバリア
// 戻り値：なし
// 詳　細：バリア情報の入替え処理
//****************************************************************************
void CBarrier::PriSwap(unsigned int& change_type, unsigned int& function_type, int next, int prev)
{
	// 入替え時にバリアをOFFする
	function_type &= ~m_function_type;
	m_soundManager->StopSE(SE_ID_BARRIER);
	m_soundManager->PlaySE(SE_ID_BARRIER_SELECT);

	// 切替フラグを立てる
	change_type |= CHANGE_ON;
	g_barriers[BARRIER_CURRENT].isBarrier = false;

	ShareInfo::S_BARRIER_INFO temp = g_barriers[BARRIER_CURRENT];
	g_barriers[BARRIER_CURRENT] = g_barriers[next];
	g_barriers[next] = g_barriers[prev];
	g_barriers[prev] = temp;

	// 現在の色にアニメーション種別を合わせる
	m_animType = g_barriers[BARRIER_CURRENT].color;
}

//****************************************************************************
// 関数名：PriFluctuation
// 概　要：増減処理(Private)
// 引　数：第1引数　機能種(現在ONになっている機能)
// 　　　　第2引数　バリア配列にアクセスするための番号
// 　　　　第3引数　ゲーム情報
// 戻り値：なし
// 詳　細：各属性バリアのエネルギ－増減処理
//****************************************************************************
void CBarrier::PriFluctuation(unsigned int& function_type, int num, ShareInfo::CDocGameInfo& info)
{
	if(g_barriers[num].isBarrier){
		g_barriers[num].energy = (g_barriers[num].energy > 0.0f) ? g_barriers[num].energy - 0.35f : 0.0f;
		// バリアエネルギーが空か
		if(g_barriers[num].energy == 0.0f){
			g_barriers[num].isEmpty = true;
			g_barriers[num].isBarrier = false;
			// バリア機能をOFFする
			if(m_animType == g_barriers[num].color){ function_type &= ~m_function_type; }
		}

		// アニメーション処理
		static float anim_time_count = 0.0f;
		anim_time_count += info.GetDeltaTime();
		if(anim_time_count > 0.05f){
			anim_time_count = 0.0f;
			m_animFrame++;
			m_animFrame %= (ANIM_FRAME_MAX / 2);
		}
	}else{
		// バリア未使用 or バリアを使い切った場合
		g_barriers[num].energy = (g_barriers[num].energy < MAX_ENERGY) ? g_barriers[num].energy + 0.1f : MAX_ENERGY;
		if(g_barriers[num].energy == MAX_ENERGY){ g_barriers[num].isEmpty = false; }
	}
}

} // namespace Function