#include "CHelpScene.h"
#include "CPauseScene.h"
#include "CSecondaryController.h"

#include "../../CGameDirector.h"
#include "../../Utility/CMenu.h"
#include "../../ShareInfo/CDocGameInfo.h"

namespace {
// TODO：メッセージ系もCSVにしたい
const std::string COMMAND_MOVE_R		= "Dキ－";
const std::string COMMAND_MOVE_L		= "Aキ－";
const std::string COMMAND_JUMP			= "SPACE";
const std::string COMMAND_DASH			= "L SHIFT";
const std::string COMMAND_BARRIER		= "Bキ－";
const std::string COMMAND_CHANGE_UP		= "Wキ－";
const std::string COMMAND_CHANGE_DOWN	= "Sキ－";
const std::string COMMAND_PAUSE			= "Gキ－";
const std::vector<std::string> operate_text = {
	COMMAND_MOVE_R, COMMAND_MOVE_L, COMMAND_JUMP, COMMAND_DASH,
	COMMAND_BARRIER, COMMAND_CHANGE_UP, COMMAND_CHANGE_DOWN, COMMAND_PAUSE
};

const std::string OPE_NAME_MOVE_R		= "移動：右方向";
const std::string OPE_NAME_MOVE_L		= "移動：左方向";
const std::string OPE_NAME_JUMP			= "ジャンプ";
const std::string OPE_NAME_DASH			= "ダッシュ";
const std::string OPE_NAME_BARRIER		= "バリア展開";
const std::string OPE_NAME_CHANGE_UP	= "バリア切替：上方向";
const std::string OPE_NAME_CHANGE_DOWN	= "バリア切替：下方向";
const std::string OPE_NAME_PAUSE		= "ポーズ/ポーズ解除";
const std::vector<std::string> action_text = {
	OPE_NAME_MOVE_R, OPE_NAME_MOVE_L, OPE_NAME_JUMP, OPE_NAME_DASH,
	OPE_NAME_BARRIER, OPE_NAME_CHANGE_UP, OPE_NAME_CHANGE_DOWN, OPE_NAME_PAUSE
};

const int FIRST_LABEL_X = 400;	// ラベル座標：X軸
const int FIRST_LABEL_Y = 250;	// ラベル座標：Y軸

const int TITLE_LABEL_X = 450;	// ラベル座標：X軸
const int TITLE_LABEL_Y = 450;	// ラベル座標：Y軸

const int ALIGNMENT_X = 20;		// ラベル表示間隔：X軸
const int ALIGNMENT_Y = 20;		// ラベル表示間隔：Y軸

const std::string RETURN_TEXT = "戻る";
std::vector<std::string> message_text = { RETURN_TEXT };

std::tuple<bool, std::string> result;
}

namespace Sequence
{
namespace PlayGame
{
CHelpScene::CHelpScene()
{
	result = std::make_tuple(false, "");
	Utility::SelectReset();
}

CHelpScene::~CHelpScene()
{}

IScene* CHelpScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;

	// ゲーム画面でヘルプ画面を表示
	if(info.GetPrevScene() == PAUSE_SCENE){
		controller.GetGameInstance()->Draw();
	}

	// コマンドテキスト描画
	for(int i = 0; i < operate_text.size(); i++){
		DrawStringEx(FIRST_LABEL_X, FIRST_LABEL_Y + i * ALIGNMENT_Y, GetColor(255, 255, 0), "%s", operate_text[i].c_str());
	}

	// 区切り文字描画
	for(int i = 0; i < operate_text.size(); i++){
		DrawStringEx(FIRST_LABEL_X + 75, FIRST_LABEL_Y + i * ALIGNMENT_Y, GetColor(255, 255, 0), "%s", "：");
	}

	// 操作テキスト描画
	for(int i = 0; i < action_text.size(); i++){
		DrawStringEx(FIRST_LABEL_X + 100, FIRST_LABEL_Y + i * ALIGNMENT_Y, GetColor(255, 255, 0), "%s", action_text[i].c_str());
	}

	// メニュー選択
	Utility::MenuDraw(message_text,
					  TITLE_LABEL_X, TITLE_LABEL_Y,
					  ALIGNMENT_X, ALIGNMENT_Y,
					  GetColor(255, 255, 0), GetColor(255, 255, 0));

	// 選択したメニュー毎の処理
	if(!std::get<0>(result)){
		// メニュー選択
		result = Utility::MenuSelect(message_text);
	}else{
		std::string message_text = std::get<1>(result);
		if(message_text == RETURN_TEXT){
			if(info.GetPrevScene() == PAUSE_SCENE){
				// ポーズ画面に戻る
				next_scene = new CPauseScene();
			}else{
				// タイトル画面に戻る
				controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
			}
		}else{
			tnl::DebugTrace("CHelpScene->Error No Text\n");
		}
		info.SetPrevScene(HELP_SCENE);
	}

	return next_scene;
}

} // namespace PlayGame

} // namespace Sequence