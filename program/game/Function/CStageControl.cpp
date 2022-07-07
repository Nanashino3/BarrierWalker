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
// 関数名：Update
// 概　要：更新処理
// 引　数：第1引数　ゲーム情報
// 戻り値：なし
// 詳　細：ステージ更新と最終ステージ判定
//****************************************************************************
void CStageControl::Update(ShareInfo::CDocGameInfo& info)
{
	tnl::Vector3 camPos = info.GetCamera()->GetPosition();
	tnl::Vector3 pos = info.GetCurrentPos();
	
	// スクリーン幅(半分)
	int screenHalfW = info.GetScreenWidth() >> 1;
	// プレイヤーの矩形幅(半分)
	int rectHalfW = PLAYER_RECT_W >> 1;
	// 現在のステージ番号取得
	int stage_index = info.GetStageIndex();
	// 描画座標に変換
	int playerPosX = pos.x - camPos.x + screenHalfW;

	// 以下のアクション動作中か確認
	bool isJump = (info.GetActionType() & IS_JUMPING) ? true : false;
	bool isAir = (info.GetActionType() & IS_AIR) ? true : false;

	if(!info.GetIsClear() && !isJump && !isAir){
		if((playerPosX == info.GetScreenWidth() - rectHalfW)){
			// とりあえずステージはクリア
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