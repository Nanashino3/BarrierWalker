//*************************************************************
// 固定ステージクラス
//*************************************************************
#include "CFixedFloor.h"

#include "../Camera/CCamera2D.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Utility/CMath.h"

namespace {
// 矩形サイズ
const int P_RECT_W = 20;
const int P_RECT_H = PLAYER_RECT_H;
const int RECT_W = MAP_CHIP_W << 1;
const int RECT_H = MAP_CHIP_H << 1;

// 地形CSV読込用
const std::vector<std::string> g_stageCsv = {
	//"resource/stage/stage_layout_1.csv",
	//"resource/stage/stage_layout_2.csv",
	"resource/stage/stage_layout_3.csv"
};

// 地形操作用
typedef std::vector<std::vector<std::string>> CSV_DATA;
CSV_DATA g_stageData;

// 地形画像ハンドル
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
	// メモリ上から削除する
	for(int i = 0; i < MAX_MAP_CHIP; i++){
		DeleteGraph(g_gfxHdl[i]);
	}
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各ステージ情報(CSV)の読込とデータ設定
//****************************************************************************
void CFixedFloor::Initialize(ShareInfo::CDocGameInfo& info)
{
	// 要素の初期化とメモリサイズを合わせる
	g_stageData.clear();
	g_stageData.shrink_to_fit();

	// ステージ情報取得
	int index = info.GetStageIndex();
	g_stageData = tnl::LoadCsv(g_stageCsv[index].c_str());

	// 最大ステージ数設定(0オリジン)
	info.SetMaxStage(g_stageCsv.size() - 1);

	// 最大画面数(ブロック総数 / 1画面に表示できるブロック数)
	int screen_max = g_stageData[0].size() / SIZE_32;
	info.SetScreenMax(screen_max);

	// 初期ブロック配置位置
	m_firstPosX = SIZE_16 - ((g_stageData[0].size() / screen_max) * SIZE_32 * 0.5f);
 	m_firstPosY = SIZE_16 - (g_stageData.size() * SIZE_32 * 0.5f);
}

//****************************************************************************
// 関数名：Collision
// 概　要：衝突判定処理
// 引　数：第1引数　現在位置
// 　　　　第2引数　前回位置
// 　　　　第3引数　ゲーム情報
// 戻り値：なし
// 詳　細：ステージの衝突判定処理
//****************************************************************************
void CFixedFloor::Collision(tnl::Vector3& current_pos, tnl::Vector3& prev_pos, ShareInfo::CDocGameInfo& info)
{
	// TODO：1マスダッシュ処理
	//ShareInfo::S_VELOCITY_INFO velocity = info.GetVelocity();
	for(int i = 0; i < g_stageData.size(); i++){
		for(int j = 0; j < g_stageData[i].size(); j++){
			int stage_num = std::atoi(g_stageData[i][j].c_str());
			if(stage_num == 0){ continue; }

			// 各チップの相対座標を計算
			int stage_pos_x = m_firstPosX + j * SIZE_32;
			int stage_pos_y = m_firstPosY + i * SIZE_32;
			tnl::Vector3 obj = tnl::Vector3(stage_pos_x, stage_pos_y, 0);

			// TODO：1マスダッシュ処理
			//static int prev_corrent_tyep = 0;

			// 当たり判定処理
			int correct_type = IsIntersectRectToCorrectPosition(current_pos, prev_pos, P_RECT_W, P_RECT_H, obj, RECT_W, RECT_H);
			if(correct_type != 0){
				info.SetIsCollision(true);
				info.SetCorrectType(ShareInfo::COLLISION_OBJECT_STAGE, correct_type);
				info.SetCurrentPos(current_pos);

				// TODO：1マスダッシュ処理
				//if(prev_corrent_tyep == 3 && ((correct_type == 1) || (correct_type == 2))){
				//	tnl::DebugTrace("衝突中！！\n");
				//}
				//prev_corrent_tyep = correct_type;
			}
		}
	}
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：ステージの描画処理
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

			// 各チップの相対座標を計算
			int stage_pos_x = m_firstPosX + j * SIZE_32;
			int stage_pos_y = m_firstPosY + i * SIZE_32;

			// 描画用座標に変換
			int view_stagePos_x = stage_pos_x - camera->GetPosition().x + screen_half_w;
			int view_stagePos_y = stage_pos_y - camera->GetPosition().y + screen_half_h;

			DrawRotaGraph(view_stagePos_x, view_stagePos_y, 2.0f, 0, g_gfxHdl[imageID - 1], true);
		}
	}
}

} // namespace GameObject