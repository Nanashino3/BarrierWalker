//*************************************************************
// アイテムオブジェクトクラス
//*************************************************************
#include "CItemManager.h"

#include "S_ITEM_INFO.h"
#include "../Camera/CCamera2D.h"
#include "../Utility/CImageManager.h"
#include "../ShareInfo/CDocGameInfo.h"
#include "../Sound/CSoundManager.h"

namespace {
// 地形CSV読込用
const std::vector<std::string> g_item_csv = {
	//"resource/item/item_layout_1.csv",
	//"resource/item/item_layout_2.csv",
	"resource/item/item_layout_3.csv"
};

const int ITEM_KIND_COIN = 0;
const int MAX_ITEM_KIND = 1;

std::vector<std::vector<std::string>> g_itemDatas;
std::vector<GameObject::S_ITEM_INFO> g_items;

int g_gfxHdl[MAX_ITEM_KIND] = {0};
}

namespace GameObject
{
CItemManager::CItemManager()
{
	g_gfxHdl[ITEM_KIND_COIN] = Utility::LoadGraphEx("resource/item/sticon2b-3.png");
	m_soundManager = Sound::CSoundManager::GetInstance();
}
	
CItemManager::~CItemManager()
{
	for(int i = 0; i < MAX_ITEM_KIND; i++){
		Utility::DeleteGraphEx(g_gfxHdl[i]);
	}
}

//****************************************************************************
// 関数名：Initialize
// 概　要：初期化処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：各ステージに配置するアイテム情報の読込とデータ設定
//****************************************************************************
void CItemManager::Initialize(ShareInfo::CDocGameInfo& info)
{
	// 要素の初期化とメモリサイズを合わせる
	g_itemDatas.clear();
	g_itemDatas.shrink_to_fit();
	g_items.clear();
	g_items.shrink_to_fit();

	int index = info.GetStageIndex();
	g_itemDatas = tnl::LoadCsv(g_item_csv[index].c_str());
	for(int i = 0; i < g_itemDatas.size(); i++){
		S_ITEM_INFO item_info;
		item_info.imageID = atoi(g_itemDatas[i][0].c_str());
		item_info.item_kind = atoi(g_itemDatas[i][1].c_str());
		item_info.pos_x = atof(g_itemDatas[i][2].c_str()) - (info.GetScreenWidth() >> 1);
		item_info.pos_y = atof(g_itemDatas[i][3].c_str()) - (info.GetScreenHeight() >> 1);
		item_info.isEnable = true;

		g_items.push_back(item_info);
	}
}

//****************************************************************************
// 関数名：Collision
// 概　要：衝突判定処理
// 引　数：第1引数　現在位置
// 　　　　第2引数　前回位置
// 　　　　第3引数　ゲーム情報
// 戻り値：なし
// 詳　細：アイテムとの当たり安定を行う
//****************************************************************************
void CItemManager::Collision(tnl::Vector3& current_pos, tnl::Vector3& prev_pos, ShareInfo::CDocGameInfo& info)
{
	for(int i = 0; i < g_items.size(); i++){
		tnl::Vector3 item_pos = tnl::Vector3(g_items[i].pos_x, g_items[i].pos_y, 0);
		// 有効なアイテムが
		if(!g_items[i].isEnable){ continue; }
		// 衝突したか
		if(!PriCollision(current_pos, item_pos)){ continue;}

		// 衝突した場合はアイテムを無効化し各処理を行う
		g_items[i].isEnable = false;
		unsigned int items = 0;
		switch(g_items[i].item_kind)
		{
		case ITEM_KIND_COIN:
			items |= ITEM_COIN;
			m_soundManager->PlaySE(SE_ID_GET_COIN);
			break;
		default:
			break;
		}
		info.SetAcquiredItem(items);
		return;
	}
}

//****************************************************************************
// 関数名：Draw
// 概　要：描画処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：アイテムの描画処理を行う
//****************************************************************************
void CItemManager::Draw(ShareInfo::CDocGameInfo& info)
{
	Camera::CCamera2D* camera = info.GetCamera();
	int screen_half_w = info.GetScreenWidth() >> 1;
	int screen_half_h = info.GetScreenHeight() >> 1;

	for(int i = 0; i < g_items.size(); i++){
		if(!g_items[i].isEnable){ continue; }
		int view_pos_x = g_items[i].pos_x - camera->GetPosition().x + screen_half_w;
		int view_pos_y = g_items[i].pos_y - camera->GetPosition().y + screen_half_h;
		DrawRotaGraph(view_pos_x, view_pos_y, 1.0f, 0, g_gfxHdl[g_items[i].imageID], true);
	}
}

//****************************************************************************
// 関数名：PriCollision
// 概　要：衝突判定(Private)
// 引　数：第1引数　現在位置
// 　　　　第2引数　アイテム位置
// 戻り値：なし
// 詳　細：アイテムとの衝突判定実処理
//****************************************************************************
bool CItemManager::PriCollision(tnl::Vector3& current_pos, tnl::Vector3& item_pos)
{
	// 矩形A(自機)
	int left_a	 = current_pos.x - 4;
	int right_a	 = current_pos.x + 4;
	int top_a	 = current_pos.y - 48 / 2;
	int bottom_a = current_pos.y + 48 / 2;

	// 矩形B(アイテム)
	int left_b	 = item_pos.x - 24 / 2;
	int right_b	 = item_pos.x + 24 / 2;
	int top_b	 = item_pos.y - 24 / 2;
	int bottom_b = item_pos.y + 24 / 2;

	// 補正なし衝突判定
	return tnl::IsIntersectRectPrimitive(left_a, right_a, top_a, bottom_a, left_b, right_b, top_b, bottom_b);
}

} // namespace GameObject
