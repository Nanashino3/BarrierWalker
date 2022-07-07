#include "CPauseScene.h"
#include "CPlayScene.h"
#include "CHelpScene.h"
#include "CSecondaryController.h"

#include "../../Utility/CMenu.h"
#include "../../Utility/CUtility.h"
#include "../../CGameDirector.h"
#include "../../ShareInfo/CDocGameInfo.h"
#include "../../Sound/CSoundManager.h"

namespace {
const std::string RETURN_GAME_TEXT = "ゲームへ戻る";
const std::string HELP_TEXT = "ヘルプ";
const std::string RETURN_TITLE_TEXT = "タイトルへ戻る";
const std::vector<std::string> message_text = {
	RETURN_GAME_TEXT, HELP_TEXT, RETURN_TITLE_TEXT
};

const int PAUSE_LABEL_X = 450;
const int PAUSE_LABEL_Y = 300;

const int FIRST_LABEL_X = 450;	// 初期ラベル座標：X軸
const int FIRST_LABEL_Y = 384;	// 初期ラベル座標：Y軸

const int ALIGNMENT_X = 20;		// ラベル表示間隔：X軸
const int ALIGNMENT_Y = 20;		// ラベル表示間隔：Y軸

std::tuple<bool, std::string> result;
}

namespace Sequence
{
namespace PlayGame
{
CPauseScene::CPauseScene()
{
	m_soundManager = Sound::CSoundManager::GetInstance();
	// メニュー選択関連の初期化
	result = std::make_tuple(false, "");
	Utility::SelectReset();
}

CPauseScene::~CPauseScene()
{}

IScene* CPauseScene::Update(CSecondaryController& controller, ShareInfo::CDocGameInfo& info)
{
	IScene* next_scene = this;
	// ゲーム画面描画
	controller.GetGameInstance()->Draw();

	DrawStringEx(PAUSE_LABEL_X, PAUSE_LABEL_Y, GetColor(255, 255, 0), "PAUSE");

	if(tnl::Input::IsKeyDownTrigger(eKeys::KB_G)){
		m_soundManager->PlaySE(SE_ID_PAUSE_CLOSE);
		return new CPlayScene(false);
	}

	// メニュー描画
	Utility::MenuDraw(message_text,
					  FIRST_LABEL_X, FIRST_LABEL_Y,
					  ALIGNMENT_X, ALIGNMENT_Y,
					  GetColor(255, 255, 0), GetColor(255, 255, 0));

	// 選択したメニュー毎の処理
	if(!std::get<0>(result)){
		// メニュー選択
		result = Utility::MenuSelect(message_text);
	}else{
		std::string message_text = std::get<1>(result);
		if(message_text == RETURN_GAME_TEXT){
			next_scene = new CPlayScene(false);
		}else if(message_text == HELP_TEXT){
			next_scene = new CHelpScene();
		}else if(message_text == RETURN_TITLE_TEXT){
			if(!Utility::FadeOut()){ return next_scene; }
			controller.MoveToScene(CSecondaryController::PRIMARY_ID_TITLE);
			m_soundManager->AllStopSound();
		}else{
			tnl::DebugTrace("CPauseScene->Error No Text\n");
		}
		info.SetPrevScene(PAUSE_SCENE);
	}

	return next_scene;
}

} // namespace PlayGame

} // namespace Sequence